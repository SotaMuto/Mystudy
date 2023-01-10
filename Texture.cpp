#include "stdafx.h"

/**
* @file Texture.cpp
* @brief �e�N�X�`��
* @author GP12B183 12 �����y��
* @details
*/


/**
* @brief �R���X�g���N�^
*/
Texture::Texture()
{
}


/**
* @brief �f�X�g���N�^
*/
Texture::~Texture()
{
	Unload();
}


/*
@brief �t�@�C������ǂݍ���
*/
void Texture::LoadData(const char * filePath)
{
	HRESULT hr;

	// �t�@�C������e�N�X�`���ǂݍ���
	ID3D11Resource* pRes;
	hr = D3DX11CreateTextureFromFile(DX11.GetDevice(), filePath, NULL, NULL, &pRes, NULL);
	_tex2D = static_cast<ID3D11Texture2D*>(pRes);
	if (FAILED(hr)) Debug.ErrorDialog("�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����F%s", filePath);

	// �e�N�X�`�������擾����
	D3D11_TEXTURE2D_DESC texDesc;
	_tex2D->GetDesc(&texDesc);

	// �V�F�[�_�[���\�[�X�r���[�쐬
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	hr = DX11.GetDevice()->CreateShaderResourceView(_tex2D, &srvDesc, &_srv);
	if (FAILED(hr)) Debug.ErrorDialog("�V�F�[�_�[���\�[�X�r���[�̍쐬�Ɏ��s���܂����F%s", filePath);

	// �T���v���쐬
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.MaxAnisotropy = 1;
	samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = DX11.GetDevice()->CreateSamplerState(&samDesc, &_samplerState);
	if (FAILED(hr)) Debug.ErrorDialog("�T���v���̍쐬�Ɏ��s���܂����F%s", filePath);
}


/*
@brief �쐬�����e�N�X�`�����Z�b�g���āA��������Ƃ�SRV�ƃT���v�����쐬
*/
void Texture::SetData(ID3D11Texture2D * tex2D)
{
	HRESULT hr;

	_tex2D = tex2D;

	// �e�N�X�`�������擾����
	D3D11_TEXTURE2D_DESC texDesc;
	_tex2D->GetDesc(&texDesc);

	// �V�F�[�_�[���\�[�X�r���[�쐬
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	hr = DX11.GetDevice()->CreateShaderResourceView(_tex2D, &srvDesc, &_srv);
	if (FAILED(hr)) Debug.ErrorDialog("�V�F�[�_�[���\�[�X�r���[�̍쐬�Ɏ��s���܂���");

	// �T���v���쐬
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.MaxAnisotropy = 1;
	samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = DX11.GetDevice()->CreateSamplerState(&samDesc, &_samplerState);
	if (FAILED(hr)) Debug.ErrorDialog("�T���v���̍쐬�Ɏ��s���܂���");
}


/**
* @brief �f�[�^�������
*/
void Texture::Unload(void)
{
	SAFE_RELEASE(_tex2D);
	SAFE_RELEASE(_srv);
	SAFE_RELEASE(_samplerState);
}


/**
* @brief �V�F�[�_��SRV�ƃT���v���𑗂�
*/
void Texture::SetActive(void)
{
	if (_srv == NULL || _samplerState == NULL) return;

	// �e�N�X�`���Z�b�g
	DX11.GetDeviceContext()->PSSetShaderResources(0, 1, &_srv);

	// �T���v���Z�b�g
	DX11.GetDeviceContext()->PSSetSamplers(0, 1, &_samplerState);
}
