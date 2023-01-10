#pragma once

/**
* @file TextureManager.h
* @brief テクスチャの保持を行う
* @author GP12B183 12 小峰遥一
* @details
*/


/**
* @brief テクスチャマネージャ
*/
class TextureManager
{
private:
	std::map<std::string, Texture*> _texMap;	// 読み込み済みのテクスチャ

public:
	TextureManager();
	~TextureManager();

	Texture* Load(const char*  filePath);
	BOOL Release(const char* filePath);
	BOOL ReleaseAll(void);
};