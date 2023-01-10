#pragma once

/**
* @file Actor.h
* @brief �S�ẴQ�[���I�u�W�F�N�g�̊��N���X
* @author GP12B183 12 �����y��
* @details �K�w�\���ƃR���|�[�l���g�x�[�X���n�C�u���b�h�ɂ��Ă�
*/


/**
* @brief �A�N�^�[�N���X
*/
class Actor
{
public:
	// �A�N�^�[�̏��
	enum STATE
	{
		ACTIVE,	// �A�N�e�B�u	
		PAUSE,	// delete�͂���Ȃ����X�V����Ȃ�
		DEAD,	// ���̃t���[����delete�����
	};

private:
	Actor::STATE	_state = ACTIVE;						// ���݂̏��
	XMFLOAT3		_pos   = { 0.0f, 0.0f, 0.0f };			// ���W
	XMFLOAT3		_scl   = { 1.0f, 1.0f, 1.0f };			// �g�k
	XMFLOAT4		_quat  = { 0.0f, 0.0f, 0.0f, 1.0f };	// �N�I�[�^�j�I��
	XMFLOAT4X4		_mtxWorld;								// ���[���h�}�g���N�X
	XMFLOAT4X4		_mtxWorldParent;						// �e�̃��[���h�}�g���N�X
	bool			_hasParent = false;
	bool			_needRecalcMtxWorld = true;				// ���[���h�}�g���N�X���Čv�Z����t���O
	std::vector<Component*> _components;					// ���L���Ă���R���|�[�l���g

public:
	Actor();
	virtual ~Actor();

	// ����
	void ProcessInput(const InputSystem& input);
	virtual void ActorInput(const InputSystem& input);

	// �X�V
	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void UpdateActor(float deltaTime);

	//! �ړ�
	void Translate(XMFLOAT3 translate) { SetPos(GetPos() + translate); }
	void Translate(float x, float y, float z) { Translate(XMFLOAT3(x, y, z)); }

	//! ��]
	void Rotate(XMFLOAT3 angle) { SetRot(GetRot() + angle); }
	void Rotate(float x, float y, float z) { Rotate(XMFLOAT3(x, y, z)); }

	//! �g�k
	void Scale(XMFLOAT3 scl) { SetScl(GetScl() + scl); }
	void Scale(float x, float y, float z) { Scale(XMFLOAT3(x, y, z)); }

	//! ���[���h�}�g���N�X�v�Z
	void CalcWorldMatrix(void);

#pragma region Getter/Setter
	Actor::STATE GetState(void) const { return _state; }
	void SetState(Actor::STATE state) { _state = state; }

	XMFLOAT3 GetPos(void) const { return _pos; }
	void SetPos(XMFLOAT3 pos) { _pos = pos; _needRecalcMtxWorld = true; }
	void SetPos(float x, float y, float z) { SetPos(XMFLOAT3(x, y, z)); }

	XMFLOAT3 GetScl(void) const { return _scl; }
	void SetScl(XMFLOAT3 scl) { _scl = scl; _needRecalcMtxWorld = true; }
	void SetScl(float x, float y, float z) { SetScl(XMFLOAT3(x, y, z)); }

	XMFLOAT3 GetRot(void) const { return QuaternionToRotation(_quat); }
	void SetRot(XMFLOAT3 rot) { _quat = RotationToQuaternion(rot); _needRecalcMtxWorld = true; }
	void SetRot(float x, float y, float z) { SetRot(XMFLOAT3(x, y, z)); }

	XMFLOAT4 GetQuat(void) const { return _quat; }
	void SetQuat(XMFLOAT4 quat) { _quat = quat; _needRecalcMtxWorld = true; }
	void SetQuat(float x, float y, float z, float w) { SetQuat(XMFLOAT4(x, y, z, w)); }

	void SetParentWorldMatrix(XMFLOAT4X4 mtxParent) { _mtxWorldParent = mtxParent; _hasParent = true; }

	//! �O�x�N�g��
	XMFLOAT3 GetForward(void) const { return VectorRotation(XMFLOAT3(0.0f, 0.0f, 1.0f), GetQuat()); }
	//! �E�x�N�g��
	XMFLOAT3 GetRight(void) const { return XMFLOAT3Cross(GetForward(), XMFLOAT3(0.0f, 1.0f, 0.0f)); }
	//! �w����W������
	void LookAt(XMFLOAT3 target, XMFLOAT3 up = { 0.0f, 1.0f, 0.0f });

	//! ���[���h�}�g���N�X
	XMFLOAT4X4 GetWorldMatrix(void) const 
	{ 
		XMFLOAT4X4 mtxWorld = _mtxWorld;

		if (_hasParent)
		{
			XMStoreFloat4x4(&mtxWorld, 
				XMLoadFloat4x4(&_mtxWorld) * XMLoadFloat4x4(&_mtxWorldParent));
		}

		return mtxWorld;
	}
#pragma endregion

	//! �R���|�[�l���g�ǉ�/�폜
	void AddComponent(Component* component);
	void RemoveComponent(Component* component);
};