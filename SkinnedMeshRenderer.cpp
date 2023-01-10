/**
* @file SkinnedMeshRenderer.h
* @brief �X�L�����b�V���p�����_�����O����
* @author GP12B183 12 �����y��
* @details
*/

#include "stdafx.h"


/**
* @brief ����������
*/
void SkinnedMeshRenderer::Init(void)
{
	_skinnedMeshMgr = SkinnedMeshManager();

	InitShader();
	InitConstantBuffer();
}


/**
* @brief �V�F�[�_�[������
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

	// ���_�V�F�[�_�[
	{
		hr = D3DX11CompileFromFile("Shader/Skinning.hlsl", NULL, NULL, "VS_Main", "vs_5_0", shFlag, 0, NULL, &pVSBlob, &pErrorBlob, NULL);
		if (FAILED(hr)) MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "VS", MB_OK | MB_ICONERROR);
		DX11.GetDevice()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &_vsSkining);
	}

	// ���̓��C�A�E�g����
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

	// �s�N�Z���V�F�[�_�[
	ID3DBlob* pPSBlob = NULL;

	hr = D3DX11CompileFromFile("Shader/Skinning.hlsl", NULL, NULL, "PS_Main", "ps_5_0", shFlag, 0, NULL, &pPSBlob, &pErrorBlob, NULL);
	if (FAILED(hr)) MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "PS", MB_OK | MB_ICONERROR);
	DX11.GetDevice()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &_psSkining);

	pPSBlob->Release();
	if (pErrorBlob) pErrorBlob->Release();
}


/**
* @brief �萔�o�b�t�@����������
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
* @brief �I������
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
* @brief �`�揈��
*/
void SkinnedMeshRenderer::Draw(void)
{
	Renderer* pRenderer = Game.GetRenderer();

	// �u�����h���[�h
	DX11.SetBlendState(DX11.BLEND_MODE_ALPHA);

	// ���e�X�g
	DX11.SetAlphaTest(FALSE);

	// �����_�[�^�[�Q�b�g�r���[
	DX11.SetRenderTarget();

	// �r���[�|�[�g
	DX11.SetViewPort();

	// ���̓��C�A�E�g
	DX11.GetDeviceContext()->IASetInputLayout(_inputLayout);

	// �J�������_�ɕύX
	CameraActor* mainCamera = Game.GetMainCamera();
	pRenderer->SetViewMatrix(&XMLoadFloat4x4(mainCamera->GetViewMatrix()));
	pRenderer->SetProjMatrix(&XMLoadFloat4x4(mainCamera->GetProjMatrix()));

	// ���_�V�F�[�_�[
	DX11.GetDeviceContext()->VSSetShader(_vsSkining, nullptr, 0);
	DX11.GetDeviceContext()->VSSetConstantBuffers(0, 1, &_combBuffer);
	DX11.GetDeviceContext()->VSSetConstantBuffers(1, 1, pRenderer->GetViewBuffer());
	DX11.GetDeviceContext()->VSSetConstantBuffers(2, 1, pRenderer->GetProjBuffer());

	// �s�N�Z���V�F�[�_�[
	DX11.GetDeviceContext()->PSSetShader(_psSkining, nullptr, 0);
	DX11.GetDeviceContext()->PSSetConstantBuffers(3, 1, pRenderer->GetMaterialBuffer());

	// �T���v���Z�b�g
	ID3D11SamplerState* sampler = pRenderer->GetDefaultSamplerState();
	DX11.GetDeviceContext()->PSSetSamplers(0, 1, &sampler);

	// ���b�V�������Ԃɕ`��
	for (auto mesh : _skinnedMeshs)
	{
		mesh->Draw();
	}

}


/**
* @brief �����ϊ��s��o�b�t�@�̃Z�b�g
*/
void SkinnedMeshRenderer::SetCombMatrix(XMFLOAT4X4 * mtxCombAry)
{
	// �]�u����
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
* @brief ���b�V���R���|�[�l���g�ǉ�
*/
void SkinnedMeshRenderer::AddSkinnedMesh(SkinnedMeshComponent * mesh)
{
	_skinnedMeshs.emplace_back(mesh);
}


/**
* @brief ���b�V���R���|�[�l���g�폜
*/
void SkinnedMeshRenderer::RemoveSkinnedMesh(SkinnedMeshComponent * mesh)
{
	auto itr = std::find(_skinnedMeshs.begin(), _skinnedMeshs.end(), mesh);
	if (itr != _skinnedMeshs.end())
	{
		_skinnedMeshs.erase(itr);
	}
}