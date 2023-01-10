#pragma once

/**
* @file Texture.h
* @brief テクスチャ
* @author GP12B183 12 小峰遥一
* @details
*/

/**
* @brief テクスチャクラス
*/
class Texture
{
private:
	ID3D11Texture2D* _tex2D = NULL;			// 2Dテクスチャ
	ID3D11ShaderResourceView* _srv = NULL;	// シェーダーとリソースをつなぐインターフェース
	ID3D11SamplerState* _samplerState;		// サンプラー

public:
	Texture();
	virtual ~Texture();

	virtual void LoadData(const char* filePath);
	void SetData(ID3D11Texture2D* tex2D);
	void Unload(void);

	//! シェーダにSRVとサンプラを送る
	void SetActive(void);

	ID3D11Texture2D* GetTexture2D(void) { return _tex2D; }
	ID3D11ShaderResourceView** GetShaderResourceView(void) { return &_srv; }
	ID3D11SamplerState** GetSamplerState(void) { return &_samplerState; }
};