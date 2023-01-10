#pragma once

/**
* @file SkinnedMeshComponent.h
* @brief スキンメッシュの描画
* @author GP12B183 12 小峰遥一
* @details
*/


/**
* メッシュの描画を行うコンポーネント
*/
class SkinnedMeshComponent : public Component
{
private:
	SkinnedMesh*	_pMesh       = NULL;	// メッシュデータへのポインタ
	bool			_isPlaying   = true;	// アニメーション再生フラグ
	double			_animTimer   = 0.0f;	// アニメーションタイマー
	int				_animStackId = 0;		// アニメーションスタック

public:
	SkinnedMeshComponent(Actor* pOwner);
	virtual ~SkinnedMeshComponent();

	SkinnedMesh* GetMesh(void) { return _pMesh; }
	void SetMesh(SkinnedMesh* pMesh) { _pMesh = pMesh; }

	void SetAnimStack(int id) { _animStackId = id; }

	void PlayAnimation(void) { _isPlaying = true; }
	void StopAnimation(void) { _isPlaying = false; }

	void Update(float deltaTime) override;
	void Draw(void);
};