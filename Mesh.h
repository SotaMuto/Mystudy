#pragma once

/**
* @file Mesh.h
* @brief ���b�V��
* @author GP12B183 12 �����y��
* @details
*/


/**
* @brief ���_�\����
*/
struct Vertex3D
{
	XMFLOAT3 position;	// ���W
	XMFLOAT3 normal;	// �@��
	XMFLOAT4 diffuse;	// ���_�J���[
	XMFLOAT2 texCoord;	// �e�N�X�`�����W
};


/**
* @brief �}�e���A���\����
*/
struct Material
{
	XMFLOAT4 ambient;		// ��������
	XMFLOAT4 diffuse;		// �g�U����
	XMFLOAT4 specular;		// ���ʔ���
	XMFLOAT4 emission;		
	float	 shininess;
	int		 illum;			// �Ɩ����f��(1�ŋ��ʔ��˖���, 2�ŗL���A�ڂ����́Fhttps://ja.wikipedia.org/wiki/Wavefront_.obj�j
	int		 noTexSampling;	// True�Ńe�N�X�`�����g�p���Ȃ�
	int		 pad;
};

/**
* @brief �`��p�T�u�Z�b�g�\����
*/
struct MeshSubset
{
	unsigned short	startIndex;		// �J�n�C���f�b�N�X
	unsigned short	indexNum;		// �C���f�b�N�X��

	Material material;				// �}�e���A���\����
	Texture* pTexture = NULL;		// �e�N�X�`���f�[�^�ւ̃|�C���^
};

struct VertexSkinned;

/**
* @brief ���b�V���N���X
*/
class Mesh
{
private:
	ID3D11Buffer*	_vertexBuffer;	// ���_�o�b�t�@
	ID3D11Buffer*	_indexBuffer;	// �C���f�b�N�X�o�b�t�@

	MeshSubset*		_subsetAry;		// �`��T�u�Z�b�g�z��
	unsigned short	_subsetNum;		// �`��T�u�Z�b�g��

	unsigned short	_vertexNum;		// ���_��
	unsigned short	_indexNum;		// �C���f�b�N�X��

public:
	Mesh();
	Mesh(Vertex3D* vAry, unsigned short vNum,
		unsigned short *idxAry, unsigned short idxNum, MeshSubset* subsetAry, unsigned short subsetNum);
	Mesh(VertexSkinned* vAry, unsigned short vNum,
		unsigned short *idxAry, unsigned short idxNum, MeshSubset* subsetAry, unsigned short subsetNum);
	~Mesh();

	void Draw(UINT stride = sizeof(Vertex3D));
};