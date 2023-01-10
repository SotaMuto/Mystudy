#pragma once

/**
* @file ActorManager.h
* @brief �A�N�^�[�N���X�̕ێ��E�X�V
* @author GP12B183 12 �����y��
* @details
*/

/**
* �A�N�^�[�}�l�[�W��
*/
class ActorManager
{
private:
	//! _actors�̍X�V���[�v���ɐV�K�A�N�^�[��ǉ������Ȃ����߂̃t���O
	bool _isUpdatingActors = false;	

	std::vector<Actor*> _actors;			// �A�N�e�B�u�A�N�^�[
	std::vector<Actor*> _pendingActors;		// �҂��A�N�^�[

public:
	ActorManager();
	~ActorManager();

	void ProcessInput(const InputSystem& input);
	void Update(float deltaTime);

	void Add(Actor* actor);
	void Remove(Actor* actor); 
	void RemoveAll(void);
};