#include "stdafx.h"
#include "Mesh.h"

/**
* @fp Mesh.cpp
* @brief ���b�V��
* @author GP12B183 12 �����y��
* @details
*/


/**
* @brief �R���X�g���N�^
*/
Mesh::Mesh()
{
}

Mesh::Mesh(Vertex3D * vAry, unsigned short vNum, 
	unsigned short * idxAry, unsigned short idxNum, MeshSubset * subsetAry, unsigned short subsetNum)
{
	// ���_�o�b�t�@����
	{
		_vertexNum = vNum;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.ByteWidth = sizeof(Vertex3D) * _vertexNum;

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = vAry;

		DX11.GetDevice()->CreateBuffer(&bd, &sd, &_vertexBuffer);
	}


	// �C���f�b�N�X�o�b�t�@����
	{
		_indexNum = idxNum;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned short) * _indexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = idxAry;

		DX11.GetDevice()->CreateBuffer(&bd, &sd, &_indexBuffer);
	}

	// �T�u�Z�b�g�ݒ�
	{
		_subsetAry = subsetAry;
		_subsetNum = subsetNum;
	}
}


Mesh::Mesh(VertexSkinned * vAry, unsigned short vNum,
	unsigned short * idxAry, unsigned short idxNum, MeshSubset * subsetAry, unsigned short subsetNum)
{
	// ���_�o�b�t�@����
	{
		_vertexNum = vNum;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.ByteWidth = sizeof(VertexSkinned) * _vertexNum;

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = vAry;

		DX11.GetDevice()->CreateBuffer(&bd, &sd, &_vertexBuffer);
	}


	// �C���f�b�N�X�o�b�t�@����
	{
		_indexNum = idxNum;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned short) * _indexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = idxAry;

		DX11.GetDevice()->CreateBuffer(&bd, &sd, &_indexBuffer);
	}

	// �T�u�Z�b�g�ݒ�
	{
		_subsetAry = subsetAry;
		_subsetNum = subsetNum;
	}
}


/**
* @brief �f�X�g���N�^
*/
Mesh::~Mesh()
{
	SAFE_RELEASE(_vertexBuffer);
	SAFE_RELEASE(_indexBuffer);
	SAFE_DELETE_ARRAY(_subsetAry);
}


/**
* @brief �`�揈��
*/
void Mesh::Draw(UINT stride)
{
	// ���_�o�b�t�@�ݒ�
	//UINT stride = sizeof(Vertex3D);
	UINT offset = 0;
	DX11.GetDeviceContext()->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�ݒ�
	DX11.GetDeviceContext()->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// �v���~�e�B�u�g�|���W
	DX11.GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (unsigned short i = 0; i < _subsetNum; i++)
	{
		// �}�e���A���ݒ�
		Game.GetRenderer()->SetMaterialBuffer(&_subsetAry[i].material);

		// �e�N�X�`���ݒ�
		if (_subsetAry[i].material.noTexSampling == FALSE)
		{
			// �e�N�X�`���ƃT���v�����V�F�[�_�[�ɃZ�b�g
			_subsetAry[i].pTexture->SetActive();

			//DX11.GetDeviceContext()->DSSetShaderResources(0, 1, _subsetAry[i].pTexture->GetShaderResourceView());
		}

		// �|���S���`��
		DX11.GetDeviceContext()->DrawIndexed(_subsetAry[i].indexNum, _subsetAry[i].startIndex, 0);
	}
}
