#include "stdafx.h"

/**
* @file MeshManager.cpp
* @brief メッシュの読みこみ保持を行う
* @author GP12B183 12 小峰遥一
* @details
*/


/**
* @brief コンストラクタ
*/
MeshManager::MeshManager()
{
}


/**
* @brief デストラクタ
*/
MeshManager::~MeshManager()
{
	ReleaseAll();
}


/**
* @brief メッシュのロード
*/
Mesh* MeshManager::Load(const char * filePath)
{
	// 既に読みこんでるものがあるか探す
	auto itr = _meshMap.find(filePath);

	if (itr != _meshMap.end())		// キーが設定されている場合
	{
		return itr->second;
	}
	else							// キーが設定されていない場合
	{
		// Loaderクラス
		MeshLoaderBase *meshLoader = NULL;

		// 拡張子でLoaderクラスを生成
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
			Debug.Out("未対応拡張子：%s", fileType);
			return NULL;
		}

		if (meshLoader == NULL) return NULL;

		// メッシュを生成
		Mesh* mesh = meshLoader->LoadFromFile(filePath);
		_meshMap[filePath] = mesh;

		// 
		delete meshLoader;

		return _meshMap[filePath];
	}
}


/**
* @brief メッシュの解放
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
* @brief メッシュの全開放
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
