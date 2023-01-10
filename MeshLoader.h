#pragma once

/**
* @file MeshLoader.h
* @brief メッシュ読み込みクラスの親クラス
* @author GP12B183 12 小峰遥一
* @details OBJやFBXのLoaderクラスはこれを継承する
*/

class MeshLoaderBase
{
public:
	MeshLoaderBase() {}
	virtual ~MeshLoaderBase() {}

	virtual Mesh* LoadFromFile(const char* filePath) = 0;
};