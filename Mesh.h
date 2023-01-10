#pragma once

/**
* @file Mesh.h
* @brief メッシュ
* @author GP12B183 12 小峰遥一
* @details
*/


/**
* @brief 頂点構造体
*/
struct Vertex3D
{
	XMFLOAT3 position;	// 座標
	XMFLOAT3 normal;	// 法線
	XMFLOAT4 diffuse;	// 頂点カラー
	XMFLOAT2 texCoord;	// テクスチャ座標
};


/**
* @brief マテリアル構造体
*/
struct Material
{
	XMFLOAT4 ambient;		// 環境光反射
	XMFLOAT4 diffuse;		// 拡散反射
	XMFLOAT4 specular;		// 鏡面反射
	XMFLOAT4 emission;		
	float	 shininess;
	int		 illum;			// 照明モデル(1で鏡面反射無効, 2で有効、詳しくは：https://ja.wikipedia.org/wiki/Wavefront_.obj）
	int		 noTexSampling;	// Trueでテクスチャを使用しない
	int		 pad;
};

/**
* @brief 描画用サブセット構造体
*/
struct MeshSubset
{
	unsigned short	startIndex;		// 開始インデックス
	unsigned short	indexNum;		// インデックス数

	Material material;				// マテリアル構造体
	Texture* pTexture = NULL;		// テクスチャデータへのポインタ
};

struct VertexSkinned;

/**
* @brief メッシュクラス
*/
class Mesh
{
private:
	ID3D11Buffer*	_vertexBuffer;	// 頂点バッファ
	ID3D11Buffer*	_indexBuffer;	// インデックスバッファ

	MeshSubset*		_subsetAry;		// 描画サブセット配列
	unsigned short	_subsetNum;		// 描画サブセット数

	unsigned short	_vertexNum;		// 頂点数
	unsigned short	_indexNum;		// インデックス数

public:
	Mesh();
	Mesh(Vertex3D* vAry, unsigned short vNum,
		unsigned short *idxAry, unsigned short idxNum, MeshSubset* subsetAry, unsigned short subsetNum);
	Mesh(VertexSkinned* vAry, unsigned short vNum,
		unsigned short *idxAry, unsigned short idxNum, MeshSubset* subsetAry, unsigned short subsetNum);
	~Mesh();

	void Draw(UINT stride = sizeof(Vertex3D));
};