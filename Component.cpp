#include "stdafx.h"

/**
* @file Component.cpp
* @brief �R���|�[�l���g�̊��N���X
* @author GP12B183 12 �����y��
* @details
*/


/**
* @brief �R���X�g���N�^
* @param pOwner ���L�A�N�^�[�̃|�C���^
* @param updateOrder �X�V�����i�������قǑ����X�V�j
*/
Component::Component(Actor * pOwner, int updateOrder)
	: _pOwner(pOwner), _updateOrder(updateOrder)
{
	_pOwner->AddComponent(this);
}


/**
* @brief �f�X�g���N�^
*/
Component::~Component()
{
	_pOwner->RemoveComponent(this);
}


/**
* @brief �X�V����
*/
void Component::Update(float deltaTime)
{
}
