#include "stdafx.h"

/**
* @file MeshComponent.cpp
* @brief ���b�V���̕`��
* @author GP12B183 12 �����y��
* @details
*/


/**
* @brief �R���X�g���N�^
* @param pOwner ���L�A�N�^�[�̃|�C���^
*/
MeshComponent::MeshComponent(Actor * pOwner, Mesh * mesh) : 
	Component(pOwner),
	_pMesh(mesh)
{
	Game.GetRenderer()->AddMesh(this);
}

/**
* @brief �f�X�g���N�^
*/
MeshComponent::~MeshComponent()
{
	Game.GetRenderer()->RemoveMesh(this);
}


/**
* @brief �`�揈��
*/
void MeshComponent::Draw(void)
{
	if (_pMesh == NULL || Component::GetInUse() == FALSE) return;

	// �[�x�e�X�g
	DX11.SetDepthState(_enableDepthTest);

	// �J�����O
	DX11.SetRasterizerState(DX11.CULL_MODE_BACK);

	// ���[���h�}�g���N�X���Z�b�g
	XMMATRIX mtxWorld = XMLoadFloat4x4(&GetOwner()->GetWorldMatrix());
	Game.GetRenderer()->SetWorldMatrix(&mtxWorld);

	// ���b�V���`��
	_pMesh->Draw();
}
