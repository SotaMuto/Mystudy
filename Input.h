#pragma once

/**
* @file Input.h
* @brief ���͏���
* @author GP12B183 12 �����y��
* @details
*/


/**
* @brief ���͏����N���X
*/
class InputSystem
{
private:
	static const int   KEY_MAX = 256;		// �L�[�̐�
	static const int   PAD_MAX = 4;			// �p�b�h�̍ő吔
	static const int   PAD_DEAD_ZONE;		// �p�b�h�̖����]�[��
	static const int   PAD_RANGE_MIN ;		// �p�b�h�̗L���͈͍ŏ��l
	static const int   PAD_RANGE_MAX;		// �p�b�h�̗L���͈͍ő�l
	static const int   STICK_R_MIN;			// �E�X�e�B�b�N�̍ŏ��l
	static const int   STICK_R_MAX;			// �E�X�e�B�b�N�̍ő�l
	static const float STICK_R_DEFAULT;		// �E�X�e�B�b�N�̃f�t�H���g�l
	static const float STICK_R_DEAD_ZONE;	// �E�X�e�B�b�N�̖����]�[��

	BOOL					_isLoad = FALSE;

	// win32�E�B���h�E�n���h��
	HWND					_hwnd = NULL;

	// �E�B���h�E�������̃N���C�A���g�̈�
	RECT					_rectOnInit;

	// IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^
	LPDIRECTINPUT8			_pDirectInput = NULL;	

	// �L�[�{�[�h
	LPDIRECTINPUTDEVICE8	_pKeyboard = NULL;
	BYTE					_keyState[KEY_MAX];
	BYTE					_keyStateTrigger[KEY_MAX];
	BYTE					_keyStateRepeat[KEY_MAX];
	BYTE					_keyStateRelease[KEY_MAX];
	BYTE					_keyStateRepeatCnt[KEY_MAX];

	// �}�E�X
	LPDIRECTINPUTDEVICE8	_pMouse = NULL;
	DIMOUSESTATE2			_mouseState;
	DIMOUSESTATE2			_mouseTrigger;
	DIMOUSESTATE2			_mouseRelease;

	// �Q�[���p�b�h
	LPDIRECTINPUTDEVICE8	_pGamePad[PAD_MAX] = { NULL,NULL,NULL,NULL };
	DWORD					_padState[PAD_MAX];
	DWORD					_padTrigger[PAD_MAX];
	DWORD					_padRelease[PAD_MAX];
	int						_padCnt;

	HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);
	void UninitKeyboard(void);
	HRESULT UpdateKeyboard(void);

	HRESULT InitMouse(HINSTANCE hInst, HWND hWnd);
	void UninitMouse();
	HRESULT UpdateMouse();

	HRESULT InitGamePad(HINSTANCE hInst, HWND hWnd);
	void UninitGamePad(void);
	HRESULT UpdateGamePad(void);


public:
	HRESULT Init(HINSTANCE hInst, HWND hWnd);
	void Uninit(void);
	void Update(void);

	BOOL GetKeyboardPress(int key) const;
	BOOL GetKeyboardTrigger(int key) const;
	BOOL GetKeyboardRepeat(int key) const;
	BOOL GetKeyboardRelease(int key) const;

	BOOL IsMouseLeftPressed(void) const;
	BOOL IsMouseLeftTriggered(void) const;
	BOOL IsMouseLeftReleased(void) const;
	BOOL IsMouseRightPressed(void) const;
	BOOL IsMouseRightTriggered(void) const;
	BOOL IsMouseRightReleased(void) const;
	BOOL IsMouseCenterPressed(void) const;
	BOOL IsMouseCenterTriggered(void) const;
	BOOL IsMouseCenterReleased(void) const;
	long GetMouseX(void) const;				// �}�E�X��X�����ɓ��������Βl
	long GetMouseY(void) const;				// �}�E�X��Y�����ɓ��������Βl
	long GetMouseZ(void) const;				// �}�E�X�z�C�[�������������Βl
	XMFLOAT2 GetMousePos(void) const;
	XMFLOAT2 NormalizeClient(const XMFLOAT2& pos) const;
	XMFLOAT2 GetCurClientRaio(void) const;			// �E�B���h�E�������̃N���C�A���g�̈�ƌ��݂̃N���C�A���g�̈�̔��Ԃ�

	BOOL IsButtonPressed(int padNo, DWORD button);
	BOOL IsButtonTriggered(int padNo, DWORD button);
	BOOL IsButtonReleased(int padNo, DWORD button);
	BOOL IsPadConnected(void);

	LPDIRECTINPUT8 GetDevice(void) const { return _pDirectInput; }
	LPDIRECTINPUTDEVICE8* GetGamePad(int i) { return &_pGamePad[i]; }
	int GetPadCnt(void) const { return _padCnt; }
	void SetPadCnt(int padCnt) { _padCnt = padCnt; }
};

// �v���O������������Ƃ��Ɏg��
#define	USE_KEYBOARD				// �錾����ƃL�[�{�[�h�ő���\�ɂȂ�
#define	USE_MOUSE					// �錾����ƃ}�E�X�ő���\�ɂȂ�
#define	USE_PAD						// �錾����ƃp�b�h�ő���\�ɂȂ�

#define LEFT_STICK_UP		0x00000001l	// ���X�e�B�b�N��(.IY<0)
#define LEFT_STICK_DOWN		0x00000002l	// ���X�e�B�b�N��(.IY>0)
#define LEFT_STICK_LEFT		0x00000004l	// ���X�e�B�b�N��(.IX<0)
#define LEFT_STICK_RIGHT	0x00000008l	// ���X�e�B�b�N�E(.IX>0)

//#define BUTTON_A			0x00000010l	// �`�{�^��(.rgbButtons[0]&0x80)
//#define BUTTON_B			0x00000020l	// �a�{�^��(.rgbButtons[1]&0x80)
//#define BUTTON_C			0x00000040l	// �b�{�^��(.rgbButtons[2]&0x80)
//#define BUTTON_X			0x00000080l	// �w�{�^��(.rgbButtons[3]&0x80)
//#define BUTTON_Y			0x00000100l	// �x�{�^��(.rgbButtons[4]&0x80)
//#define BUTTON_Z			0x00000200l	// �y�{�^��(.rgbButtons[5]&0x80)
//#define BUTTON_L			0x00000400l	// �k�{�^��(.rgbButtons[6]&0x80)
//#define BUTTON_R			0x00000800l	// �q�{�^��(.rgbButtons[7]&0x80)
//#define BUTTON_START		0x00001000l	// �r�s�`�q�s�{�^��(.rgbButtons[8]&0x80)
//#define BUTTON_M			0x00002000l	// �l�{�^��(.rgbButtons[9]&0x80)

#define BUTTON_X			0x00000010l	// �w�{�^��(.rgbButtons[0]&0x80)	�l�p
#define BUTTON_A			0x00000020l	// �`�{�^��(.rgbButtons[1]&0x80)	�o�c
#define BUTTON_B			0x00000040l	// �a�{�^��(.rgbButtons[2]&0x80)	��
#define BUTTON_Y			0x00000080l	// �x�{�^��(.rgbButtons[3]&0x80)	�O�p
#define BUTTON_L			0x00000100l	// �k�{�^��(.rgbButtons[4]&0x80)
#define BUTTON_R			0x00000200l	// �q�{�^��(.rgbButtons[5]&0x80)
#define BUTTON_L2			0x00000400l	// �k�{�^��(.rgbButtons[6]&0x80)
#define BUTTON_R2			0x00000800l	// �q�{�^��(.rgbButtons[7]&0x80)
#define BUTTON_SELECT		0x00001000l	// �y�{�^��(.rgbButtons[8]&0x80)
#define BUTTON_START		0x00002000l	// �r�s�`�q�s�{�^��(.rgbButtons[9]&0x80)
#define BUTTON_BACK			0x00004000l	// �y�{�^��(.rgbButtons[10]&0x80)
#define BUTTON_OPTION		0x00008000l	// OPTION�{�^��(.rgbButtons[11]&0x80)

#define RIGHT_STICK_UP		0x00010000l	// �E�X�e�B�b�N��
#define RIGHT_STICK_DOWN	0x00020000l	// �E�X�e�B�b�N��
#define RIGHT_STICK_LEFT	0x00040000l	// �E�X�e�B�b�N��
#define RIGHT_STICK_RIGHT	0x00080000l	// �E�X�e�B�b�N�E

#define CROSS_BUTTON_UP		0x00100000l	// �����L�[��{�^��(.rgdwPOV[0] == 0)
#define CROSS_BUTTON_LEFT	0x00200000l	// �����L�[���{�^��(.rgdwPOV[0] == 27000)
#define CROSS_BUTTON_RIGHT	0x00400000l	// �����L�[�E�{�^��(.rgdwPOV[0] == 9000)
#define CROSS_BUTTON_DOWN	0x00800000l	// �����L�[���{�^��(.rgdwPOV[0] == 18000)

enum // �{�^���ɑ΂���rgbButtons[?]�z��̔z��ԍ�(DirectInput��ł�XYAB�{�^���ʒu�ɍ��킹��)
{	
	rgbButtons_X,
	rgbButtons_A,
	rgbButtons_B,
	rgbButtons_Y,
	rgbButtons_L,
	rgbButtons_R,
	rgbButtons_L2,
	rgbButtons_R2,
	rgbButtons_SELECT,
	rgbButtons_START,
};

