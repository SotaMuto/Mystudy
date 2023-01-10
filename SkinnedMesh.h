#pragma once

/**
* @file SkinnedMesh.h
* @brief スキンメッシュ
* @author GP12B183 12 小峰遥一
* @details
*/


// Blenderで出力する場合、出力設定でスケール係数を0.01にすること
// ボーンの最大数は12個まで、アニメーション無しのモデルは考慮してない

// ToDo:アニメーションブレンド

#define BONE_MAX				(12)
#define WEIGHT_MAX_PER_VERTEX	(4)	// 各頂点の影響ボーン最大数

/**
* @brief スキンメッシュ頂点構造体
*/
struct VertexSkinned
{
	XMFLOAT3		position;
	XMFLOAT3		normal;
	XMFLOAT4		diffuse;
	XMFLOAT2		texCoord;
	unsigned char	mtxIndex[WEIGHT_MAX_PER_VERTEX];	// ボーンの要素番号
	float			weight[WEIGHT_MAX_PER_VERTEX];		// ウェイト値

public:

	// ボーンのウェイトを設定
	void SetWeight(int boneIdx, float boneWeight)
	{
		int minIdx = -1;
		float minWeight = boneWeight;
		for (int i = 0; i < WEIGHT_MAX_PER_VERTEX; i++)
		{
			if (weight[i] < minWeight)
			{
				minIdx = i;
				minWeight = weight[i];
			}
		}

		if (minIdx != -1)
		{
			mtxIndex[minIdx] = boneIdx;
			weight[minIdx] = boneWeight;
		}
	}

	// ウェイトの合計値を1.0fにする
	void NormalizeWeight(void)
	{
		float sumWeight = 0.0f;
		for (int i = 0; i < WEIGHT_MAX_PER_VERTEX; i++)
		{
			sumWeight += weight[i];
		}

		for (int i = 0; i < WEIGHT_MAX_PER_VERTEX; i++)
		{
			weight[i] = weight[i] / sumWeight;
		}

	}
};


/**
* @brief スキンメッシュ用モデル
*/
class SkinnedMesh
{
private:
	FBXLoader::IScene*	_fbxScene = NULL;		// FBXデータ

	Mesh*			_mesh    = NULL;
	int				_boneNum = 0;			// ボーン数
	Bone*			_boneAry = NULL;		// ボーン配列
	XMFLOAT4X4		_mtxCombAry[BONE_MAX];	// 合成変換行列（これをシェーダーに渡す）

public:
	SkinnedMesh();
	~SkinnedMesh();

	void Update(XMMATRIX* mtxWorld, int animStackID, double animTime);
	void Draw(void);

	double LoopFrame(int animStackID, double animTime, float deltaTime);

	//! 取りあえずローダーに分離しない
	static SkinnedMesh* LoadFromFile(const char* filePath);
};