/**
* @file SkinnedMeshRenderer.h
* @brief スキンメッシュ用レンダリング処理
* @author GP12B183 12 小峰遥一
* @details
*/

#include "stdafx.h"


/**
* @brief 初期化処理
*/
void SkinnedMeshRenderer::Init(void)
{
	_skinnedMeshMgr = SkinnedMeshManager();

	InitShader();
	InitConstantBuffer();
}


/**
* @brief シェーダー初期化
*/
void SkinnedMeshRenderer::InitShader(void)
{
	HRESULT hr;
	ID3DBlob* pErrorBlob;
	ID3DBlob* pVSBlob = NULL;
	DWORD shFlag = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(_DEBUG) && defined(DEBUG_SHADER)
	shFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	// 頂点シェーダー
	{
		hr = D3DX11CompileFromFile("Shader/Skinning.hlsl", NULL, NULL, "VS_Main", "vs_5_0", shFlag, 0, NULL, &pVSBlob, &pErrorBlob, NULL);
		if (FAILED(hr)) MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "VS", MB_OK | MB_ICONERROR);
		DX11.GetDevice()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &_vsSkining);
	}

	// 入力レイアウト生成
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION",     0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",       0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",        0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",     0, DXGI_FORMAT_R32G32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDINDICES", 0, DXGI_FORMAT_R8G8B8A8_UINT,			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = ARRAYSIZE(layout);

		hr = DX11.GetDevice()->CreateInputLayout(layout, numElements,
			pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &_inputLayout);
	}

	pVSBlob->Release();
	if (pErrorBlob) pErrorBlob->Release();

	// ピクセルシェーダー
	ID3DBlob* pPSBlob = NULL;

	hr = D3DX11CompileFromFile("Shader/Skinning.hlsl", NULL, NULL, "PS_Main", "ps_5_0", shFlag, 0, NULL, &pPSBlob, &pErrorBlob, NULL);
	if (FAILED(hr)) MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "PS", MB_OK | MB_ICONERROR);
	DX11.GetDevice()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &_psSkining);

	pPSBlob->Release();
	if (pErrorBlob) pErrorBlob->Release();
}


/**
* @brief 定数バッファ初期化処理
*/
void SkinnedMeshRenderer::InitConstantBuffer(void)
{
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(XMMATRIX) * BONE_MAX;
	hBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);
	DX11.GetDevice()->CreateBuffer(&hBufferDesc, NULL, &_combBuffer);
}


/**
* @brief 終了処理
*/
void SkinnedMeshRenderer::Uninit(void)
{
	_skinnedMeshMgr.ReleaseAll();

	SAFE_RELEASE(_vsSkining);
	SAFE_RELEASE(_psSkining);
	SAFE_RELEASE(_combBuffer);
	SAFE_RELEASE(_inputLayout)
}


/**
* @brief 描画処理
*/
void SkinnedMeshRenderer::Draw(void)
{
	Renderer* pRenderer = Game.GetRenderer();

	// ブレンドモード
	DX11.SetBlendState(DX11.BLEND_MODE_ALPHA);

	// αテスト
	DX11.SetAlphaTest(FALSE);

	// レンダーターゲットビュー
	DX11.SetRenderTarget();

	// ビューポート
	DX11.SetViewPort();

	// 入力レイアウト
	DX11.GetDeviceContext()->IASetInputLayout(_inputLayout);

	// カメラ視点に変更
	CameraActor* mainCamera = Game.GetMainCamera();
	pRenderer->SetViewMatrix(&XMLoadFloat4x4(mainCamera->GetViewMatrix()));
	pRenderer->SetProjMatrix(&XMLoadFloat4x4(mainCamera->GetProjMatrix()));

	// 頂点シェーダー
	DX11.GetDeviceContext()->VSSetShader(_vsSkining, nullptr, 0);
	DX11.GetDeviceContext()->VSSetConstantBuffers(0, 1, &_combBuffer);
	DX11.GetDeviceContext()->VSSetConstantBuffers(1, 1, pRenderer->GetViewBuffer());
	DX11.GetDeviceContext()->VSSetConstantBuffers(2, 1, pRenderer->GetProjBuffer());

	// ピクセルシェーダー
	DX11.GetDeviceContext()->PSSetShader(_psSkining, nullptr, 0);
	DX11.GetDeviceContext()->PSSetConstantBuffers(3, 1, pRenderer->GetMaterialBuffer());

	// サンプラセット
	ID3D11SamplerState* sampler = pRenderer->GetDefaultSamplerState();
	DX11.GetDeviceContext()->PSSetSamplers(0, 1, &sampler);

	// メッシュを順番に描画
	for (auto mesh : _skinnedMeshs)
	{
		mesh->Draw();
	}

}


/**
* @brief 合成変換行列バッファのセット
*/
void SkinnedMeshRenderer::SetCombMatrix(XMFLOAT4X4 * mtxCombAry)
{
	// 転置する
	XMFLOAT4X4 transposeAry[BONE_MAX];
	for (int i = 0; i < BONE_MAX; i++)
	{
		XMStoreFloat4x4(&transposeAry[i], XMMatrixTranspose(XMLoadFloat4x4(&mtxCombAry[i])));
	}

	D3D11_MAPPED_SUBRESOURCE msr;
	DX11.GetDeviceContext()->Map(_combBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	XMMATRIX* pBuffer = (XMMATRIX*)msr.pData;
	memcpy(pBuffer, transposeAry, sizeof(XMMATRIX) * BONE_MAX);
	DX11.GetDeviceContext()->Unmap(_combBuffer, 0);

	//GetDeviceContext()->UpdateSubresource(combBuffer, 0, NULL, &combMatrixAry, 0, 0);
}


/**
* @brief メッシュコンポーネント追加
*/
void SkinnedMeshRenderer::AddSkinnedMesh(SkinnedMeshComponent * mesh)
{
	_skinnedMeshs.emplace_back(mesh);
}


/**
* @brief メッシュコンポーネント削除
*/
void SkinnedMeshRenderer::RemoveSkinnedMesh(SkinnedMeshComponent * mesh)
{
	auto itr = std::find(_skinnedMeshs.begin(), _skinnedMeshs.end(), mesh);
	if (itr != _skinnedMeshs.end())
	{
		_skinnedMeshs.erase(itr);
	}
}