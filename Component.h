#pragma once

/**
* @file Component.h
* @brief コンポーネントの基底クラス
* @author GP12B183 12 小峰遥一
* @details
*/

// 前方宣言
class Actor;

/**
* @brief コンポーネントの基底クラス
*/
class Component
{
private:
	Actor* _pOwner;		// このコンポーネントの所有者アクター
	int  _updateOrder;	// コンポーネントの更新順序（小さいほど早く更新される）

	BOOL _inUse = TRUE;

public:
	Component(Actor* pOwner, int updateOrder = 100);
	virtual ~Component();

	Actor* GetOwner(void) { return _pOwner; }
	int GetUpdateOrder(void) const { return _updateOrder; }
	BOOL GetInUse(void) const { return _inUse; }
	void SetInUse(BOOL inUse) { _inUse = inUse; }

	//! 更新処理
	virtual void Update(float deltaTime);
	//! 入力処理
	virtual void ProcessInput(const InputSystem& input) {}
	//! アクターのワールドマトリクス更新時に呼び出される
	virtual void OnUpdateWorldMatrix(void) {}	
};