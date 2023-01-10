#include "stdafx.h"

/**
* @file SkinnedMeshComponent.h
* @brief �X�L�����b�V���̕`��
* @author GP12B183 12 �����y��
* @details
*/


/**
* @brief �R���X�g���N�^
*/
SkinnedMeshComponent::SkinnedMeshComponent(Actor * pOwner)
	: Component(pOwner)
{
	Game.GetSkinnedMeshRenderer()->AddSkinnedMesh(this);
}

/**
* @brief �f�X�g���N�^
*/
SkinnedMeshComponent::~SkinnedMeshComponent()
{
	Game.GetSkinnedMeshRenderer()->RemoveSkinnedMesh(this);
}


/**
* @brief �X�V����
*/
void SkinnedMeshComponent::Update(float deltaTime)
{
	if (!_isPlaying) return;

	// �����ϊ��s����X�V
	XMMATRIX  mtxWorld = XMLoadFloat4x4(&GetOwner()->GetWorldMatrix());
	_pMesh->Update(&mtxWorld, _animStackId, _animTimer);

	// �t���[�����X�V
	_animTimer = _pMesh->LoopFrame(_animStackId, _animTimer, deltaTime);

	//Debug.Print("%f", _animTimer);
}

/**
* @brief �`�揈��
*/
void SkinnedMeshComponent::Draw(void)
{
	if (_pMesh == NULL || GetOwner()->GetState() == Actor::DEAD) return;

	// �[�x�e�X�g
	DX11.SetDepthState(TRUE);

	// �J�����O
	DX11.SetRasterizerState(DX11.CULL_MODE_BACK);

	// ���b�V���`��
	_pMesh->Draw();
}
