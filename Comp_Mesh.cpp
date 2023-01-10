#include "stdafx.h"

/**
* @file MeshComponent.cpp
* @brief メッシュの描画
* @author GP12B183 12 小峰遥一
* @details
*/


/**
* @brief コンストラクタ
* @param pOwner 所有アクターのポインタ
*/
MeshComponent::MeshComponent(Actor * pOwner, Mesh * mesh) : 
	Component(pOwner),
	_pMesh(mesh)
{
	Game.GetRenderer()->AddMesh(this);
}

/**
* @brief デストラクタ
*/
MeshComponent::~MeshComponent()
{
	Game.GetRenderer()->RemoveMesh(this);
}


/**
* @brief 描画処理
*/
void MeshComponent::Draw(void)
{
	if (_pMesh == NULL || Component::GetInUse() == FALSE) return;

	// 深度テスト
	DX11.SetDepthState(_enableDepthTest);

	// カリング
	DX11.SetRasterizerState(DX11.CULL_MODE_BACK);

	// ワールドマトリクスをセット
	XMMATRIX mtxWorld = XMLoadFloat4x4(&GetOwner()->GetWorldMatrix());
	Game.GetRenderer()->SetWorldMatrix(&mtxWorld);

	// メッシュ描画
	_pMesh->Draw();
}
