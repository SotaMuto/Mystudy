#pragma once

/**
* @file SkinnedMeshRenderer.h
* @brief �X�L�����b�V���p�����_�����O����
* @author GP12B183 12 �����y��
* @details
*/

/**
* @brief �X�L�����b�V���p�����_�����O�����N���X
*/
class SkinnedMeshRenderer
{
private:
	// �V�F�[�_�[
	ID3D11VertexShader* _vsSkining;
	ID3D11PixelShader*  _psSkining;

	// �萔�o�b�t�@
	ID3D11Buffer* _combBuffer;	// �����ϊ��s��o�b�t�@

	// ���̓��C�A�E�g
	ID3D11InputLayout*  _inputLayout = NULL;

	// ���b�V���}�l�[�W��
	SkinnedMeshManager _skinnedMeshMgr;

	// �X�L�����b�V���R���|�[�l���g
	std::vector<SkinnedMeshComponent*> _skinnedMeshs;

	void InitShader(void);
	void InitConstantBuffer(void);

public:
	SkinnedMeshRenderer() {}
	~SkinnedMeshRenderer() {}

	void Init(void);
	void Uninit(void);
	void Draw(void);

	void SetCombMatrix(XMFLOAT4X4* mtxCombAry);

	void AddSkinnedMesh(SkinnedMeshComponent* mesh);
	void RemoveSkinnedMesh(SkinnedMeshComponent* mesh);

	SkinnedMeshManager* GetSkinnedMeshManager(void) { return &_skinnedMeshMgr; }
};
