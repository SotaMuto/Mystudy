#pragma once

/**
* @file Input.h
* @brief 入力処理
* @author GP12B183 12 小峰遥一
* @details
*/


/**
* @brief 入力処理クラス
*/
class InputSystem
{
private:
	static const int   KEY_MAX = 256;		// キーの数
	static const int   PAD_MAX = 4;			// パッドの最大数
	static const int   PAD_DEAD_ZONE;		// パッドの無効ゾーン
	static const int   PAD_RANGE_MIN ;		// パッドの有効範囲最小値
	static const int   PAD_RANGE_MAX;		// パッドの有効範囲最大値
	static const int   STICK_R_MIN;			// 右スティックの最小値
	static const int   STICK_R_MAX;			// 右スティックの最大値
	static const float STICK_R_DEFAULT;		// 右スティックのデフォルト値
	static const float STICK_R_DEAD_ZONE;	// 右スティックの無効ゾーン

	BOOL					_isLoad = FALSE;

	// win32ウィンドウハンドル
	HWND					_hwnd = NULL;

	// ウィンドウ生成時のクライアント領域
	RECT					_rectOnInit;

	// IDirectInput8インターフェースへのポインタ
	LPDIRECTINPUT8			_pDirectInput = NULL;	

	// キーボード
	LPDIRECTINPUTDEVICE8	_pKeyboard = NULL;
	BYTE					_keyState[KEY_MAX];
	BYTE					_keyStateTrigger[KEY_MAX];
	BYTE					_keyStateRepeat[KEY_MAX];
	BYTE					_keyStateRelease[KEY_MAX];
	BYTE					_keyStateRepeatCnt[KEY_MAX];

	// マウス
	LPDIRECTINPUTDEVICE8	_pMouse = NULL;
	DIMOUSESTATE2			_mouseState;
	DIMOUSESTATE2			_mouseTrigger;
	DIMOUSESTATE2			_mouseRelease;

	// ゲームパッド
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
	long GetMouseX(void) const;				// マウスがX方向に動いた相対値
	long GetMouseY(void) const;				// マウスがY方向に動いた相対値
	long GetMouseZ(void) const;				// マウスホイールが動いた相対値
	XMFLOAT2 GetMousePos(void) const;
	XMFLOAT2 NormalizeClient(const XMFLOAT2& pos) const;
	XMFLOAT2 GetCurClientRaio(void) const;			// ウィンドウ生成時のクライアント領域と現在のクライアント領域の比を返す

	BOOL IsButtonPressed(int padNo, DWORD button);
	BOOL IsButtonTriggered(int padNo, DWORD button);
	BOOL IsButtonReleased(int padNo, DWORD button);
	BOOL IsPadConnected(void);

	LPDIRECTINPUT8 GetDevice(void) const { return _pDirectInput; }
	LPDIRECTINPUTDEVICE8* GetGamePad(int i) { return &_pGamePad[i]; }
	int GetPadCnt(void) const { return _padCnt; }
	void SetPadCnt(int padCnt) { _padCnt = padCnt; }
};

// プログラム分けするときに使う
#define	USE_KEYBOARD				// 宣言するとキーボードで操作可能になる
#define	USE_MOUSE					// 宣言するとマウスで操作可能になる
#define	USE_PAD						// 宣言するとパッドで操作可能になる

#define LEFT_STICK_UP		0x00000001l	// 左スティック上(.IY<0)
#define LEFT_STICK_DOWN		0x00000002l	// 左スティック下(.IY>0)
#define LEFT_STICK_LEFT		0x00000004l	// 左スティック左(.IX<0)
#define LEFT_STICK_RIGHT	0x00000008l	// 左スティック右(.IX>0)

//#define BUTTON_A			0x00000010l	// Ａボタン(.rgbButtons[0]&0x80)
//#define BUTTON_B			0x00000020l	// Ｂボタン(.rgbButtons[1]&0x80)
//#define BUTTON_C			0x00000040l	// Ｃボタン(.rgbButtons[2]&0x80)
//#define BUTTON_X			0x00000080l	// Ｘボタン(.rgbButtons[3]&0x80)
//#define BUTTON_Y			0x00000100l	// Ｙボタン(.rgbButtons[4]&0x80)
//#define BUTTON_Z			0x00000200l	// Ｚボタン(.rgbButtons[5]&0x80)
//#define BUTTON_L			0x00000400l	// Ｌボタン(.rgbButtons[6]&0x80)
//#define BUTTON_R			0x00000800l	// Ｒボタン(.rgbButtons[7]&0x80)
//#define BUTTON_START		0x00001000l	// ＳＴＡＲＴボタン(.rgbButtons[8]&0x80)
//#define BUTTON_M			0x00002000l	// Ｍボタン(.rgbButtons[9]&0x80)

#define BUTTON_X			0x00000010l	// Ｘボタン(.rgbButtons[0]&0x80)	四角
#define BUTTON_A			0x00000020l	// Ａボタン(.rgbButtons[1]&0x80)	バツ
#define BUTTON_B			0x00000040l	// Ｂボタン(.rgbButtons[2]&0x80)	丸
#define BUTTON_Y			0x00000080l	// Ｙボタン(.rgbButtons[3]&0x80)	三角
#define BUTTON_L			0x00000100l	// Ｌボタン(.rgbButtons[4]&0x80)
#define BUTTON_R			0x00000200l	// Ｒボタン(.rgbButtons[5]&0x80)
#define BUTTON_L2			0x00000400l	// Ｌボタン(.rgbButtons[6]&0x80)
#define BUTTON_R2			0x00000800l	// Ｒボタン(.rgbButtons[7]&0x80)
#define BUTTON_SELECT		0x00001000l	// Ｚボタン(.rgbButtons[8]&0x80)
#define BUTTON_START		0x00002000l	// ＳＴＡＲＴボタン(.rgbButtons[9]&0x80)
#define BUTTON_BACK			0x00004000l	// Ｚボタン(.rgbButtons[10]&0x80)
#define BUTTON_OPTION		0x00008000l	// OPTIONボタン(.rgbButtons[11]&0x80)

#define RIGHT_STICK_UP		0x00010000l	// 右スティック上
#define RIGHT_STICK_DOWN	0x00020000l	// 右スティック下
#define RIGHT_STICK_LEFT	0x00040000l	// 右スティック左
#define RIGHT_STICK_RIGHT	0x00080000l	// 右スティック右

#define CROSS_BUTTON_UP		0x00100000l	// 方向キー上ボタン(.rgdwPOV[0] == 0)
#define CROSS_BUTTON_LEFT	0x00200000l	// 方向キー左ボタン(.rgdwPOV[0] == 27000)
#define CROSS_BUTTON_RIGHT	0x00400000l	// 方向キー右ボタン(.rgdwPOV[0] == 9000)
#define CROSS_BUTTON_DOWN	0x00800000l	// 方向キー下ボタン(.rgdwPOV[0] == 18000)

enum // ボタンに対するrgbButtons[?]配列の配列番号(DirectInput上でのXYABボタン位置に合わせた)
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

