#include "stdafx.h"

/**
* @file Actor.cpp
* @brief 全てのゲームオブジェクトの基底クラス
* @author GP12B183 12 小峰遥一
* @details 階層構造とコンポーネントベースをハイブリッドにしてる
*/


/**
* @brief コンストラクタ
*/
Actor::Actor()
{
	Game.GetActorManager()->Add(this);
}


/**
* @brief デストラクタ
*/
Actor::~Actor()
{
	Game.GetActorManager()->Remove(this);

	// コンポーネント全削除
	while (!_components.empty())
	{
		delete _components.back();
	}
}


/**
* @brief 入力処理（GameSystemから呼び出される、オーバーライド不可）
*/
void Actor::ProcessInput(const InputSystem & input)
{
	if (_state != STATE::ACTIVE) return;

	for (auto comp : _components)
	{
		comp->ProcessInput(input);
	}

	ActorInput(input);
}


/**
* @brief アクター独自の入力処理（オーバーライド可）
*/
void Actor::ActorInput(const InputSystem & input)
{
}


/**
* @brief 更新処理（GameSystemから呼び出される、オーバーライド不可）
*/
void Actor::Update(float deltaTime)
{
	if (_state != STATE::ACTIVE) return;

	CalcWorldMatrix();
	UpdateComponents(deltaTime);
	UpdateActor(deltaTime);
	CalcWorldMatrix();			// UpdateActor()でSRTが変化したときに備えてもう一度呼び出す
}


/**
* @brief アクターが持つコンポーネントの更新処理（オーバーライド不可）
*/
void Actor::UpdateComponents(float deltaTime)
{
	for (auto comp : _components)
	{
		comp->Update(deltaTime);
	}
}


/**
* @brief アクター独自の更新処理（オーバーライド可）
*/
void Actor::UpdateActor(float deltaTime)
{
}


/**
* @brief ワールドマトリクスの計算処理
*/
void Actor::CalcWorldMatrix(void)
{
	if (!_needRecalcMtxWorld) return;

	XMMATRIX mtxWorld = XMMatrixIdentity();

	// スケールを反映
	XMMATRIX mtxScl = XMMatrixScaling(_scl.x, _scl.y, _scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	//// 回転を反映
	//XMMATRIX mtxRot = XMMatrixRotationRollPitchYaw(_rot.x, _rot.y, _rot.z);
	//mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// クォータニオンを反映
	XMMATRIX quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&_quat));
	mtxWorld = XMMatrixMultiply(mtxWorld, quatMatrix);

	// 移動を反映
	XMMATRIX mtxTranslate = XMMatrixTranslation(_pos.x, _pos.y, _pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	XMStoreFloat4x4(&_mtxWorld, mtxWorld);

	// ワールドマトリクス更新をコンポーネントに通知する
	for (auto comp : _components)
	{
		comp->OnUpdateWorldMatrix();
	}

	// フラグを折る
	_needRecalcMtxWorld = false;
}


/**
* @brief 指定方向を向く
*/
void Actor::LookAt(XMFLOAT3 target, XMFLOAT3 up)
{
	XMMATRIX mtxLookAt = XMMatrixLookAtLH(ToXMVECTOR(GetPos()), ToXMVECTOR(target), ToXMVECTOR(up));
	XMVECTOR vec = XMQuaternionRotationMatrix(mtxLookAt);
	SetQuat(ToXMFLOAT4(vec));
}

/**
* @brief コンポーネントの追加処理
*/
void Actor::AddComponent(Component * component)
{
	// 更新順通りに挿入する

	int order = component->GetUpdateOrder();
	auto itr = _components.begin();

	while (itr != _components.end())
	{
		if (order < (*itr)->GetUpdateOrder()) break;
		itr++;
	}

	_components.insert(itr, component);
}


/**
* @brief コンポーネントの削除処理
*/
void Actor::RemoveComponent(Component * component)
{
	auto itr = std::find(_components.begin(), _components.end(), component);
	if (itr != _components.end())
	{
		_components.erase(itr);
	}
}
