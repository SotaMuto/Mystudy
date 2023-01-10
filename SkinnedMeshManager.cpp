#include "stdafx.h"

/**
* @file SkinnedMeshManager.cpp
* @brief �X�L�����b�V���̓ǂ݂��ݕێ����s��
* @author GP12B183 12 �����y��
* @details
*/


/**
* @brief �R���X�g���N�^
*/
SkinnedMeshManager::SkinnedMeshManager()
{
}


/**
* @brief �f�X�g���N�^
*/
SkinnedMeshManager::~SkinnedMeshManager()
{
	ReleaseAll();
}


/**
* @brief �X�L�����b�V���̃��[�h
*/
SkinnedMesh* SkinnedMeshManager::Load(const char * filePath)
{
	// ���ɓǂ݂���ł���̂����邩�T��
	auto itr = _meshMap.find(filePath);

	if (itr != _meshMap.end())		// �L�[���ݒ肳��Ă���ꍇ
	{
		return itr->second;
	}
	else							// �L�[���ݒ肳��Ă��Ȃ��ꍇ
	{
		// �g���q��Loader�N���X�𐶐�
		char fname[256];
		strcpy(fname, filePath);

		char *fileType;
		fileType = strtok(fname, ".");
		fileType = strtok(NULL, ".");

		if (strcmp(fileType, "fbx") != 0)
		{
			Debug.Out("���Ή��g���q�F%s", fileType);
			return NULL;
		}

		// �X�L�����b�V���𐶐�
		SkinnedMesh* mesh = SkinnedMesh::LoadFromFile(filePath);
		_meshMap[filePath] = mesh;

		return _meshMap[filePath];
	}
}


/**
* @brief �X�L�����b�V���̉��
*/
BOOL SkinnedMeshManager::Release(const char * filePath)
{
	auto itr = _meshMap.find(filePath);
	if (itr == _meshMap.end()) return FALSE;

	delete itr->second;
	_meshMap.erase(itr);

	return TRUE;
}


/**
* @brief �X�L�����b�V���̑S�J��
*/
BOOL SkinnedMeshManager::ReleaseAll(void)
{
	for (auto itr = _meshMap.begin(); itr != _meshMap.end(); itr++)
	{
		delete itr->second;
	}

	_meshMap.clear();

	return TRUE;
}
