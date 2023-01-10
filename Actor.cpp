#include "stdafx.h"

/**
* @file Actor.cpp
* @brief �S�ẴQ�[���I�u�W�F�N�g�̊��N���X
* @author GP12B183 12 �����y��
* @details �K�w�\���ƃR���|�[�l���g�x�[�X���n�C�u���b�h�ɂ��Ă�
*/


/**
* @brief �R���X�g���N�^
*/
Actor::Actor()
{
	Game.GetActorManager()->Add(this);
}


/**
* @brief �f�X�g���N�^
*/
Actor::~Actor()
{
	Game.GetActorManager()->Remove(this);

	// �R���|�[�l���g�S�폜
	while (!_components.empty())
	{
		delete _components.back();
	}
}


/**
* @brief ���͏����iGameSystem����Ăяo�����A�I�[�o�[���C�h�s�j
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
* @brief �A�N�^�[�Ǝ��̓��͏����i�I�[�o�[���C�h�j
*/
void Actor::ActorInput(const InputSystem & input)
{
}


/**
* @brief �X�V�����iGameSystem����Ăяo�����A�I�[�o�[���C�h�s�j
*/
void Actor::Update(float deltaTime)
{
	if (_state != STATE::ACTIVE) return;

	CalcWorldMatrix();
	UpdateComponents(deltaTime);
	UpdateActor(deltaTime);
	CalcWorldMatrix();			// UpdateActor()��SRT���ω������Ƃ��ɔ����Ă�����x�Ăяo��
}


/**
* @brief �A�N�^�[�����R���|�[�l���g�̍X�V�����i�I�[�o�[���C�h�s�j
*/
void Actor::UpdateComponents(float deltaTime)
{
	for (auto comp : _components)
	{
		comp->Update(deltaTime);
	}
}


/**
* @brief �A�N�^�[�Ǝ��̍X�V�����i�I�[�o�[���C�h�j
*/
void Actor::UpdateActor(float deltaTime)
{
}


/**
* @brief ���[���h�}�g���N�X�̌v�Z����
*/
void Actor::CalcWorldMatrix(void)
{
	if (!_needRecalcMtxWorld) return;

	XMMATRIX mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	XMMATRIX mtxScl = XMMatrixScaling(_scl.x, _scl.y, _scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	//// ��]�𔽉f
	//XMMATRIX mtxRot = XMMatrixRotationRollPitchYaw(_rot.x, _rot.y, _rot.z);
	//mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �N�H�[�^�j�I���𔽉f
	XMMATRIX quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&_quat));
	mtxWorld = XMMatrixMultiply(mtxWorld, quatMatrix);

	// �ړ��𔽉f
	XMMATRIX mtxTranslate = XMMatrixTranslation(_pos.x, _pos.y, _pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	XMStoreFloat4x4(&_mtxWorld, mtxWorld);

	// ���[���h�}�g���N�X�X�V���R���|�[�l���g�ɒʒm����
	for (auto comp : _components)
	{
		comp->OnUpdateWorldMatrix();
	}

	// �t���O��܂�
	_needRecalcMtxWorld = false;
}


/**
* @brief �w�����������
*/
void Actor::LookAt(XMFLOAT3 target, XMFLOAT3 up)
{
	XMMATRIX mtxLookAt = XMMatrixLookAtLH(ToXMVECTOR(GetPos()), ToXMVECTOR(target), ToXMVECTOR(up));
	XMVECTOR vec = XMQuaternionRotationMatrix(mtxLookAt);
	SetQuat(ToXMFLOAT4(vec));
}

/**
* @brief �R���|�[�l���g�̒ǉ�����
*/
void Actor::AddComponent(Component * component)
{
	// �X�V���ʂ�ɑ}������

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
* @brief �R���|�[�l���g�̍폜����
*/
void Actor::RemoveComponent(Component * component)
{
	auto itr = std::find(_components.begin(), _components.end(), component);
	if (itr != _components.end())
	{
		_components.erase(itr);
	}
}
