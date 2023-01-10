#pragma once

/**
* @file MeshComponent.h
* @brief メッシュの描画
* @author GP12B183 12 小峰遥一
* @details
*/


/**
* メッシュの描画を行うコンポーネント
*/
class MeshComponent : public Component
{
private:
	Mesh* _pMesh = NULL;	// メッシュデータへのポインタ

	BOOL _enableDepthTest = TRUE;


public:
	MeshComponent(Actor* pOwner, Mesh* mesh);
	virtual ~MeshComponent();

	void SetDepthTest(BOOL flg){ _enableDepthTest = flg; }

	Mesh* GetMesh(void) { return _pMesh; }
	void SetMesh(Mesh* pMesh) { _pMesh = pMesh; }

	virtual void Draw(void);
};