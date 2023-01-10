#include "stdafx.h"

/**
* @file SkinnedMeshComponent.h
* @brief スキンメッシュの描画
* @author GP12B183 12 小峰遥一
* @details
*/


/**
* @brief コンストラクタ
*/
SkinnedMeshComponent::SkinnedMeshComponent(Actor * pOwner)
	: Component(pOwner)
{
	Game.GetSkinnedMeshRenderer()->AddSkinnedMesh(this);
}

/**
* @brief デストラクタ
*/
SkinnedMeshComponent::~SkinnedMeshComponent()
{
	Game.GetSkinnedMeshRenderer()->RemoveSkinnedMesh(this);
}


/**
* @brief 更新処理
*/
void SkinnedMeshComponent::Update(float deltaTime)
{
	if (!_isPlaying) return;

	// 合成変換行列を更新
	XMMATRIX  mtxWorld = XMLoadFloat4x4(&GetOwner()->GetWorldMatrix());
	_pMesh->Update(&mtxWorld, _animStackId, _animTimer);

	// フレーム数更新
	_animTimer = _pMesh->LoopFrame(_animStackId, _animTimer, deltaTime);

	//Debug.Print("%f", _animTimer);
}

/**
* @brief 描画処理
*/
void SkinnedMeshComponent::Draw(void)
{
	if (_pMesh == NULL || GetOwner()->GetState() == Actor::DEAD) return;

	// 深度テスト
	DX11.SetDepthState(TRUE);

	// カリング
	DX11.SetRasterizerState(DX11.CULL_MODE_BACK);

	// メッシュ描画
	_pMesh->Draw();
}
