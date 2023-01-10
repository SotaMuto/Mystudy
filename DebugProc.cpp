#include "stdafx.h"

/**
* @file DebugProc.cpp
* @brief �f�o�b�O����
* @author GP12B183 12 �����y��
* @details
*/


// �V���O���g���̃C���X�^���X�錾
DebugProc* DebugProc::_instance = nullptr;

// �f�o�b�O�\���ӏ�
const RECT		DebugProc::DEBUG_PRINT_RECT = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
// �f�o�b�O�����F
const COLORREF	DebugProc::DEBUG_PRINT_COLOR = RGB(255, 255, 255);


/**
* @brief ���C������
*/
void DebugProc::Out(const char * str, ...)
{
#ifndef _DEBUG
	return;
#endif // !_DEBUG

	char buff[DEBUG_STR_MAX] = { "\0" };

	// �ψ�������ϐ������o��
	va_list ap;
	va_start(ap, str);

	if (!vsprintf_s(buff, DEBUG_STR_MAX, str, ap)) {

		OutputDebugStringA("dprintf error.");
	}
	va_end(ap);

	OutputDebugStringA(buff);
}


/**
* @brief ��ʍ���Ƀf�o�b�O����\��
*/
void DebugProc::Print(const char * str, ...)
{
#ifndef _DEBUG
	return;
#endif // !_DEBUG

	char buff[DEBUG_STR_MAX] = { "\0" };

	// �ψ�������ϐ������o��
	va_list ap;
	va_start(ap, str);

	if (!vsprintf_s(buff, DEBUG_STR_MAX, str, ap)) {

		OutputDebugStringA("dprintf error.");
	}
	va_end(ap);


	// �A���i�`�揈���ł܂Ƃ߂ĕ`�悷�邽�߁j
	if ((strlen(_printBuff) + strlen(buff)) < ((sizeof _printBuff) - 1))
	{
		strcat_s(_printBuff, buff);
	}
}


/**
* @brief �G���[�_�C�A���O��\��
*/
void DebugProc::ErrorDialog(const char * str, ...)
{
#ifndef _DEBUG
	return;
#endif // !_DEBUG

	char buff[DEBUG_STR_MAX] = { "\0" };

	// �ψ�������ϐ������o��
	va_list ap;
	va_start(ap, str);

	if (!vsprintf_s(buff, DEBUG_STR_MAX, str, ap)) {

		OutputDebugStringA("dprintf error.");
	}
	va_end(ap);

	// ���b�Z�[�W�{�b�N�X�\��
	MessageBox(NULL, buff, "ERROR", MB_ICONWARNING);
	PostQuitMessage(0);	// �I�����b�Z�[�W�𑗂�
}


/**
* @brief ���O�t�@�C�����o��
*/
void DebugProc::ExportFile(const char * fileName, const char * str, ...)
{
#ifndef _DEBUG
	return;
#endif // !_DEBUG

	char buff[DEBUG_STR_MAX] = { "\0" };

	// �ψ�������ϐ������o��
	va_list ap;
	va_start(ap, str);

	if (!vsprintf_s(buff, DEBUG_STR_MAX, str, ap)) {

		OutputDebugStringA("dprintf error.");
	}
	va_end(ap);

	// �t�@�C���ɏ㏑�����[�h�ŏ�������
	FILE *fp = fopen(fileName, "a");
	if (fp == NULL) return;
	fprintf(fp, buff);
	fclose(fp);
}


/**
* @brief �`�揈��
*/
void DebugProc::Draw(void)
{
#ifndef _DEBUG
	return;
#endif // !_DEBUG

#ifndef DEBUG_DISP_TEXTOUT
	return;
#endif // !DEBUG_DISP_TEXTOUT

	HRESULT hr;

	// �o�b�N�o�b�t�@����T�[�t�F�X���擾����
	IDXGISurface1* pBackSurface = NULL;
	hr = DX11.GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface1), (void**)&pBackSurface);
	if (hr != S_OK) return;

	// �擾�����T�[�t�F�X����f�o�C�X�R���e�L�X�g���擾����
	HDC hdc;
	hr = pBackSurface->GetDC(FALSE, &hdc);
	if (hr != S_OK) return;

	// �����F��ύX
	SetTextColor(hdc, DEBUG_PRINT_COLOR);

	// �w�i�𓧖��ɕύX
	SetBkMode(hdc, TRANSPARENT);

	//�e�L�X�g�o��
	RECT rect = DEBUG_PRINT_RECT;
	DrawText(hdc, _printBuff, (int)strlen(_printBuff), &rect, DT_LEFT);

	//�f�o�C�X�R���e�L�X�g���������
	pBackSurface->ReleaseDC(NULL);

	//�T�[�t�F�X���������
	pBackSurface->Release();

	//�����_�����O�^�[�Q�b�g�����Z�b�g�����̂ŃZ�b�g���Ȃ���
	ID3D11RenderTargetView *rtv = DX11.GetDefaultRenderTargetView();
	DX11.GetDeviceContext()->OMSetRenderTargets(1, &rtv, nullptr);

	// �o�b�t�@���Z�b�g
	memset(_printBuff, 0, sizeof(_printBuff));
}