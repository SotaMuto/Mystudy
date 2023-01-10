#pragma once

/**
* @file Renderer.h
* @brief �����_�����O����
* @author GP12B183 12 �����y��
* @details
*/

/**
* @brief �����_�����O�����N���X
* @todo 2D��3D�ł킯��H
*/
class Renderer
{
	// �萔�o�b�t�@
	ID3D11Buffer* _worldBuffer    = NULL;	// ���[���h�}�g���N�X
	ID3D11Buffer* _viewBuffer     = NULL;	// �r���[�}�g���N�X
	ID3D11Buffer* _projBuffer     = NULL;	// �v���W�F�N�V�����}�g���N�X
	ID3D11Buffer* _materialBuffer = NULL;	// �}�e���A��
	ID3D11Buffer* _cameraBuffer   = NULL;	// �J����
	ID3D11Buffer* _lightBuffer    = NULL;	// ���C�g

	XMFLOAT4X4 _mtxView;
	XMFLOAT4X4 _mtxProj;

	// ���̓��C�A�E�g
	ID3D11InputLayout*  _inputLayout = NULL;

	// �T���v��
	ID3D11SamplerState* _samplerState = NULL;

	TextureManager	_texMgr;
	MeshManager		_meshMgr;

	// ���_�V�F�[�_�[/�s�N�Z���V�F�[�_�[
	ID3D11VertexShader* _vsMesh = NULL;
	ID3D11PixelShader*  _psMesh = NULL;

	// ���b�V���R���|�[�l���g
	std::vector<MeshComponent*> _meshs;

	void Draw3D(void);

public:
	Renderer() {}
	~Renderer() {}

	void Init(void);
	void Uninit(void);
	void Draw(void);

	void CreateShader(void);
	void CreateConstantBuffer(void);

	void AddMesh(MeshComponent* mesh);
	void RemoveMesh(MeshComponent* mesh);

	void SetWorldMatrix(XMMATRIX *mtxWorld);
	XMFLOAT4X4* GetViewMatrix(void) { return &_mtxView; }
	void SetViewMatrix(XMMATRIX *mtxView);
	XMFLOAT4X4* GetProjMatrix(void) { return &_mtxProj; }
	void SetProjMatrix(XMMATRIX *mtxProj);
	void SetWorldViewProjection2D(void);
	void SetMaterialBuffer(Material *mat);
	void SetCameraBuffer(XMFLOAT3 cameraPos);

	ID3D11InputLayout* GetDefaultInputLayout(void) const { return _inputLayout; }
	ID3D11SamplerState* GetDefaultSamplerState(void) const { return _samplerState; }

	ID3D11Buffer** GetWorldBuffer(void) { return &_worldBuffer; }
	ID3D11Buffer** GetViewBuffer(void) { return &_viewBuffer; }
	ID3D11Buffer** GetProjBuffer(void) { return &_projBuffer; }
	ID3D11Buffer** GetMaterialBuffer(void) { return &_materialBuffer; }
	ID3D11Buffer** GetCameraBuffer(void) { return &_cameraBuffer; }
	ID3D11Buffer** GetLightBuffer(void) { return &_lightBuffer; }

	TextureManager* GetTextureManager(void) { return &_texMgr; }
	MeshManager* GetMeshManager(void) { return &_meshMgr; }
};
