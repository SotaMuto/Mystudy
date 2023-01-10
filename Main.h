#pragma once

/**
* @file Main.h
* @brief ���C������
* @author GP12B183 12 �����y��
* @details
*/

// �{���̓w�b�_�ɏ����Ȃ������ǂ�
using namespace DirectX;


#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")

#define SCREEN_WIDTH		(1280)					// �E�C���h�E�̕�
#define SCREEN_HEIGHT		( 720)					// �E�C���h�E�̍���
#define FRAME_RATE			(  60)					// �t���[�����[�g
#define SCREEN_CENTER_X		(SCREEN_WIDTH  * 0.5f)	// �E�C���h�E�̒��S�w���W
#define SCREEN_CENTER_Y		(SCREEN_HEIGHT * 0.5f)	// �E�C���h�E�̒��S�x���W
#define DEBUG_DISP_TEXTOUT							// �f�o�b�O�p��ʃe�L�X�g�o�͂�L���ɂ���

HWND GetHWND(void);