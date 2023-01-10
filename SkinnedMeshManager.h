#pragma once

/**
* @file SkinnedMeshManager.h
* @brief �X�L�����b�V���̓ǂݍ��݁E�ێ����s��
* @author GP12B183 12 �����y��
* @details
*/


/**
* @brief �X�L�����b�V���}�l�[�W��
*/
class SkinnedMeshManager
{
private:
	std::map<const char*, SkinnedMesh*> _meshMap;	// �ǂݍ��ݍς݂̃f�[�^

public:
	SkinnedMeshManager();
	~SkinnedMeshManager();

	SkinnedMesh* Load(const char*  filePath);
	BOOL Release(const char* filePath);
	BOOL ReleaseAll(void);
};