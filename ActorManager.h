#pragma once

/**
* @file ActorManager.h
* @brief アクタークラスの保持・更新
* @author GP12B183 12 小峰遥一
* @details
*/

/**
* アクターマネージャ
*/
class ActorManager
{
private:
	//! _actorsの更新ループ中に新規アクターを追加させないためのフラグ
	bool _isUpdatingActors = false;	

	std::vector<Actor*> _actors;			// アクティブアクター
	std::vector<Actor*> _pendingActors;		// 待ちアクター

public:
	ActorManager();
	~ActorManager();

	void ProcessInput(const InputSystem& input);
	void Update(float deltaTime);

	void Add(Actor* actor);
	void Remove(Actor* actor); 
	void RemoveAll(void);
};