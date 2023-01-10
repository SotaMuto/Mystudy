#include "stdafx.h"

/**
* @file Texture.cpp
* @brief テクスチャ
* @author GP12B183 12 小峰遥一
* @details
*/


/**
* @brief コンストラクタ
*/
Texture::Texture()
{
}


/**
* @brief デストラクタ
*/
Texture::~Texture()
{
	Unload();
}


/*
@brief ファイルから読み込み
*/
void Texture::LoadData(const char * filePath)
{
	HRESULT hr;

	// ファイルからテクスチャ読み込み
	ID3D11Resource* pRes;
	hr = D3DX11CreateTextureFromFile(DX11.GetDevice(), filePath, NULL, NULL, &pRes, NULL);
	_tex2D = static_cast<ID3D11Texture2D*>(pRes);
	if (FAILED(hr)) Debug.ErrorDialog("テクスチャの読み込みに失敗しました：%s", filePath);

	// テクスチャ情報を取得する
	D3D11_TEXTURE2D_DESC texDesc;
	_tex2D->GetDesc(&texDesc);

	// シェーダーリソースビュー作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	hr = DX11.GetDevice()->CreateShaderResourceView(_tex2D, &srvDesc, &_srv);
	if (FAILED(hr)) Debug.ErrorDialog("シェーダーリソースビューの作成に失敗しました：%s", filePath);

	// サンプラ作成
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.MaxAnisotropy = 1;
	samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = DX11.GetDevice()->CreateSamplerState(&samDesc, &_samplerState);
	if (FAILED(hr)) Debug.ErrorDialog("サンプラの作成に失敗しました：%s", filePath);
}


/*
@brief 作成したテクスチャをセットして、それをもとにSRVとサンプラを作成
*/
void Texture::SetData(ID3D11Texture2D * tex2D)
{
	HRESULT hr;

	_tex2D = tex2D;

	// テクスチャ情報を取得する
	D3D11_TEXTURE2D_DESC texDesc;
	_tex2D->GetDesc(&texDesc);

	// シェーダーリソースビュー作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	hr = DX11.GetDevice()->CreateShaderResourceView(_tex2D, &srvDesc, &_srv);
	if (FAILED(hr)) Debug.ErrorDialog("シェーダーリソースビューの作成に失敗しました");

	// サンプラ作成
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.MaxAnisotropy = 1;
	samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = DX11.GetDevice()->CreateSamplerState(&samDesc, &_samplerState);
	if (FAILED(hr)) Debug.ErrorDialog("サンプラの作成に失敗しました");
}


/**
* @brief データ解放処理
*/
void Texture::Unload(void)
{
	SAFE_RELEASE(_tex2D);
	SAFE_RELEASE(_srv);
	SAFE_RELEASE(_samplerState);
}


/**
* @brief シェーダにSRVとサンプラを送る
*/
void Texture::SetActive(void)
{
	if (_srv == NULL || _samplerState == NULL) return;

	// テクスチャセット
	DX11.GetDeviceContext()->PSSetShaderResources(0, 1, &_srv);

	// サンプラセット
	DX11.GetDeviceContext()->PSSetSamplers(0, 1, &_samplerState);
}
