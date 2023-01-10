#pragma once

/**
* @file DirectX11Device.h
* @brief DirectX11�̊Ǘ����s��
* @author GP12B183 12 �����y��
* @details
*/


// ���C�u�����Ƃ̃����N
#pragma comment (lib, "d3d11.lib")		
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "d3dx11.lib")	


// ComPtr���g�p�ł���悤�ɂ���
// �Q�l�Fhttps://cocoa-programing.hatenablog.com/entry/2018/11/16/000242
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;


/**
* @brief DirectX11�Ǘ��N���X
* @warning ��{�I�ɂ��̃N���X�͘M��Ȃ�
*/
class DirectX11Device
{
public:
	// �u�����h���[�h
	enum BLEND_MODE
	{
		BLEND_MODE_NONE,		// �u�����h����
		BLEND_MODE_ALPHA,		// ���u�����h
		BLEND_MODE_ADD,			// ���Z�u�����h
		BLEND_MODE_SUBTRACT,	// ���Z�u�����h
	};

	// �J�����O���[�h
	enum CULL_MODE
	{
		CULL_MODE_NONE,			//�J�����O����
		CULL_MODE_FRONT,		//�\�̃|���S����`�悵�Ȃ�(CW)
		CULL_MODE_BACK,			//���̃|���S����`�悵�Ȃ�(CCW)
		CULL_MODE_WIRE_FRAME,	//���C���[�t���[��	
	};


private:
	// �f�o�C�X/�f�o�C�X�R���e�L�X�g
	ComPtr<ID3D11Device>			_device;
	ComPtr<ID3D11DeviceContext>		_deviceContext;

	//! �X���b�v�`�F�C��
	ComPtr<IDXGISwapChain>			_swapChain;

	// �����_�[�^�[�Q�b�g�r���[
	ComPtr<ID3D11RenderTargetView>	_renderTargetView;

	// �[�x�X�e���V���r���[
	ComPtr<ID3D11Texture2D>			_depthTexture;
	ComPtr<ID3D11DepthStencilView>	_depthStencilView;

	// �r���[�|�[�g
	D3D11_VIEWPORT					_viewPort;

	// �[�x�X�e���V���X�e�[�g
	ComPtr<ID3D11DepthStencilState> _depthStateEnable;
	ComPtr<ID3D11DepthStencilState> _depthStateDisable;

	// �u�����h���[�h
	BLEND_MODE						_curBlendMode;
	ComPtr<ID3D11BlendState>		_blendStateNone;
	ComPtr<ID3D11BlendState>		_blendStateAlpha;
	ComPtr<ID3D11BlendState>		_blendStateAdd;
	ComPtr<ID3D11BlendState>		_blendStateSub;

	// ���X�^���C�U�X�e�[�g
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

	// --------------�V���O���g����������--------------
private:
	static DirectX11Device* _instance;

	DirectX11Device() {}	// �R���X�g���N�^�̓v���C�x�[�g�ɉB��
	~DirectX11Device() {}
public:
	// �C���X�^���X����
	static void CreateInstance()
	{
		DeleteInstance();

		_instance = new DirectX11Device();
	}
	// �C���X�^���X�폜
	static void DeleteInstance()
	{
		if (_instance != nullptr)
		{
			delete _instance;
			_instance = nullptr;
		}
	}
	// �C���X�^���X�擾
	static DirectX11Device& GetInstance()
	{
		return *_instance;
	}
	// --------------�V���O���g�������܂�--------------
};

/**
* @def D3D
* @brief Direct3D�Ǘ��N���X�̃V���O���g���C���X�^���X���ȒP�Ɏ擾���邽�߂̃}�N��
*/
#define DX11 DirectX11Device::GetInstance()


HRESULT CompileShaderFromFile(const TCHAR* filename, const char* entryPoint, const char* shaderModel, ID3DBlob** ppBlobOut);



