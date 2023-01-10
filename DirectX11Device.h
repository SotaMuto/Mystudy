#pragma once

/**
* @file DirectX11Device.h
* @brief DirectX11の管理を行う
* @author GP12B183 12 小峰遥一
* @details
*/


// ライブラリとのリンク
#pragma comment (lib, "d3d11.lib")		
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "d3dx11.lib")	


// ComPtrを使用できるようにする
// 参考：https://cocoa-programing.hatenablog.com/entry/2018/11/16/000242
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;


/**
* @brief DirectX11管理クラス
* @warning 基本的にこのクラスは弄らない
*/
class DirectX11Device
{
public:
	// ブレンドモード
	enum BLEND_MODE
	{
		BLEND_MODE_NONE,		// ブレンド無し
		BLEND_MODE_ALPHA,		// αブレンド
		BLEND_MODE_ADD,			// 加算ブレンド
		BLEND_MODE_SUBTRACT,	// 減算ブレンド
	};

	// カリングモード
	enum CULL_MODE
	{
		CULL_MODE_NONE,			//カリング無し
		CULL_MODE_FRONT,		//表のポリゴンを描画しない(CW)
		CULL_MODE_BACK,			//裏のポリゴンを描画しない(CCW)
		CULL_MODE_WIRE_FRAME,	//ワイヤーフレーム	
	};


private:
	// デバイス/デバイスコンテキスト
	ComPtr<ID3D11Device>			_device;
	ComPtr<ID3D11DeviceContext>		_deviceContext;

	//! スワップチェイン
	ComPtr<IDXGISwapChain>			_swapChain;

	// レンダーターゲットビュー
	ComPtr<ID3D11RenderTargetView>	_renderTargetView;

	// 深度ステンシルビュー
	ComPtr<ID3D11Texture2D>			_depthTexture;
	ComPtr<ID3D11DepthStencilView>	_depthStencilView;

	// ビューポート
	D3D11_VIEWPORT					_viewPort;

	// 深度ステンシルステート
	ComPtr<ID3D11DepthStencilState> _depthStateEnable;
	ComPtr<ID3D11DepthStencilState> _depthStateDisable;

	// ブレンドモード
	BLEND_MODE						_curBlendMode;
	ComPtr<ID3D11BlendState>		_blendStateNone;
	ComPtr<ID3D11BlendState>		_blendStateAlpha;
	ComPtr<ID3D11BlendState>		_blendStateAdd;
	ComPtr<ID3D11BlendState>		_blendStateSub;

	// ラスタライザステート
	ComPtr<ID3D11RasterizerState>	_rasterStateCullNone;
	ComPtr<ID3D11RasterizerState>	_rasterStateCullCW;
	ComPtr<ID3D11RasterizerState>	_rasterStateCullCCW;
	ComPtr<ID3D11RasterizerState>	_rasterStateWireFrame;


public:
	HRESULT Init(HWND hWnd, BOOL isFullScr);
	void Uninit(void);

	void ClearRender(void);
	void ClearRender(ID3D11RenderTargetView* renderTargetView);
	void ClearDepth(void);
	void Present(void);

	void SetFullScreen(BOOL isFullScr);
	void SetDepthState(BOOL enable);
	void SetBlendState(BLEND_MODE blendMode);
	void SetAlphaTest(BOOL enable);
	void SetRasterizerState(CULL_MODE cullMode);
	void SetViewPort(D3D11_VIEWPORT* vp = NULL);
	void SetRenderTarget(ID3D11RenderTargetView* renderTargetView = NULL, ID3D11DepthStencilView * depthStencilView = NULL);

	ID3D11Device* GetDevice(void) const { return _device.Get(); }
	ID3D11DeviceContext* GetDeviceContext(void) const { return _deviceContext.Get(); }
	ID3D11RenderTargetView* GetDefaultRenderTargetView(void) const { return _renderTargetView.Get(); }
	IDXGISwapChain* GetSwapChain(void) const { return _swapChain.Get(); }

	// --------------シングルトンここから--------------
private:
	static DirectX11Device* _instance;

	DirectX11Device() {}	// コンストラクタはプライベートに隠す
	~DirectX11Device() {}
public:
	// インスタンス生成
	static void CreateInstance()
	{
		DeleteInstance();

		_instance = new DirectX11Device();
	}
	// インスタンス削除
	static void DeleteInstance()
	{
		if (_instance != nullptr)
		{
			delete _instance;
			_instance = nullptr;
		}
	}
	// インスタンス取得
	static DirectX11Device& GetInstance()
	{
		return *_instance;
	}
	// --------------シングルトンここまで--------------
};

/**
* @def D3D
* @brief Direct3D管理クラスのシングルトンインスタンスを簡単に取得するためのマクロ
*/
#define DX11 DirectX11Device::GetInstance()


HRESULT CompileShaderFromFile(const TCHAR* filename, const char* entryPoint, const char* shaderModel, ID3DBlob** ppBlobOut);



