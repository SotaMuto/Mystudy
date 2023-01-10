#include "stdafx.h"

/**
* @file TextureManager.cpp
* @brief テクスチャの保持を行う
* @author GP12B183 12 小峰遥一
* @details
*/


/**
* @brief コンストラクタ
*/
TextureManager::TextureManager()
{
}


/**
* @brief デストラクタ
*/
TextureManager::~TextureManager()
{
	ReleaseAll();
}


/**
* @brief テクスチャのロード
*/
Texture* TextureManager::Load(const char * filePath)
{
	// キャッシュにテクスチャがあるか探す
	auto itr = _texMap.find(filePath);

	if (itr != _texMap.end())		// キーが設定されている場合
	{
		return itr->second;
	}
	else							// キーが設定されていない場合
	{
		Texture *tex = new Texture();
		tex->LoadData(filePath);

		_texMap[filePath] = tex;

		return _texMap[filePath];
	}

}


/**
* @brief テクスチャの解放
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
* @brief テクスチャの全開放
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
