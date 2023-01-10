#pragma once

/**
* @file SkinnedMesh.h
* @brief �X�L�����b�V��
* @author GP12B183 12 �����y��
* @details
*/


// Blender�ŏo�͂���ꍇ�A�o�͐ݒ�ŃX�P�[���W����0.01�ɂ��邱��
// �{�[���̍ő吔��12�܂ŁA�A�j���[�V���������̃��f���͍l�����ĂȂ�

// ToDo:�A�j���[�V�����u�����h

#define BONE_MAX				(12)
#define WEIGHT_MAX_PER_VERTEX	(4)	// �e���_�̉e���{�[���ő吔

/**
* @brief �X�L�����b�V�����_�\����
*/
struct VertexSkinned
{
	XMFLOAT3		position;
	XMFLOAT3		normal;
	XMFLOAT4		diffuse;
	XMFLOAT2		texCoord;
	unsigned char	mtxIndex[WEIGHT_MAX_PER_VERTEX];	// �{�[���̗v�f�ԍ�
	float			weight[WEIGHT_MAX_PER_VERTEX];		// �E�F�C�g�l

public:

	// �{�[���̃E�F�C�g��ݒ�
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

	// �E�F�C�g�̍��v�l��1.0f�ɂ���
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
* @brief �X�L�����b�V���p���f��
*/
class SkinnedMesh
{
private:
	FBXLoader::IScene*	_fbxScene = NULL;		// FBX�f�[�^

	Mesh*			_mesh    = NULL;
	int				_boneNum = 0;			// �{�[����
	Bone*			_boneAry = NULL;		// �{�[���z��
	XMFLOAT4X4		_mtxCombAry[BONE_MAX];	// �����ϊ��s��i������V�F�[�_�[�ɓn���j

public:
	SkinnedMesh();
	~SkinnedMesh();

	void Update(XMMATRIX* mtxWorld, int animStackID, double animTime);
	void Draw(void);

	double LoopFrame(int animStackID, double animTime, float deltaTime);

	//! ��肠�������[�_�[�ɕ������Ȃ�
	static SkinnedMesh* LoadFromFile(const char* filePath);
};