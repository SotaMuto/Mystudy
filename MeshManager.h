#pragma once

/**
* @file MeshManager.h
* @brief ���b�V���̓ǂݍ��݁E�ێ����s��
* @author GP12B183 12 �����y��
* @details
*/


/**
* @brief ���b�V���}�l�[�W��
*/
class MeshManager
{
private:
	std::map<const char*, Mesh*> _meshMap;	// �ǂݍ��ݍς݂̃e�N�X�`��

public:
	MeshManager();
	~MeshManager();

	Mesh* Load(const char*  filePath);
	BOOL Release(const char* filePath);
	BOOL ReleaseAll(void);
};