#pragma once

/**
* @file Renderer.h
* @brief レンダリング処理
* @author GP12B183 12 小峰遥一
* @details
*/

/**
* @brief レンダリング処理クラス
* @todo 2Dと3Dでわける？
*/
class Renderer
{
	// 定数バッファ
	ID3D11Buffer* _worldBuffer    = NULL;	// ワールドマトリクス
	ID3D11Buffer* _viewBuffer     = NULL;	// ビューマトリクス
	ID3D11Buffer* _projBuffer     = NULL;	// プロジェクションマトリクス
	ID3D11Buffer* _materialBuffer = NULL;	// マテリアル
	ID3D11Buffer* _cameraBuffer   = NULL;	// カメラ
	ID3D11Buffer* _lightBuffer    = NULL;	// ライト

	XMFLOAT4X4 _mtxView;
	XMFLOAT4X4 _mtxProj;

	// 入力レイアウト
	ID3D11InputLayout*  _inputLayout = NULL;

	// サンプラ
	ID3D11SamplerState* _samplerState = NULL;

	TextureManager	_texMgr;
	MeshManager		_meshMgr;

	// 頂点シェーダー/ピクセルシェーダー
	ID3D11VertexShader* _vsMesh = NULL;
	ID3D11PixelShader*  _psMesh = NULL;

	// メッシュコンポーネント
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
