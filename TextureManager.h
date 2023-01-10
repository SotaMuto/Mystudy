#pragma once

/**
* @file TextureManager.h
* @brief �e�N�X�`���̕ێ����s��
* @author GP12B183 12 �����y��
* @details
*/


/**
* @brief �e�N�X�`���}�l�[�W��
*/
class TextureManager
{
private:
	std::map<std::string, Texture*> _texMap;	// �ǂݍ��ݍς݂̃e�N�X�`��

public:
	TextureManager();
	~TextureManager();

	Texture* Load(const char*  filePath);
	BOOL Release(const char* filePath);
	BOOL ReleaseAll(void);
};