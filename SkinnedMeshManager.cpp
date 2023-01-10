#include "stdafx.h"

/**
* @file SkinnedMeshManager.cpp
* @brief スキンメッシュの読みこみ保持を行う
* @author GP12B183 12 小峰遥一
* @details
*/


/**
* @brief コンストラクタ
*/
SkinnedMeshManager::SkinnedMeshManager()
{
}


/**
* @brief デストラクタ
*/
SkinnedMeshManager::~SkinnedMeshManager()
{
	ReleaseAll();
}


/**
* @brief スキンメッシュのロード
*/
SkinnedMesh* SkinnedMeshManager::Load(const char * filePath)
{
	// 既に読みこんでるものがあるか探す
	auto itr = _meshMap.find(filePath);

	if (itr != _meshMap.end())		// キーが設定されている場合
	{
		return itr->second;
	}
	else							// キーが設定されていない場合
	{
		// 拡張子でLoaderクラスを生成
		char fname[256];
		strcpy(fname, filePath);

		char *fileType;
		fileType = strtok(fname, ".");
		fileType = strtok(NULL, ".");

		if (strcmp(fileType, "fbx") != 0)
		{
			Debug.Out("未対応拡張子：%s", fileType);
			return NULL;
		}

		// スキンメッシュを生成
		SkinnedMesh* mesh = SkinnedMesh::LoadFromFile(filePath);
		_meshMap[filePath] = mesh;

		return _meshMap[filePath];
	}
}


/**
* @brief スキンメッシュの解放
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
* @brief スキンメッシュの全開放
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
