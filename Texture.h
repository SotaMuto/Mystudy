#pragma once

/**
* @file Texture.h
* @brief �e�N�X�`��
* @author GP12B183 12 �����y��
* @details
*/

/**
* @brief �e�N�X�`���N���X
*/
class Texture
{
private:
	ID3D11Texture2D* _tex2D = NULL;			// 2D�e�N�X�`��
	ID3D11ShaderResourceView* _srv = NULL;	// �V�F�[�_�[�ƃ��\�[�X���Ȃ��C���^�[�t�F�[�X
	ID3D11SamplerState* _samplerState;		// �T���v���[

public:
	Texture();
	virtual ~Texture();

	virtual void LoadData(const char* filePath);
	void SetData(ID3D11Texture2D* tex2D);
	void Unload(void);

	//! �V�F�[�_��SRV�ƃT���v���𑗂�
	void SetActive(void);

	ID3D11Texture2D* GetTexture2D(void) { return _tex2D; }
	ID3D11ShaderResourceView** GetShaderResourceView(void) { return &_srv; }
	ID3D11SamplerState** GetSamplerState(void) { return &_samplerState; }
};