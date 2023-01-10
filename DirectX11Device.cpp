/**
* @file DirectX11Device.cpp
* @brief DirectX11�̊Ǘ����s��
* @author GP12B183 12 �����y��
* @details
*/

#include "stdafx.h"

// �V���O���g���̃C���X�^���X�錾
DirectX11Device* DirectX11Device::_instance = nullptr;


/**
* @brief Direct3D�Ǘ��N���X�̏���������
* @param[in] hWnd �E�B���h�E�n���h��
* @param[in] isFullScr �t���X�N���[���ŋN������
* @return bool ������������true
* @details
*/
HRESULT DirectX11Device::Init(HWND hWnd, BOOL isFullScr)
{
	HRESULT hr = S_OK;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	// �f�o�C�X�A�X���b�v�`�F�[���A�R���e�L�X�g����
	DWORD deviceFlags = 0;
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = SCREEN_WIDTH;
	sd.BufferDesc.Height = SCREEN_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = isFullScr;


	//�f�o�b�O�����o�͗p�ݒ�
#if defined(_DEBUG) && defined(DEBUG_DISP_TEXTOUT)
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE;
	deviceFlags = D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#endif


	// ����
	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		deviceFlags,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&_swapChain,
		&_device,
		&featureLevel,
		&_deviceContext);
	if(FAILED(hr)) Debug.ErrorDialog("�f�o�C�X�쐬���s");



	//�f�o�b�O�����o�͗p�ݒ�
#if defined(_DEBUG) && defined(DEBUG_DISP_TEXTOUT)
	hr = _swapChain->ResizeBuffers(0, 
		SCREEN_WIDTH, SCREEN_HEIGHT, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE);
	if(FAILED(hr)) Debug.ErrorDialog("�o�b�t�@�[�m�ێ��s");
#endif


	// �����_�[�^�[�Q�b�g�r���[�쐬
	ID3D11Texture2D* pBackBuffer = NULL;
	_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	hr = _device->CreateRenderTargetView(pBackBuffer, NULL, &_renderTargetView);
	pBackBuffer->Release();
	if(FAILED(hr)) Debug.ErrorDialog("�����_�[�^�[�Q�b�g�쐬���s");


	//�X�e���V���p�e�N�X�`���[�쐬
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = SCREEN_WIDTH;
	td.Height = SCREEN_HEIGHT;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R24G8_TYPELESS;
	td.SampleDesc = sd.SampleDesc;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	hr = _device->CreateTexture2D(&td, NULL, &_depthTexture);
	if(FAILED(hr)) Debug.ErrorDialog("�[�x�X�e���V���e�N�X�`���쐬���s");


	//�X�e���V���^�[�Q�b�g�쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//dsvd.Format = DXGI_FORMAT_D32_FLOAT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags = 0;
	hr = _device->CreateDepthStencilView(_depthTexture.Get(), &dsvd, &_depthStencilView);
	if(FAILED(hr)) Debug.ErrorDialog("�[�x�X�e���V���r���[�쐬���s");


	// �r���[�|�[�g
	ZeroMemory(&_viewPort, sizeof(D3D11_VIEWPORT));
	_viewPort.Width    = SCREEN_WIDTH;
	_viewPort.Height   = SCREEN_HEIGHT;
	_viewPort.MinDepth = 0.0f;
	_viewPort.MaxDepth = 1.0f;
	_viewPort.TopLeftX = 0;
	_viewPort.TopLeftY = 0;


	// �[�x�X�e���V���X�e�[�g�쐬
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable    = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc      = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable  = FALSE;
	hr = _device->CreateDepthStencilState(&depthStencilDesc, &_depthStateEnable);	//�[�x�L���X�e�[�g
	if(FAILED(hr)) Debug.ErrorDialog("�[�x�X�e���V���X�e�[�g�쐬���s");
	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	hr = _device->CreateDepthStencilState(&depthStencilDesc, &_depthStateDisable);	//�[�x�����X�e�[�g
	if(FAILED(hr)) Debug.ErrorDialog("�[�x�X�e���V���X�e�[�g�쐬���s");

	// �u�����h�X�e�[�g�쐬
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = _device->CreateBlendState(&blendDesc, &_blendStateAlpha);
	if(FAILED(hr)) Debug.ErrorDialog("�u�����h�X�e���V���X�e�[�g�쐬���s(1)");

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = _device->CreateBlendState(&blendDesc, &_blendStateNone);
	if(FAILED(hr)) Debug.ErrorDialog("�u�����h�X�e���V���X�e�[�g�쐬���s(2)");

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = _device->CreateBlendState(&blendDesc, &_blendStateAdd);
	if(FAILED(hr)) Debug.ErrorDialog("�u�����h�X�e���V���X�e�[�g�쐬���s(3)");

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = _device->CreateBlendState(&blendDesc, &_blendStateSub);
	if(FAILED(hr)) Debug.ErrorDialog("�u�����h�X�e���V���X�e�[�g�쐬���s(4)");


	// ���X�^���C�U�X�e�[�g�쐬
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE;
	rd.DepthClipEnable = TRUE;
	rd.MultisampleEnable = FALSE;
	hr = _device->CreateRasterizerState(&rd, &_rasterStateCullNone);
	if(FAILED(hr)) Debug.ErrorDialog("���X�^���C�U�X�e�[�g�쐬���s(1)");

	rd.CullMode = D3D11_CULL_FRONT;
	hr = _device->CreateRasterizerState(&rd, &_rasterStateCullCW);
	if(FAILED(hr)) Debug.ErrorDialog("���X�^���C�U�X�e�[�g�쐬���s(2)");

	rd.CullMode = D3D11_CULL_BACK;
	hr = _device->CreateRasterizerState(&rd, &_rasterStateCullCCW);
	if(FAILED(hr)) Debug.ErrorDialog("���X�^���C�U�X�e�[�g�쐬���s(3)");

	rd.CullMode = D3D11_CULL_BACK;
	rd.FillMode = D3D11_FILL_WIREFRAME;
	hr = _device->CreateRasterizerState(&rd, &_rasterStateWireFrame);
	if(FAILED(hr)) Debug.ErrorDialog("���X�^���C�U�X�e�[�g�쐬���s(4)");


	// �Ƃ肠�����̕`��ݒ�
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
	_deviceContext->RSSetViewports(1, &_viewPort);


	return S_OK;
}


/*
@brief �I������
*/
void DirectX11Device::Uninit(void)
{
	if (_swapChain)
	{
		_swapChain->SetFullscreenState(FALSE, 0);
	}
}


/*
@brief ��ʃN���A
*/
void DirectX11Device::ClearRender(void)
{
	float clearColor[4] = { 41.0f / 255.0f, 128.0f / 255.0f, 185.0f / 255.0f, 1.0f };

	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), clearColor);
	_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}


/*
@brief ��ʃN���A�i�����_�[�^�[�Q�b�g�r���[�A�N���A�J���[�w��j
*/
void DirectX11Device::ClearRender(ID3D11RenderTargetView * renderTargetView)
{
	//float col[4] = { clearColor.x, clearColor.y, clearColor.z, clearColor.y };
	float clearColor[4] = { 41.0f / 255.0f, 128.0f / 255.0f, 185.0f / 255.0f, 1.0f };

	_deviceContext->ClearRenderTargetView(renderTargetView, clearColor);
	_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}


/*
@brief �[�x�o�b�t�@�N���A
*/
void DirectX11Device::ClearDepth(void)
{
	_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}


/*
@brief �t�����g�o�b�t�@�ƃo�b�N�o�b�t�@�̓���ւ�
*/
void DirectX11Device::Present(void)
{
	_swapChain.Get()->Present(0, 0);
}


/*
@brief �t���X�N���[���؂�ւ�
*/
void DirectX11Device::SetFullScreen(BOOL isFullScr)
{
	_swapChain->SetFullscreenState(isFullScr, 0);
}


/*
@brief �[�x�e�X�g�̗L���E�����؂�ւ�
*/
void DirectX11Device::SetDepthState(BOOL enable)
{
	if (enable)
		_deviceContext.Get()->OMSetDepthStencilState(_depthStateEnable.Get(), NULL);
	else
		_deviceContext.Get()->OMSetDepthStencilState(_depthStateDisable.Get(), NULL);
}


/*
@brief �u�����h���[�h�̐؂�ւ�
*/
void DirectX11Device::SetBlendState(BLEND_MODE blendMode)
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	_curBlendMode = blendMode;
	switch (_curBlendMode)
	{
	case DirectX11Device::BLEND_MODE_NONE:
		_deviceContext->OMSetBlendState(_blendStateNone.Get(), blendFactor, 0xffffffff);
		break;
	case DirectX11Device::BLEND_MODE_ALPHA:
		_deviceContext->OMSetBlendState(_blendStateAlpha.Get(), blendFactor, 0xffffffff);
		break;
	case DirectX11Device::BLEND_MODE_ADD:
		_deviceContext->OMSetBlendState(_blendStateAdd.Get(), blendFactor, 0xffffffff);
		break;
	case DirectX11Device::BLEND_MODE_SUBTRACT:
		_deviceContext->OMSetBlendState(_blendStateSub.Get(), blendFactor, 0xffffffff);
		break;
	default:
		break;
	}
}


/*
@brief ���e�X�g�̗L���E�����؂�ւ�
*/
void DirectX11Device::SetAlphaTest(BOOL enable)
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	blendDesc.AlphaToCoverageEnable = enable;

	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;

	switch (_curBlendMode)
	{
	case DirectX11Device::BLEND_MODE_NONE:
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	case DirectX11Device::BLEND_MODE_ALPHA:
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	case DirectX11Device::BLEND_MODE_ADD:
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	case DirectX11Device::BLEND_MODE_SUBTRACT:
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	}

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11BlendState* blendState = NULL;
	_device->CreateBlendState(&blendDesc, &blendState);
	_deviceContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);

	if (blendState != NULL) blendState->Release();

}


/*
@brief ���X�^���C�U�X�e�[�g�̐؂�ւ�
*/
void DirectX11Device::SetRasterizerState(CULL_MODE cullMode)
{
	switch (cullMode)
	{
	case DirectX11Device::CULL_MODE_NONE:
		_deviceContext.Get()->RSSetState(_rasterStateCullNone.Get());
		break;
	case DirectX11Device::CULL_MODE_FRONT:
		_deviceContext.Get()->RSSetState(_rasterStateCullCW.Get());
		break;
	case DirectX11Device::CULL_MODE_BACK:
		_deviceContext.Get()->RSSetState(_rasterStateCullCCW.Get());
		break;
	case DirectX11Device::CULL_MODE_WIRE_FRAME:
		_deviceContext.Get()->RSSetState(_rasterStateWireFrame.Get());
		break;
	default:
		break;
	}
}


/*
@brief �r���[�|�[�g�Z�b�g
*/
void DirectX11Device::SetViewPort(D3D11_VIEWPORT * vp)
{
	if (vp == NULL)
	{
		vp = &_viewPort;
	}

	_deviceContext->RSSetViewports(1, vp);
}


/*
@brief �����_�[�^�[�Q�b�g�r���[���Z�b�g�iNULL�Ńf�t�H���g��RTV���Z�b�g�j
*/
void DirectX11Device::SetRenderTarget(ID3D11RenderTargetView * renderTargetView, ID3D11DepthStencilView * depthStencilView)
{
	if (renderTargetView == NULL)
	{
		renderTargetView = _renderTargetView.Get();
	}
	if (depthStencilView == NULL)
	{
		depthStencilView = _depthStencilView.Get();
	}
	_deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
}


HRESULT CompileShaderFromFile(const TCHAR* filename, const char* entryPoint, const char* shaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(
		filename,		// �t�@�C����
		NULL,			// �}�N��
		NULL,			// �C���N���[�h�t�@�C��
		entryPoint,		// �G���g���|�C���g�֐���
		shaderModel,	// �V�F�[�_���f��
		dwShaderFlags,	// �V�F�[�_�R���p�C���t���O
		0,				// �G�t�F�N�g�R���p�C���t���O
		NULL,			// �X���b�h�|���v�C���^�[�t�F�[�X�ւ̃|�C���^
		ppBlobOut,
		&pErrorBlob,
		NULL);

	// �ǂݍ��݃G���[
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
		{
			Debug.ErrorDialog("�V�F�[�_�[�R���p�C���G���[�F%s\n\n%s", filename, (char*)pErrorBlob->GetBufferPointer());
		}
		else
		{
			Debug.ErrorDialog("�V�F�[�_�[�R���p�C���G���[�F%s", filename);
		}
	}

	if (pErrorBlob)
	{
		SAFE_RELEASE(pErrorBlob);
	}

	return hr;
}
