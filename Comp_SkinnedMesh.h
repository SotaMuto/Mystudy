#pragma once

/**
* @file SkinnedMeshComponent.h
* @brief �X�L�����b�V���̕`��
* @author GP12B183 12 �����y��
* @details
*/


/**
* ���b�V���̕`����s���R���|�[�l���g
*/
class SkinnedMeshComponent : public Component
{
private:
	SkinnedMesh*	_pMesh       = NULL;	// ���b�V���f�[�^�ւ̃|�C���^
	bool			_isPlaying   = true;	// �A�j���[�V�����Đ��t���O
	double			_animTimer   = 0.0f;	// �A�j���[�V�����^�C�}�[
	int				_animStackId = 0;		// �A�j���[�V�����X�^�b�N

public:
	SkinnedMeshComponent(Actor* pOwner);
	virtual ~SkinnedMeshComponent();

	SkinnedMesh* GetMesh(void) { return _pMesh; }
	void SetMesh(SkinnedMesh* pMesh) { _pMesh = pMesh; }

	void SetAnimStack(int id) { _animStackId = id; }

	void PlayAnimation(void) { _isPlaying = true; }
	void StopAnimation(void) { _isPlaying = false; }

	void Update(float deltaTime) override;
	void Draw(void);
};