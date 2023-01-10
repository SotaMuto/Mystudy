#include "stdafx.h"

/**
* @file ActorManager.cpp
* @brief アクタークラスの保持・更新
* @author GP12B183 12 小峰遥一
* @details
*/


/**
* @brief コンストラクタ
*/
ActorManager::ActorManager()
{
	_actors.reserve(128);
}


/**
* @brief デストラクタ
*/
ActorManager::~ActorManager()
{
	RemoveAll();
}


/**
* @brief アクターの入力処理
*/
void ActorManager::ProcessInput(const InputSystem & input)
{
	for (auto actor : _actors)
	{
		if (actor->GetState() != Actor::ACTIVE) continue;

		actor->ProcessInput(input);
	}
}


/**
* @brief 更新処理
*/
void ActorManager::Update(float deltaTime)
{
	// 全てのアクティブアクターを更新
	_isUpdatingActors = true;
	for (auto actor : _actors)
	{
		actor->Update(deltaTime);
	}
	_isUpdatingActors = false;

	// 待機中のアクターをアクティブなアクターコンテナに移動
	for (auto pendingActor : _pendingActors)
	{
		pendingActor->CalcWorldMatrix();			// 作成時にワールドマトリクスを計算する
		_actors.emplace_back(pendingActor);
	}
	_pendingActors.clear();

	// 死亡状態のアクターを一時退避
	std::vector<Actor*> deadActors;
	for (auto actor : _actors)
	{
		if (actor->GetState() == Actor::DEAD)
		{
			deadActors.emplace_back(actor);
		}
	}

	// 死亡アクターを解放していく
	for (auto deadActor : deadActors)
	{
		delete deadActor;
	}
}


/**
* @brief アクターの追加処理
*/
void ActorManager::Add(Actor * actor)
{
	// アクターの更新中なら待ちに追加
	if (_isUpdatingActors)
	{
		_pendingActors.emplace_back(actor);
	}
	else
	{
		_actors.emplace_back(actor);
	}
}


/**
* @brief アクターの削除処理
*/
void ActorManager::Remove(Actor * actor)
{
	{
		auto itr = std::find(_pendingActors.begin(), _pendingActors.end(), actor);
		if (itr != _pendingActors.end())
		{
			std::iter_swap(itr, _pendingActors.end() - 1);
			_pendingActors.pop_back();
		}
	}

	{
		auto itr = std::find(_actors.begin(), _actors.end(), actor);
		if (itr != _actors.end())
		{
			std::iter_swap(itr, _actors.end() - 1);
			_actors.pop_back();
		}
	}
}

void ActorManager::RemoveAll(void)
{
	while (!_actors.empty())
	{
		delete _actors.back();
	}
}
