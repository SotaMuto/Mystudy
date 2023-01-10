#pragma once

/**
* @file SkinnedMeshRenderer.h
* @brief スキンメッシュ用レンダリング処理
* @author GP12B183 12 小峰遥一
* @details
*/

/**
* @brief スキンメッシュ用レンダリング処理クラス
*/
class SkinnedMeshRenderer
{
private:
	// シェーダー
	ID3D11VertexShader* _vsSkining;
	ID3D11PixelShader*  _psSkining;

	// 定数バッファ
	ID3D11Buffer* _combBuffer;	// 合成変換行列バッファ

	// 入力レイアウト
	ID3D11InputLayout*  _inputLayout = NULL;

	// メッシュマネージャ
	SkinnedMeshManager _skinnedMeshMgr;

	// スキンメッシュコンポーネント
	std::vector<SkinnedMeshComponent*> _skinnedMeshs;

	void InitShader(void);
	void InitConstantBuffer(void);

public:
	SkinnedMeshRenderer() {}
	~SkinnedMeshRenderer() {}

	void Init(void);
	void Uninit(void);
	void Draw(void);

	void SetCombMatrix(XMFLOAT4X4* mtxCombAry);

	void AddSkinnedMesh(SkinnedMeshComponent* mesh);
	void RemoveSkinnedMesh(SkinnedMeshComponent* mesh);

	SkinnedMeshManager* GetSkinnedMeshManager(void) { return &_skinnedMeshMgr; }
};
