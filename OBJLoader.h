#pragma once

/**
* @file OBJLoader.h
* @brief Objファイルの読み込み
* @author GP12B183 12 小峰遥一
* @details
*/


struct OBJ_Material
{
	char		name[256];
	Material	material;
	char		textureName[256];
};

struct OBJ_Subset
{
	unsigned short startIndex;
	unsigned short indexNum;
	OBJ_Material   material;
};

/**
* @brief OBJファイルの読み込みクラス
*/
class OBJLoader : public MeshLoaderBase
{
private:
	static const float SCALE_MODEL;

	Vertex3D*			_vertexAry = NULL;
	unsigned short		_vertexNum = 0;
	unsigned short*		_indexAry  = NULL;
	unsigned short		_indexNum  = 0;
	OBJ_Subset*			_subsetAry = NULL;
	unsigned short		_subsetNum = 0;
	OBJ_Material*		_matAry    = NULL;
	unsigned short		_matNum    = 0;

	void LoadMaterial(const char* filePath);
	Mesh* CreateMesh(void);

public:
	OBJLoader();
	~OBJLoader();

	Mesh* LoadFromFile(const char* filePath) override;
	void Unload(void);
};