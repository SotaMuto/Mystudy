#pragma once

/**
* @file MeshManager.h
* @brief メッシュの読み込み・保持を行う
* @author GP12B183 12 小峰遥一
* @details
*/


/**
* @brief メッシュマネージャ
*/
class MeshManager
{
private:
	std::map<const char*, Mesh*> _meshMap;	// 読み込み済みのテクスチャ

public:
	MeshManager();
	~MeshManager();

	Mesh* Load(const char*  filePath);
	BOOL Release(const char* filePath);
	BOOL ReleaseAll(void);
};