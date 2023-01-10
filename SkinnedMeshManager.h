#pragma once

/**
* @file SkinnedMeshManager.h
* @brief スキンメッシュの読み込み・保持を行う
* @author GP12B183 12 小峰遥一
* @details
*/


/**
* @brief スキンメッシュマネージャ
*/
class SkinnedMeshManager
{
private:
	std::map<const char*, SkinnedMesh*> _meshMap;	// 読み込み済みのデータ

public:
	SkinnedMeshManager();
	~SkinnedMeshManager();

	SkinnedMesh* Load(const char*  filePath);
	BOOL Release(const char* filePath);
	BOOL ReleaseAll(void);
};