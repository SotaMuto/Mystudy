#include "stdafx.h"

/**
* @file MeshManager.cpp
* @brief ���b�V���̓ǂ݂��ݕێ����s��
* @author GP12B183 12 �����y��
* @details
*/


/**
* @brief �R���X�g���N�^
*/
MeshManager::MeshManager()
{
}


/**
* @brief �f�X�g���N�^
*/
MeshManager::~MeshManager()
{
	ReleaseAll();
}


/**
* @brief ���b�V���̃��[�h
*/
Mesh* MeshManager::Load(const char * filePath)
{
	// ���ɓǂ݂���ł���̂����邩�T��
	auto itr = _meshMap.find(filePath);

	if (itr != _meshMap.end())		// �L�[���ݒ肳��Ă���ꍇ
	{
		return itr->second;
	}
	else							// �L�[���ݒ肳��Ă��Ȃ��ꍇ
	{
		// Loader�N���X
		MeshLoaderBase *meshLoader = NULL;

		// �g���q��Loader�N���X�𐶐�
		char fname[256];
		strcpy(fname, filePath);

		char *fileType;
		fileType = strtok(fname, ".");
		fileType = strtok(NULL, ".");

		if (strcmp(fileType, "obj") == 0)
		{
			meshLoader = new OBJLoader();
		}
		//else if (strcmp(fileType, "fbx") == 0)
		//{
		//	meshLoader = new FBXLoader();
		//}
		else
		{
			Debug.Out("���Ή��g���q�F%s", fileType);
			return NULL;
		}

		if (meshLoader == NULL) return NULL;

		// ���b�V���𐶐�
		Mesh* mesh = meshLoader->LoadFromFile(filePath);
		_meshMap[filePath] = mesh;

		// 
		delete meshLoader;

		return _meshMap[filePath];
	}
}


/**
* @brief ���b�V���̉��
*/
BOOL MeshManager::Release(const char * filePath)
{
	auto itr = _meshMap.find(filePath);
	if (itr == _meshMap.end()) return FALSE;

	delete itr->second;
	_meshMap.erase(itr);

	return TRUE;
}


/**
* @brief ���b�V���̑S�J��
*/
BOOL MeshManager::ReleaseAll(void)
{
	for (auto itr = _meshMap.begin(); itr != _meshMap.end(); itr++)
	{
		delete itr->second;
	}

	_meshMap.clear();

	return TRUE;
}
