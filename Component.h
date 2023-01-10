#pragma once

/**
* @file Component.h
* @brief �R���|�[�l���g�̊��N���X
* @author GP12B183 12 �����y��
* @details
*/

// �O���錾
class Actor;

/**
* @brief �R���|�[�l���g�̊��N���X
*/
class Component
{
private:
	Actor* _pOwner;		// ���̃R���|�[�l���g�̏��L�҃A�N�^�[
	int  _updateOrder;	// �R���|�[�l���g�̍X�V�����i�������قǑ����X�V�����j

	BOOL _inUse = TRUE;

public:
	Component(Actor* pOwner, int updateOrder = 100);
	virtual ~Component();

	Actor* GetOwner(void) { return _pOwner; }
	int GetUpdateOrder(void) const { return _updateOrder; }
	BOOL GetInUse(void) const { return _inUse; }
	void SetInUse(BOOL inUse) { _inUse = inUse; }

	//! �X�V����
	virtual void Update(float deltaTime);
	//! ���͏���
	virtual void ProcessInput(const InputSystem& input) {}
	//! �A�N�^�[�̃��[���h�}�g���N�X�X�V���ɌĂяo�����
	virtual void OnUpdateWorldMatrix(void) {}	
};