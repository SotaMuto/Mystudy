#include "stdafx.h"

/**
* @file Component.cpp
* @brief コンポーネントの基底クラス
* @author GP12B183 12 小峰遥一
* @details
*/


/**
* @brief コンストラクタ
* @param pOwner 所有アクターのポインタ
* @param updateOrder 更新順序（小さいほど早く更新）
*/
Component::Component(Actor * pOwner, int updateOrder)
	: _pOwner(pOwner), _updateOrder(updateOrder)
{
	_pOwner->AddComponent(this);
}


/**
* @brief デストラクタ
*/
Component::~Component()
{
	_pOwner->RemoveComponent(this);
}


/**
* @brief 更新処理
*/
void Component::Update(float deltaTime)
{
}
