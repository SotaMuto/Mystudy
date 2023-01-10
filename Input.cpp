#include "stdafx.h"



const int   InputSystem::PAD_DEAD_ZONE =  2500;		// パッドの無効ゾーン
const int   InputSystem::PAD_RANGE_MIN =  1000;		// パッドの有効範囲最小値
const int   InputSystem::PAD_RANGE_MAX = -1000;		// パッドの有効範囲最大値
const int   InputSystem::STICK_R_MIN   =  0;		// 右スティックの最小値
const int   InputSystem::STICK_R_MAX   =  65535;	// 右スティックの最大値
const float InputSystem::STICK_R_DEFAULT   = (STICK_R_MAX - STICK_R_MIN) * 0.5;		// 右スティックのデフォルト値
const float InputSystem::STICK_R_DEAD_ZONE = (STICK_R_MAX - STICK_R_MIN) * 0.125;	// 右スティックの無効ゾーン

/**
* @brief 入力システムの初期化処理
*/
HRESULT InputSystem::Init(HINSTANCE hInst, HWND hWnd)
{
	if (_isLoad) return S_OK;

	_hwnd = hWnd;

	HRESULT hr = S_OK;

	// DirectInputオブジェクトの作成
	hr = DirectInput8Create(hInst,
		DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&_pDirectInput, NULL);

	// キーボードの初期化
	InitKeyboard(hInst, hWnd);

	// マウスの初期化
	InitMouse(hInst, hWnd);

	// ゲームパッドの初期化
	InitGamePad(hInst, hWnd);

	// ウィンドウ生成時のクライアント領域を保存しておく
	GetClientRect(_hwnd, &_rectOnInit);

	_isLoad = TRUE;

	return hr;
}


/**
* @brief 入力システムの終了処理
*/
void InputSystem::Uninit(void)
{
	if (!_isLoad) return;

	// ゲームパッドの終了処理
	UninitGamePad();

	// マウスの終了処理
	UninitMouse();
	
	// キーボードの終了処理
	UninitKeyboard();

	if (_pDirectInput)
	{
		_pDirectInput->Release();
		_pDirectInput = NULL;
	}

	_isLoad = FALSE;
}


/**
* @brief 入力システムの更新処理
*/
void InputSystem::Update(void)
{
	// キーボードの更新処理
	UpdateKeyboard();

	// マウスの更新処理
	UpdateMouse();

	// ゲームパッドの更新処理
	UpdateGamePad();
}


#pragma region Keyboard

/**
* @brief キーボードの初期化処理
*/
HRESULT InputSystem::InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// デバイスオブジェクトを作成
	hr = _pDirectInput->CreateDevice(GUID_SysKeyboard, &_pKeyboard, NULL);
	if (FAILED(hr) || _pKeyboard == NULL)
	{
		MessageBox(hWnd, "No Keyboard", "Warning", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = _pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Can't setup keyboard", "Warning", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = _pKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Keyboard mode error", "警告", MB_ICONWARNING);
		return hr;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	_pKeyboard->Acquire();

	return hr;
}


/**
* @brief キーボードの終了処理
*/
void InputSystem::UninitKeyboard(void)
{
	if (_pKeyboard)
	{
		_pKeyboard->Unacquire();
		_pKeyboard->Release();
		_pKeyboard = NULL;
	}
}


/**
* @brief キーボードの更新処理
*/
HRESULT InputSystem::UpdateKeyboard(void)
{
	HRESULT hr;
	BYTE keyStateOld[256];

	// 前回のデータを保存
	memcpy(keyStateOld, _keyState, KEY_MAX);

	// デバイスからデータを取得
	hr = _pKeyboard->GetDeviceState(sizeof(_keyState), _keyState);
	if (SUCCEEDED(hr))
	{
		for (int cnt = 0; cnt < KEY_MAX; cnt++)
		{
			_keyStateTrigger[cnt] = (keyStateOld[cnt] ^ _keyState[cnt]) & _keyState[cnt];
			_keyStateRelease[cnt] = (keyStateOld[cnt] ^ _keyState[cnt]) & ~_keyState[cnt];
			_keyStateRepeat[cnt] = _keyStateTrigger[cnt];

			if (_keyState[cnt])
			{
				_keyStateRepeatCnt[cnt]++;
				if (_keyStateRepeatCnt[cnt] >= 20)
				{
					_keyStateRepeat[cnt] = _keyState[cnt];
				}
			}
			else
			{
				_keyStateRepeatCnt[cnt] = 0;
				_keyStateRepeat[cnt] = 0;
			}
		}
	}
	else
	{
		// キーボードへのアクセス権を取得
		_pKeyboard->Acquire();
	}

	return S_OK;
}

/**
* @brief キーボードのプレス状態を取得
*/
BOOL InputSystem::GetKeyboardPress(int key) const
{
	return (_keyState[key] & 0x80) ? TRUE : FALSE;
}

/**
* @brief キーボードのトリガー状態を取得
*/
BOOL InputSystem::GetKeyboardTrigger(int key) const
{
	if (key == -1) return FALSE;

	return (_keyStateTrigger[key] & 0x80) ? TRUE : FALSE;
}


/**
* @brief キーボードのリピート状態を取得
*/
BOOL InputSystem::GetKeyboardRepeat(int key) const
{
	if (key == -1) return FALSE;

	return (_keyStateRepeat[key] & 0x80) ? TRUE : FALSE;
}


/**
* @brief キーボードのリリース状態を取得
*/
BOOL InputSystem::GetKeyboardRelease(int key) const
{
	if (key == -1) return FALSE;

	return (_keyStateRelease[key] & 0x80) ? TRUE : FALSE;
}


#pragma endregion


#pragma region Mouse
/**
* @brief マウスの初期化処理
*/
HRESULT InputSystem::InitMouse(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// デバイスオブジェクトを作成
	hr = _pDirectInput->CreateDevice(GUID_SysMouse, &_pMouse, NULL);
	if (FAILED(hr) || _pMouse == NULL)
	{
		MessageBox(hWnd, "No mouse", "Warning", MB_ICONWARNING);
		return hr;
	}
	// データフォーマットを設定
	hr = _pMouse->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Can't setup mouse", "Warning", MB_ICONWARNING);
		return hr;
	}
	// 他のアプリと協調モードに設定
	hr = _pMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Mouse mode error", "Warning", MB_ICONWARNING);
		return hr;
	}

	// デバイスの設定
	DIPROPDWORD prop;
	prop.diph.dwSize = sizeof(prop);
	prop.diph.dwHeaderSize = sizeof(prop.diph);
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;	// マウスの移動値　相対値

	hr = _pMouse->SetProperty(DIPROP_AXISMODE, &prop.diph);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Mouse property error", "Warning", MB_ICONWARNING);
		return hr;
	}

	// アクセス権を得る
	_pMouse->Acquire();

	return hr;
}


/**
* @brief マウスの終了処理
*/
void InputSystem::UninitMouse()
{
	if (_pMouse)
	{
		_pMouse->Unacquire();
		_pMouse->Release();
		_pMouse = NULL;
	}
}


/**
* @brief マウスの更新処理
*/
HRESULT InputSystem::UpdateMouse()
{
	HRESULT hr;
	// 前回の値保存
	DIMOUSESTATE2 lastMouseState = _mouseState;
	// データ取得
	hr = _pMouse->GetDeviceState(sizeof(_mouseState), &_mouseState);
	if (SUCCEEDED(hr))
	{
		_mouseTrigger.lX = _mouseState.lX;
		_mouseTrigger.lY = _mouseState.lY;
		_mouseTrigger.lZ = _mouseState.lZ;
		// マウスのボタン状態
		for (int i = 0; i < 8; i++)
		{
			_mouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
				_mouseState.rgbButtons[i]) & _mouseState.rgbButtons[i]);

			_mouseRelease.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
				_mouseState.rgbButtons[i]) & lastMouseState.rgbButtons[i]);
		}
	}
	else	// 取得失敗
	{
		// アクセス権を得てみる
		hr = _pMouse->Acquire();
	}

	return hr;
}


BOOL InputSystem::IsMouseLeftPressed(void) const
{
	return (BOOL)(_mouseState.rgbButtons[0] & 0x80);
}
BOOL InputSystem::IsMouseLeftTriggered(void) const
{
	return (BOOL)(_mouseTrigger.rgbButtons[0] & 0x80);
}
BOOL InputSystem::IsMouseLeftReleased(void) const
{
	return (BOOL)(_mouseRelease.rgbButtons[0] & 0x80);
}
BOOL InputSystem::IsMouseRightPressed(void) const
{
	return (BOOL)(_mouseState.rgbButtons[1] & 0x80);
}
BOOL InputSystem::IsMouseRightTriggered(void) const
{
	return (BOOL)(_mouseTrigger.rgbButtons[1] & 0x80);
}
BOOL InputSystem::IsMouseRightReleased(void) const
{
	return (BOOL)(_mouseRelease.rgbButtons[1] & 0x80);
}
BOOL InputSystem::IsMouseCenterPressed(void) const
{
	return (BOOL)(_mouseState.rgbButtons[2] & 0x80);
}
BOOL InputSystem::IsMouseCenterTriggered(void) const
{
	return (BOOL)(_mouseTrigger.rgbButtons[2] & 0x80);
}
BOOL InputSystem::IsMouseCenterReleased(void) const
{
	return (BOOL)(_mouseRelease.rgbButtons[2] & 0x80);
}
long InputSystem::GetMouseX(void) const
{
	return _mouseState.lX;
}
long InputSystem::GetMouseY(void) const
{
	return _mouseState.lY;
}
long InputSystem::GetMouseZ(void) const
{
	return _mouseState.lZ;
}

XMFLOAT2 InputSystem::GetMousePos(void) const
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(_hwnd, &p);

	return XMFLOAT2((float)p.x, (float)p.y);
}

XMFLOAT2 InputSystem::NormalizeClient(const XMFLOAT2 & pos) const
{
	RECT rect;
	GetClientRect(_hwnd, &rect);

	//Debug.Print("lt(%f, %f)\n", (float)rect.left, (float)rect.top);
	//Debug.Print("rb(%f, %f)\n", (float)rect.right, (float)rect.bottom);

	float w = (float)(rect.right - rect.left);
	float h = (float)(rect.bottom - rect.top);

	float x = Math::Clamp(pos.x / w, 0.0f, 1.0f);
	float y = Math::Clamp(pos.y / h, 0.0f, 1.0f);

	return XMFLOAT2(x, y);
}

XMFLOAT2 InputSystem::GetCurClientRaio(void) const
{
	RECT rect;
	GetClientRect(_hwnd, &rect);

	return XMFLOAT2((float)(rect.right / _rectOnInit.right), (float)(rect.bottom / _rectOnInit.bottom));
}


#pragma endregion


#pragma region GamePad

BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID)
{
	InputSystem* input = Game.GetInputSystem();
	int padCnt = input->GetPadCnt();
	input->GetDevice()->CreateDevice(lpddi->guidInstance, input->GetGamePad(padCnt), NULL);

	padCnt++;
	input->SetPadCnt(padCnt);

	return DIENUM_CONTINUE;	// 次のデバイスを列挙
}

BOOL CALLBACK DIEnumEffectsProc(LPCDIEFFECTINFO pei, LPVOID pv)
{
	*((GUID *)pv) = pei->guid;
	return DIENUM_STOP;  // one is enough
}

/**
* @brief ゲームパッドの初期化処理
*/
HRESULT InputSystem::InitGamePad(HINSTANCE hInst, HWND hWnd)
{
	HRESULT		hr;

	_padCnt = 0;

	// ジョイパッドを探す
	// セットしたコールバック関数が、パッドを発見した数だけ呼ばれる。
	_pDirectInput->EnumDevices(DI8DEVCLASS_GAMECTRL,
		(LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY/**DIEDFL_FORCEFEEDBACK*/);

	for (int i = 0; i < _padCnt; i++)
	{
		// ジョイスティック用のデータ・フォーマットを設定
		hr = _pGamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(hr))
			return hr; // データフォーマットの設定に失敗

		// モードを設定（フォアグラウンド＆非排他モード）
		hr = _pGamePad[i]->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
		if (FAILED(hr))
			return hr; // モードの設定に失敗

		// 軸の値の範囲を設定
		// X軸、Y軸のそれぞれについて、オブジェクトが報告可能な値の範囲をセットする。
		// (max-min)は、最大10,000(?)。(max-min)/2が中央値になる。
		// 差を大きくすれば、アナログ値の細かな動きを捕らえられる。(パッドの性能による)
		DIPROPRANGE diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = PAD_RANGE_MIN;
		diprg.lMax = PAD_RANGE_MAX;
		// X軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_X;
		_pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_Y;
		_pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 各軸ごとに、無効のゾーン値を設定する。
		// 無効ゾーンとは、中央からの微少なジョイスティックの動きを無視する範囲のこと。
		// 指定する値は、10000に対する相対値(2000なら20パーセント)。
		DIPROPDWORD dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = PAD_DEAD_ZONE;
		//X軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_X;
		_pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		//Y軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_Y;
		_pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);


		// オートセンター機能をオフにする
		// オートセンター機能とはモーターを使ってジョイスティックのばねをシュミレーションする機能
		// 他の機能を邪魔しないようにオフにすると良いらしい
		DIPROPDWORD dipac;
		dipac.diph.dwSize = sizeof(DIPROPDWORD);
		dipac.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipac.diph.dwObj = 0;
		dipac.diph.dwHow = DIPH_DEVICE;
		dipac.dwData = DIPROPAUTOCENTER_OFF;
		_pGamePad[i]->SetProperty(DIPROP_AUTOCENTER, &dipac.diph);

		// サポートされているエフェクトを列挙する
		HRESULT  hr;
		GUID     guidEffect;
		hr = _pGamePad[i]->EnumEffects(
			(LPDIENUMEFFECTSCALLBACK)DIEnumEffectsProc,
			&guidEffect,
			DIEFT_PERIODIC);
		if (FAILED(hr))
		{
			// Note that success does not mean that any effects were found,
			// only that the process went smoothly.
		}

		DWORD      dwAxes[2] = { DIJOFS_X, DIJOFS_Y };
		LONG       lDirection[2] = { 0, 0 };

		DIPERIODIC diPeriodic;      // type-specific parameters
		DIENVELOPE diEnvelope;      // envelope
		DIEFFECT   diEffect;        // general parameters

		diPeriodic.dwMagnitude = DI_FFNOMINALMAX;
		diPeriodic.lOffset = 0;
		diPeriodic.dwPhase = 0;
		diPeriodic.dwPeriod = (DWORD)(0.05 * DI_SECONDS);

		diEnvelope.dwSize = sizeof(DIENVELOPE);
		diEnvelope.dwAttackLevel = 0;
		diEnvelope.dwAttackTime = (DWORD)(0.5 * DI_SECONDS);
		diEnvelope.dwFadeLevel = 0;
		diEnvelope.dwFadeTime = (DWORD)(1.0 * DI_SECONDS);

		diEffect.dwSize = sizeof(DIEFFECT);
		diEffect.dwFlags = DIEFF_POLAR | DIEFF_OBJECTOFFSETS;
		diEffect.dwDuration = (DWORD)(2 * DI_SECONDS);

		diEffect.dwSamplePeriod = 0;               // = default 
		diEffect.dwGain = DI_FFNOMINALMAX;         // no scaling
		diEffect.dwTriggerButton = DIJOFS_BUTTON0;
		diEffect.dwTriggerRepeatInterval = 0;
		diEffect.cAxes = 2;
		diEffect.rgdwAxes = dwAxes;
		diEffect.rglDirection = &lDirection[0];
		diEffect.lpEnvelope = &diEnvelope;
		diEffect.cbTypeSpecificParams = sizeof(diPeriodic);
		diEffect.lpvTypeSpecificParams = &diPeriodic;

		LPDIRECTINPUTEFFECT  g_lpdiEffect;  // global effect object

		_pGamePad[i]->CreateEffect(
			guidEffect,     // GUID from enumeration
			&diEffect,      // where the data is
			&g_lpdiEffect,  // where to put interface pointer
			NULL);          // no aggregation
		if (FAILED(hr))
		{
			return hr;
		}

		//ジョイスティック入力制御開始
		_pGamePad[i]->Acquire();
	}

	return TRUE;
}


/**
* @brief ゲームパッドの終了処理
*/
void InputSystem::UninitGamePad(void)
{
	for (int i = 0; i < PAD_MAX; i++)
	{
		if (_pGamePad[i])
		{
			_pGamePad[i]->Unacquire();
			_pGamePad[i]->Release();
			_pGamePad[i] = NULL;
		}
	}
}


/**
* @brief ゲームパッドの更新処理
*/
HRESULT InputSystem::UpdateGamePad(void)
{
	HRESULT			hr;
	DIJOYSTATE2		dijs;
	int				i;

	for (i = 0; i < _padCnt; i++)
	{
		DWORD lastPadState;
		lastPadState = _padState[i];
		_padState[i] = 0x00000000l;	// 初期化

		hr = _pGamePad[i]->Poll();	// ジョイスティックにポールをかける
		if (FAILED(hr)) {
			hr = _pGamePad[i]->Acquire();
			while (hr == DIERR_INPUTLOST)
				hr = _pGamePad[i]->Acquire();
		}

		hr = _pGamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// デバイス状態を読み取る
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) {
			hr = _pGamePad[i]->Acquire();
			while (hr == DIERR_INPUTLOST)
				hr = _pGamePad[i]->Acquire();
		}


		// ３２の各ビットに意味を持たせ、ボタン押下に応じてビットをオンにする
		//** y-axis (forward)
		if (dijs.lY < 0)					_padState[i] |= LEFT_STICK_UP;
		//** y-axis (backward)
		if (dijs.lY > 0)					_padState[i] |= LEFT_STICK_DOWN;
		//** x-axis (left)
		if (dijs.lX < 0)					_padState[i] |= LEFT_STICK_LEFT;
		//** x-axis (right)
		if (dijs.lX > 0)					_padState[i] |= LEFT_STICK_RIGHT;

		// 右スティック(lZとlRzが対応しているらしい)(それぞれ0~65535の値をとる、65535/2が触ってない状態)
		// 左スティックと同様、小さい倒れ方は無効にする
		if (dijs.lZ < STICK_R_DEFAULT - STICK_R_DEAD_ZONE)
			_padState[i] |= RIGHT_STICK_LEFT;
		if (dijs.lZ > STICK_R_DEFAULT + STICK_R_DEAD_ZONE)
			_padState[i] |= RIGHT_STICK_RIGHT;
		if (dijs.lRz < STICK_R_DEFAULT - STICK_R_DEAD_ZONE)
			_padState[i] |= RIGHT_STICK_UP;
		if (dijs.lRz > STICK_R_DEFAULT + STICK_R_DEAD_ZONE)
			_padState[i] |= RIGHT_STICK_DOWN;


		//** Ｘボタン
		if (dijs.rgbButtons[rgbButtons_X] & 0x80)	_padState[i] |= BUTTON_X;
		//** Ａボタン
		if (dijs.rgbButtons[rgbButtons_A] & 0x80)	_padState[i] |= BUTTON_A;
		//** Ｂボタン
		if (dijs.rgbButtons[rgbButtons_B] & 0x80)	_padState[i] |= BUTTON_B;
		//** Ｙボタン
		if (dijs.rgbButtons[rgbButtons_Y] & 0x80)	_padState[i] |= BUTTON_Y;
		//** Ｌボタン
		if (dijs.rgbButtons[rgbButtons_L] & 0x80)	_padState[i] |= BUTTON_L;
		//** Ｒボタン
		if (dijs.rgbButtons[rgbButtons_R] & 0x80)	_padState[i] |= BUTTON_R;
		//** Ｌ2ボタン
		if (dijs.rgbButtons[rgbButtons_L2] & 0x80)	_padState[i] |= BUTTON_L2;
		//** Ｒ2ボタン
		if (dijs.rgbButtons[rgbButtons_R2] & 0x80)	_padState[i] |= BUTTON_R2;
		//** Selectボタン
		if (dijs.rgbButtons[rgbButtons_SELECT] & 0x80)	_padState[i] |= BUTTON_SELECT;
		//** ＳＴＡＲＴボタン
		if (dijs.rgbButtons[rgbButtons_START] & 0x80)	_padState[i] |= BUTTON_START;
		//** BACKボタン
		if (dijs.rgbButtons[10] & 0x80)	_padState[i] |= BUTTON_BACK;
		//** OPTIONボタン
		if (dijs.rgbButtons[11] & 0x80)	_padState[i] |= BUTTON_OPTION;

		// 十字キー↑
		if (dijs.rgdwPOV[0] == 0)		_padState[i] |= CROSS_BUTTON_UP;
		// 十字キー←
		if (dijs.rgdwPOV[0] == 27000)	_padState[i] |= CROSS_BUTTON_LEFT;
		// 十字キー→
		if (dijs.rgdwPOV[0] == 9000)	_padState[i] |= CROSS_BUTTON_RIGHT;
		// 十字キー↓
		if (dijs.rgdwPOV[0] == 18000)	_padState[i] |= CROSS_BUTTON_DOWN;

		// Trigger設定
		_padTrigger[i] = ((lastPadState ^ _padState[i])	// 前回と違っていて
			& _padState[i]);					// しかも今ONのやつ

		// Release設定
		_padRelease[i] = (lastPadState ^ _padState[i] & ~_padState[i]);
	}

	return S_OK;
}


BOOL InputSystem::IsButtonPressed(int padNo, DWORD button)
{
	return (button & _padState[padNo]);
}
BOOL InputSystem::IsButtonTriggered(int padNo, DWORD button)
{
	return (button & _padTrigger[padNo]);
}
BOOL InputSystem::IsButtonReleased(int padNo, DWORD button)
{
	return (button & _padRelease[padNo]);
}
BOOL InputSystem::IsPadConnected(void)
{
	return (_padCnt > 0);
}


#pragma endregion



