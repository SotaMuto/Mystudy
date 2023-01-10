#include "stdafx.h"

/**
* @file ActorManager.cpp
* @brief �A�N�^�[�N���X�̕ێ��E�X�V
* @author GP12B183 12 �����y��
* @details
*/


/**
* @brief �R���X�g���N�^
*/
ActorManager::ActorManager()
{
	_actors.reserve(128);
}


/**
* @brief �f�X�g���N�^
*/
ActorManager::~ActorManager()
{
	RemoveAll();
}


/**
* @brief �A�N�^�[�̓��͏���
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
* @brief �X�V����
*/
void ActorManager::Update(float deltaTime)
{
	// �S�ẴA�N�e�B�u�A�N�^�[���X�V
	_isUpdatingActors = true;
	for (auto actor : _actors)
	{
		actor->Update(deltaTime);
	}
	_isUpdatingActors = false;

	// �ҋ@���̃A�N�^�[���A�N�e�B�u�ȃA�N�^�[�R���e�i�Ɉړ�
	for (auto pendingActor : _pendingActors)
	{
		pendingActor->CalcWorldMatrix();			// �쐬���Ƀ��[���h�}�g���N�X���v�Z����
		_actors.emplace_back(pendingActor);
	}
	_pendingActors.clear();

	// ���S��Ԃ̃A�N�^�[���ꎞ�ޔ�
	std::vector<Actor*> deadActors;
	for (auto actor : _actors)
	{
		if (actor->GetState() == Actor::DEAD)
		{
			deadActors.emplace_back(actor);
		}
	}

	// ���S�A�N�^�[��������Ă���
	for (auto deadActor : deadActors)
	{
		delete deadActor;
	}
}


/**
* @brief �A�N�^�[�̒ǉ�����
*/
void ActorManager::Add(Actor * actor)
{
	// �A�N�^�[�̍X�V���Ȃ�҂��ɒǉ�
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
* @brief �A�N�^�[�̍폜����
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
