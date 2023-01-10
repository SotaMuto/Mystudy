#include "stdafx.h"

/**
* @file Renderer.cpp
* @brief �����_�����O����
* @author GP12B183 12 �����y��
* @details
*/



/**
* @brief ����������
*/
void Renderer::Init(void)
{
	_texMgr = TextureManager();
	_meshMgr = MeshManager();

	CreateShader();
	CreateConstantBuffer();

	// �f�t�H���g�T���v��
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
* @brief �V�F�[�_�[����
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

	// ���_�V�F�[�_�[
	{
		hr = D3DX11CompileFromFile("Shader/Mesh.hlsl", NULL, NULL, "VS_Main", "vs_5_0", shFlag, 0, NULL, &pVSBlob, &pErrorBlob, NULL);
		if (FAILED(hr)) MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "VS", MB_OK | MB_ICONERROR);
		DX11.GetDevice()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &_vsMesh);
	}

	// ���̓��C�A�E�g����
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			// �Z�}���e�B�b�N��, �Z�}���e�B�b�N�ԍ�, �t�H�[�}�b�g, ���̓X���b�g�ԍ�, �I�t�Z�b�g, ���̓f�[�^�̎��, �C���X�^���X���Ƃ̌J��Ԃ� 
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

	// �s�N�Z���V�F�[�_�[
	ID3DBlob* pPSBlob = NULL;

	hr = D3DX11CompileFromFile("Shader/Mesh.hlsl", NULL, NULL, "PS_Main", "ps_5_0", shFlag, 0, NULL, &pPSBlob, &pErrorBlob, NULL);
	if (FAILED(hr)) MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "PS", MB_OK | MB_ICONERROR);
	DX11.GetDevice()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &_psMesh);

	pPSBlob->Release();
	if (pErrorBlob) pErrorBlob->Release();

}

/**
* @brief �萔�o�b�t�@����
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

	// ���[���h�}�g���N�X
	DX11.GetDevice()->CreateBuffer(&hBufferDesc, NULL, &_worldBuffer);

	// �r���[�}�g���N�X
	DX11.GetDevice()->CreateBuffer(&hBufferDesc, NULL, &_viewBuffer);

	// �v���W�F�N�V�����}�g���N�X
	DX11.GetDevice()->CreateBuffer(&hBufferDesc, NULL, &_projBuffer);

	// �}�e���A��
	hBufferDesc.ByteWidth = sizeof(Material);
	DX11.GetDevice()->CreateBuffer(&hBufferDesc, NULL, &_materialBuffer);

	// �J����
	hBufferDesc.ByteWidth = sizeof(XMFLOAT4);
	DX11.GetDevice()->CreateBuffer(&hBufferDesc, NULL, &_cameraBuffer);
}


/**
* @brief �I������
*/
void Renderer::Uninit(void)
{
	// �e�N�X�`�����\�[�X��S�J��
	_texMgr.ReleaseAll();

	// ���b�V�����\�[�X��S�J��
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
* @brief �`�揈��
*/
void Renderer::Draw(void)
{
	// 3D�V�[����`��
	Draw3D();

	// 2D�V�[����`��
	//Draw2D();
}


void Renderer::Draw3D(void)
{
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

	// ���_�V�F�[�_�[
	CameraActor* mainCamera = Game.GetMainCamera();
	SetViewMatrix(&XMLoadFloat4x4(mainCamera->GetViewMatrix()));
	SetProjMatrix(&XMLoadFloat4x4(mainCamera->GetProjMatrix()));
	DX11.GetDeviceContext()->VSSetShader(_vsMesh, nullptr, 0);
	DX11.GetDeviceContext()->VSSetConstantBuffers(0, 1, &_worldBuffer);
	DX11.GetDeviceContext()->VSSetConstantBuffers(1, 1, &_viewBuffer);
	DX11.GetDeviceContext()->VSSetConstantBuffers(2, 1, &_projBuffer);

	// �s�N�Z���V�F�[�_�[
	DX11.GetDeviceContext()->PSSetShader(_psMesh, nullptr, 0);
	DX11.GetDeviceContext()->PSSetConstantBuffers(3, 1, &_materialBuffer);

	// �T���v���Z�b�g
	DX11.GetDeviceContext()->PSSetSamplers(0, 1, &_samplerState);

	// ���b�V�������Ԃɕ`��
	for (auto mesh : _meshs)
	{
		mesh->Draw();
	}
}


/*
@brief ���b�V���ǉ�����
*/
void Renderer::AddMesh(MeshComponent * mesh)
{
	_meshs.emplace_back(mesh);
}


/*
@brief ���b�V���폜����
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
* @brief ���[���h�}�g���N�X�o�b�t�@���X�V
*/
void Renderer::SetWorldMatrix(XMMATRIX *mtxWorld)
{
	XMMATRIX mtx = XMMatrixTranspose(*mtxWorld);
	DX11.GetDeviceContext()->UpdateSubresource(_worldBuffer, 0, NULL, &mtx, 0, 0);
}

/**
* @brief �r���[�}�g���N�X�o�b�t�@���X�V
*/
void Renderer::SetViewMatrix(XMMATRIX *mtxView)
{
	XMMATRIX mtx = XMMatrixTranspose(*mtxView);
	DX11.GetDeviceContext()->UpdateSubresource(_viewBuffer, 0, NULL, &mtx, 0, 0);

	XMStoreFloat4x4(&_mtxView, mtx);
}

/**
* @brief �v���W�F�N�V�����}�g���N�X�o�b�t�@���X�V
*/
void Renderer::SetProjMatrix(XMMATRIX *mtxProj)
{
	XMMATRIX mtx = XMMatrixTranspose(*mtxProj);
	DX11.GetDeviceContext()->UpdateSubresource(_projBuffer, 0, NULL, &mtx, 0, 0);

	XMStoreFloat4x4(&_mtxProj, mtx);
}

/*
@brief 2D�`��p�̃}�g���N�X�o�b�t�@�ɍX�V
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
* @brief �}�e���A���o�b�t�@���X�V
*/
void Renderer::SetMaterialBuffer(Material * mat)
{
	DX11.GetDeviceContext()->UpdateSubresource(_materialBuffer, 0, NULL, mat, 0, 0);
}


/**
* @brief �J�����o�b�t�@���X�V
*/
void Renderer::SetCameraBuffer(XMFLOAT3 cameraPos)
{
	XMFLOAT4 tmp = XMFLOAT4(cameraPos.x, cameraPos.y, cameraPos.z, 0.0f);
	DX11.GetDeviceContext()->UpdateSubresource(_cameraBuffer, 0, NULL, &tmp, 0, 0);
}

