#include "stdafx.h"

/**
* @file TextureManager.cpp
* @brief �e�N�X�`���̕ێ����s��
* @author GP12B183 12 �����y��
* @details
*/


/**
* @brief �R���X�g���N�^
*/
TextureManager::TextureManager()
{
}


/**
* @brief �f�X�g���N�^
*/
TextureManager::~TextureManager()
{
	ReleaseAll();
}


/**
* @brief �e�N�X�`���̃��[�h
*/
Texture* TextureManager::Load(const char * filePath)
{
	// �L���b�V���Ƀe�N�X�`�������邩�T��
	auto itr = _texMap.find(filePath);

	if (itr != _texMap.end())		// �L�[���ݒ肳��Ă���ꍇ
	{
		return itr->second;
	}
	else							// �L�[���ݒ肳��Ă��Ȃ��ꍇ
	{
		Texture *tex = new Texture();
		tex->LoadData(filePath);

		_texMap[filePath] = tex;

		return _texMap[filePath];
	}

}


/**
* @brief �e�N�X�`���̉��
*/
BOOL TextureManager::Release(const char * filePath)
{
	auto itr = _texMap.find(filePath);
	if (itr == _texMap.end()) return FALSE;

	delete itr->second;
	_texMap.erase(itr);

	return TRUE;
}


/**
* @brief �e�N�X�`���̑S�J��
*/
BOOL TextureManager::ReleaseAll(void)
{
	for (auto itr = _texMap.begin(); itr != _texMap.end(); itr++)
	{
		delete itr->second;
	}

	_texMap.clear();

	return TRUE;
}
