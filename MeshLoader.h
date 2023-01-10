#pragma once

/**
* @file MeshLoader.h
* @brief ���b�V���ǂݍ��݃N���X�̐e�N���X
* @author GP12B183 12 �����y��
* @details OBJ��FBX��Loader�N���X�͂�����p������
*/

class MeshLoaderBase
{
public:
	MeshLoaderBase() {}
	virtual ~MeshLoaderBase() {}

	virtual Mesh* LoadFromFile(const char* filePath) = 0;
};