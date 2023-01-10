#include "stdafx.h"

/**
* @file Renderer.cpp
* @brief レンダリング処理
* @author GP12B183 12 小峰遥一
* @details
*/



/**
* @brief 初期化処理
*/
void Renderer::Init(void)
{
	_texMgr = TextureManager();
	_meshMgr = MeshManager();

	CreateShader();
	CreateConstantBuffer();

	// デフォルトサンプラ
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter   = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	DX11.GetDevice()->CreateSamplerState(&samplerDesc, &_samplerState);
}


/**
* @brief シェーダー生成
*/
void Renderer::CreateShader(void)
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
		hr = D3DX11CompileFromFile("Shader/Mesh.hlsl", NULL, NULL, "VS_Main", "vs_5_0", shFlag, 0, NULL, &pVSBlob, &pErrorBlob, NULL);
		if (FAILED(hr)) MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "VS", MB_OK | MB_ICONERROR);
		DX11.GetDevice()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &_vsMesh);
	}

	// 入力レイアウト生成
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			// セマンティック名, セマンティック番号, フォーマット, 入力スロット番号, オフセット, 入力データの種類, インスタンスごとの繰り返し 
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		UINT numElements = ARRAYSIZE(layout);

		hr = DX11.GetDevice()->CreateInputLayout(layout, numElements, 
			pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &_inputLayout);
	}
	
	pVSBlob->Release();
	if (pErrorBlob) pErrorBlob->Release();

	// ピクセルシェーダー
	ID3DBlob* pPSBlob = NULL;

	hr = D3DX11CompileFromFile("Shader/Mesh.hlsl", NULL, NULL, "PS_Main", "ps_5_0", shFlag, 0, NULL, &pPSBlob, &pErrorBlob, NULL);
	if (FAILED(hr)) MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "PS", MB_OK | MB_ICONERROR);
	DX11.GetDevice()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &_psMesh);

	pPSBlob->Release();
	if (pErrorBlob) pErrorBlob->Release();

}

/**
* @brief 定数バッファ生成
*/
void Renderer::CreateConstantBuffer(void)
{
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(XMMATRIX);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	// ワールドマトリクス
	DX11.GetDevice()->CreateBuffer(&hBufferDesc, NULL, &_worldBuffer);

	// ビューマトリクス
	DX11.GetDevice()->CreateBuffer(&hBufferDesc, NULL, &_viewBuffer);

	// プロジェクションマトリクス
	DX11.GetDevice()->CreateBuffer(&hBufferDesc, NULL, &_projBuffer);

	// マテリアル
	hBufferDesc.ByteWidth = sizeof(Material);
	DX11.GetDevice()->CreateBuffer(&hBufferDesc, NULL, &_materialBuffer);

	// カメラ
	hBufferDesc.ByteWidth = sizeof(XMFLOAT4);
	DX11.GetDevice()->CreateBuffer(&hBufferDesc, NULL, &_cameraBuffer);
}


/**
* @brief 終了処理
*/
void Renderer::Uninit(void)
{
	// テクスチャリソースを全開放
	_texMgr.ReleaseAll();

	// メッシュリソースを全開放
	_meshMgr.ReleaseAll();

	SAFE_RELEASE(_worldBuffer);
	SAFE_RELEASE(_viewBuffer);
	SAFE_RELEASE(_projBuffer);
	SAFE_RELEASE(_materialBuffer);
	SAFE_RELEASE(_cameraBuffer);
	SAFE_RELEASE(_lightBuffer);

	SAFE_RELEASE(_inputLayout);
	SAFE_RELEASE(_samplerState);
	SAFE_RELEASE(_vsMesh);
	SAFE_RELEASE(_psMesh);
}


/**
* @brief 描画処理
*/
void Renderer::Draw(void)
{
	// 3Dシーンを描画
	Draw3D();

	// 2Dシーンを描画
	//Draw2D();
}


void Renderer::Draw3D(void)
{
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

	// 頂点シェーダー
	CameraActor* mainCamera = Game.GetMainCamera();
	SetViewMatrix(&XMLoadFloat4x4(mainCamera->GetViewMatrix()));
	SetProjMatrix(&XMLoadFloat4x4(mainCamera->GetProjMatrix()));
	DX11.GetDeviceContext()->VSSetShader(_vsMesh, nullptr, 0);
	DX11.GetDeviceContext()->VSSetConstantBuffers(0, 1, &_worldBuffer);
	DX11.GetDeviceContext()->VSSetConstantBuffers(1, 1, &_viewBuffer);
	DX11.GetDeviceContext()->VSSetConstantBuffers(2, 1, &_projBuffer);

	// ピクセルシェーダー
	DX11.GetDeviceContext()->PSSetShader(_psMesh, nullptr, 0);
	DX11.GetDeviceContext()->PSSetConstantBuffers(3, 1, &_materialBuffer);

	// サンプラセット
	DX11.GetDeviceContext()->PSSetSamplers(0, 1, &_samplerState);

	// メッシュを順番に描画
	for (auto mesh : _meshs)
	{
		mesh->Draw();
	}
}


/*
@brief メッシュ追加処理
*/
void Renderer::AddMesh(MeshComponent * mesh)
{
	_meshs.emplace_back(mesh);
}


/*
@brief メッシュ削除処理
*/
void Renderer::RemoveMesh(MeshComponent * mesh)
{
	auto itr = std::find(_meshs.begin(), _meshs.end(), mesh);
	if (itr != _meshs.end())
	{
		_meshs.erase(itr);
	}
}


/**
* @brief ワールドマトリクスバッファを更新
*/
void Renderer::SetWorldMatrix(XMMATRIX *mtxWorld)
{
	XMMATRIX mtx = XMMatrixTranspose(*mtxWorld);
	DX11.GetDeviceContext()->UpdateSubresource(_worldBuffer, 0, NULL, &mtx, 0, 0);
}

/**
* @brief ビューマトリクスバッファを更新
*/
void Renderer::SetViewMatrix(XMMATRIX *mtxView)
{
	XMMATRIX mtx = XMMatrixTranspose(*mtxView);
	DX11.GetDeviceContext()->UpdateSubresource(_viewBuffer, 0, NULL, &mtx, 0, 0);

	XMStoreFloat4x4(&_mtxView, mtx);
}

/**
* @brief プロジェクションマトリクスバッファを更新
*/
void Renderer::SetProjMatrix(XMMATRIX *mtxProj)
{
	XMMATRIX mtx = XMMatrixTranspose(*mtxProj);
	DX11.GetDeviceContext()->UpdateSubresource(_projBuffer, 0, NULL, &mtx, 0, 0);

	XMStoreFloat4x4(&_mtxProj, mtx);
}

/*
@brief 2D描画用のマトリクスバッファに更新
*/
void Renderer::SetWorldViewProjection2D(void)
{
	XMMATRIX mtxWorld = XMMatrixTranspose(XMMatrixIdentity());
	DX11.GetDeviceContext()->UpdateSubresource(_worldBuffer, 0, NULL, &mtxWorld, 0, 0);

	XMMATRIX mtxView = XMMatrixTranspose(XMMatrixIdentity());
	DX11.GetDeviceContext()->UpdateSubresource(_viewBuffer, 0, NULL, &mtxView, 0, 0);

	XMMATRIX mtxProj;
	mtxProj = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	mtxProj = XMMatrixTranspose(mtxProj);
	DX11.GetDeviceContext()->UpdateSubresource(_projBuffer, 0, NULL, &mtxProj, 0, 0);
}

/**
* @brief マテリアルバッファを更新
*/
void Renderer::SetMaterialBuffer(Material * mat)
{
	DX11.GetDeviceContext()->UpdateSubresource(_materialBuffer, 0, NULL, mat, 0, 0);
}


/**
* @brief カメラバッファを更新
*/
void Renderer::SetCameraBuffer(XMFLOAT3 cameraPos)
{
	XMFLOAT4 tmp = XMFLOAT4(cameraPos.x, cameraPos.y, cameraPos.z, 0.0f);
	DX11.GetDeviceContext()->UpdateSubresource(_cameraBuffer, 0, NULL, &tmp, 0, 0);
}

