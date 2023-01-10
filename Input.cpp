#include "stdafx.h"



const int   InputSystem::PAD_DEAD_ZONE =  2500;		// �p�b�h�̖����]�[��
const int   InputSystem::PAD_RANGE_MIN =  1000;		// �p�b�h�̗L���͈͍ŏ��l
const int   InputSystem::PAD_RANGE_MAX = -1000;		// �p�b�h�̗L���͈͍ő�l
const int   InputSystem::STICK_R_MIN   =  0;		// �E�X�e�B�b�N�̍ŏ��l
const int   InputSystem::STICK_R_MAX   =  65535;	// �E�X�e�B�b�N�̍ő�l
const float InputSystem::STICK_R_DEFAULT   = (STICK_R_MAX - STICK_R_MIN) * 0.5;		// �E�X�e�B�b�N�̃f�t�H���g�l
const float InputSystem::STICK_R_DEAD_ZONE = (STICK_R_MAX - STICK_R_MIN) * 0.125;	// �E�X�e�B�b�N�̖����]�[��

/**
* @brief ���̓V�X�e���̏���������
*/
HRESULT InputSystem::Init(HINSTANCE hInst, HWND hWnd)
{
	if (_isLoad) return S_OK;

	_hwnd = hWnd;

	HRESULT hr = S_OK;

	// DirectInput�I�u�W�F�N�g�̍쐬
	hr = DirectInput8Create(hInst,
		DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&_pDirectInput, NULL);

	// �L�[�{�[�h�̏�����
	InitKeyboard(hInst, hWnd);

	// �}�E�X�̏�����
	InitMouse(hInst, hWnd);

	// �Q�[���p�b�h�̏�����
	InitGamePad(hInst, hWnd);

	// �E�B���h�E�������̃N���C�A���g�̈��ۑ����Ă���
	GetClientRect(_hwnd, &_rectOnInit);

	_isLoad = TRUE;

	return hr;
}


/**
* @brief ���̓V�X�e���̏I������
*/
void InputSystem::Uninit(void)
{
	if (!_isLoad) return;

	// �Q�[���p�b�h�̏I������
	UninitGamePad();

	// �}�E�X�̏I������
	UninitMouse();
	
	// �L�[�{�[�h�̏I������
	UninitKeyboard();

	if (_pDirectInput)
	{
		_pDirectInput->Release();
		_pDirectInput = NULL;
	}

	_isLoad = FALSE;
}


/**
* @brief ���̓V�X�e���̍X�V����
*/
void InputSystem::Update(void)
{
	// �L�[�{�[�h�̍X�V����
	UpdateKeyboard();

	// �}�E�X�̍X�V����
	UpdateMouse();

	// �Q�[���p�b�h�̍X�V����
	UpdateGamePad();
}


#pragma region Keyboard

/**
* @brief �L�[�{�[�h�̏���������
*/
HRESULT InputSystem::InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = _pDirectInput->CreateDevice(GUID_SysKeyboard, &_pKeyboard, NULL);
	if (FAILED(hr) || _pKeyboard == NULL)
	{
		MessageBox(hWnd, "No Keyboard", "Warning", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = _pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Can't setup keyboard", "Warning", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = _pKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Keyboard mode error", "�x��", MB_ICONWARNING);
		return hr;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	_pKeyboard->Acquire();

	return hr;
}


/**
* @brief �L�[�{�[�h�̏I������
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
* @brief �L�[�{�[�h�̍X�V����
*/
HRESULT InputSystem::UpdateKeyboard(void)
{
	HRESULT hr;
	BYTE keyStateOld[256];

	// �O��̃f�[�^��ۑ�
	memcpy(keyStateOld, _keyState, KEY_MAX);

	// �f�o�C�X����f�[�^���擾
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
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		_pKeyboard->Acquire();
	}

	return S_OK;
}

/**
* @brief �L�[�{�[�h�̃v���X��Ԃ��擾
*/
BOOL InputSystem::GetKeyboardPress(int key) const
{
	return (_keyState[key] & 0x80) ? TRUE : FALSE;
}

/**
* @brief �L�[�{�[�h�̃g���K�[��Ԃ��擾
*/
BOOL InputSystem::GetKeyboardTrigger(int key) const
{
	if (key == -1) return FALSE;

	return (_keyStateTrigger[key] & 0x80) ? TRUE : FALSE;
}


/**
* @brief �L�[�{�[�h�̃��s�[�g��Ԃ��擾
*/
BOOL InputSystem::GetKeyboardRepeat(int key) const
{
	if (key == -1) return FALSE;

	return (_keyStateRepeat[key] & 0x80) ? TRUE : FALSE;
}


/**
* @brief �L�[�{�[�h�̃����[�X��Ԃ��擾
*/
BOOL InputSystem::GetKeyboardRelease(int key) const
{
	if (key == -1) return FALSE;

	return (_keyStateRelease[key] & 0x80) ? TRUE : FALSE;
}


#pragma endregion


#pragma region Mouse
/**
* @brief �}�E�X�̏���������
*/
HRESULT InputSystem::InitMouse(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = _pDirectInput->CreateDevice(GUID_SysMouse, &_pMouse, NULL);
	if (FAILED(hr) || _pMouse == NULL)
	{
		MessageBox(hWnd, "No mouse", "Warning", MB_ICONWARNING);
		return hr;
	}
	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = _pMouse->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Can't setup mouse", "Warning", MB_ICONWARNING);
		return hr;
	}
	// ���̃A�v���Ƌ������[�h�ɐݒ�
	hr = _pMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Mouse mode error", "Warning", MB_ICONWARNING);
		return hr;
	}

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD prop;
	prop.diph.dwSize = sizeof(prop);
	prop.diph.dwHeaderSize = sizeof(prop.diph);
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;	// �}�E�X�̈ړ��l�@���Βl

	hr = _pMouse->SetProperty(DIPROP_AXISMODE, &prop.diph);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Mouse property error", "Warning", MB_ICONWARNING);
		return hr;
	}

	// �A�N�Z�X���𓾂�
	_pMouse->Acquire();

	return hr;
}


/**
* @brief �}�E�X�̏I������
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
* @brief �}�E�X�̍X�V����
*/
HRESULT InputSystem::UpdateMouse()
{
	HRESULT hr;
	// �O��̒l�ۑ�
	DIMOUSESTATE2 lastMouseState = _mouseState;
	// �f�[�^�擾
	hr = _pMouse->GetDeviceState(sizeof(_mouseState), &_mouseState);
	if (SUCCEEDED(hr))
	{
		_mouseTrigger.lX = _mouseState.lX;
		_mouseTrigger.lY = _mouseState.lY;
		_mouseTrigger.lZ = _mouseState.lZ;
		// �}�E�X�̃{�^�����
		for (int i = 0; i < 8; i++)
		{
			_mouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
				_mouseState.rgbButtons[i]) & _mouseState.rgbButtons[i]);

			_mouseRelease.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
				_mouseState.rgbButtons[i]) & lastMouseState.rgbButtons[i]);
		}
	}
	else	// �擾���s
	{
		// �A�N�Z�X���𓾂Ă݂�
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

	return DIENUM_CONTINUE;	// ���̃f�o�C�X���
}

BOOL CALLBACK DIEnumEffectsProc(LPCDIEFFECTINFO pei, LPVOID pv)
{
	*((GUID *)pv) = pei->guid;
	return DIENUM_STOP;  // one is enough
}

/**
* @brief �Q�[���p�b�h�̏���������
*/
HRESULT InputSystem::InitGamePad(HINSTANCE hInst, HWND hWnd)
{
	HRESULT		hr;

	_padCnt = 0;

	// �W���C�p�b�h��T��
	// �Z�b�g�����R�[���o�b�N�֐����A�p�b�h�𔭌������������Ă΂��B
	_pDirectInput->EnumDevices(DI8DEVCLASS_GAMECTRL,
		(LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY/**DIEDFL_FORCEFEEDBACK*/);

	for (int i = 0; i < _padCnt; i++)
	{
		// �W���C�X�e�B�b�N�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
		hr = _pGamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(hr))
			return hr; // �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s

		// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
		hr = _pGamePad[i]->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
		if (FAILED(hr))
			return hr; // ���[�h�̐ݒ�Ɏ��s

		// ���̒l�͈̔͂�ݒ�
		// X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
		// (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
		// ����傫������΁A�A�i���O�l�ׂ̍��ȓ�����߂炦����B(�p�b�h�̐��\�ɂ��)
		DIPROPRANGE diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = PAD_RANGE_MIN;
		diprg.lMax = PAD_RANGE_MAX;
		// X���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_X;
		_pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_Y;
		_pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �e�����ƂɁA�����̃]�[���l��ݒ肷��B
		// �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂��ƁB
		// �w�肷��l�́A10000�ɑ΂��鑊�Βl(2000�Ȃ�20�p�[�Z���g)�B
		DIPROPDWORD dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = PAD_DEAD_ZONE;
		//X���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_X;
		_pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		//Y���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_Y;
		_pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);


		// �I�[�g�Z���^�[�@�\���I�t�ɂ���
		// �I�[�g�Z���^�[�@�\�Ƃ̓��[�^�[���g���ăW���C�X�e�B�b�N�̂΂˂��V���~���[�V��������@�\
		// ���̋@�\���ז����Ȃ��悤�ɃI�t�ɂ���Ɨǂ��炵��
		DIPROPDWORD dipac;
		dipac.diph.dwSize = sizeof(DIPROPDWORD);
		dipac.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipac.diph.dwObj = 0;
		dipac.diph.dwHow = DIPH_DEVICE;
		dipac.dwData = DIPROPAUTOCENTER_OFF;
		_pGamePad[i]->SetProperty(DIPROP_AUTOCENTER, &dipac.diph);

		// �T�|�[�g����Ă���G�t�F�N�g��񋓂���
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

		//�W���C�X�e�B�b�N���͐���J�n
		_pGamePad[i]->Acquire();
	}

	return TRUE;
}


/**
* @brief �Q�[���p�b�h�̏I������
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
* @brief �Q�[���p�b�h�̍X�V����
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
		_padState[i] = 0x00000000l;	// ������

		hr = _pGamePad[i]->Poll();	// �W���C�X�e�B�b�N�Ƀ|�[����������
		if (FAILED(hr)) {
			hr = _pGamePad[i]->Acquire();
			while (hr == DIERR_INPUTLOST)
				hr = _pGamePad[i]->Acquire();
		}

		hr = _pGamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// �f�o�C�X��Ԃ�ǂݎ��
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) {
			hr = _pGamePad[i]->Acquire();
			while (hr == DIERR_INPUTLOST)
				hr = _pGamePad[i]->Acquire();
		}


		// �R�Q�̊e�r�b�g�ɈӖ����������A�{�^�������ɉ����ăr�b�g���I���ɂ���
		//** y-axis (forward)
		if (dijs.lY < 0)					_padState[i] |= LEFT_STICK_UP;
		//** y-axis (backward)
		if (dijs.lY > 0)					_padState[i] |= LEFT_STICK_DOWN;
		//** x-axis (left)
		if (dijs.lX < 0)					_padState[i] |= LEFT_STICK_LEFT;
		//** x-axis (right)
		if (dijs.lX > 0)					_padState[i] |= LEFT_STICK_RIGHT;

		// �E�X�e�B�b�N(lZ��lRz���Ή����Ă���炵��)(���ꂼ��0~65535�̒l���Ƃ�A65535/2���G���ĂȂ����)
		// ���X�e�B�b�N�Ɠ��l�A�������|����͖����ɂ���
		if (dijs.lZ < STICK_R_DEFAULT - STICK_R_DEAD_ZONE)
			_padState[i] |= RIGHT_STICK_LEFT;
		if (dijs.lZ > STICK_R_DEFAULT + STICK_R_DEAD_ZONE)
			_padState[i] |= RIGHT_STICK_RIGHT;
		if (dijs.lRz < STICK_R_DEFAULT - STICK_R_DEAD_ZONE)
			_padState[i] |= RIGHT_STICK_UP;
		if (dijs.lRz > STICK_R_DEFAULT + STICK_R_DEAD_ZONE)
			_padState[i] |= RIGHT_STICK_DOWN;


		//** �w�{�^��
		if (dijs.rgbButtons[rgbButtons_X] & 0x80)	_padState[i] |= BUTTON_X;
		//** �`�{�^��
		if (dijs.rgbButtons[rgbButtons_A] & 0x80)	_padState[i] |= BUTTON_A;
		//** �a�{�^��
		if (dijs.rgbButtons[rgbButtons_B] & 0x80)	_padState[i] |= BUTTON_B;
		//** �x�{�^��
		if (dijs.rgbButtons[rgbButtons_Y] & 0x80)	_padState[i] |= BUTTON_Y;
		//** �k�{�^��
		if (dijs.rgbButtons[rgbButtons_L] & 0x80)	_padState[i] |= BUTTON_L;
		//** �q�{�^��
		if (dijs.rgbButtons[rgbButtons_R] & 0x80)	_padState[i] |= BUTTON_R;
		//** �k2�{�^��
		if (dijs.rgbButtons[rgbButtons_L2] & 0x80)	_padState[i] |= BUTTON_L2;
		//** �q2�{�^��
		if (dijs.rgbButtons[rgbButtons_R2] & 0x80)	_padState[i] |= BUTTON_R2;
		//** Select�{�^��
		if (dijs.rgbButtons[rgbButtons_SELECT] & 0x80)	_padState[i] |= BUTTON_SELECT;
		//** �r�s�`�q�s�{�^��
		if (dijs.rgbButtons[rgbButtons_START] & 0x80)	_padState[i] |= BUTTON_START;
		//** BACK�{�^��
		if (dijs.rgbButtons[10] & 0x80)	_padState[i] |= BUTTON_BACK;
		//** OPTION�{�^��
		if (dijs.rgbButtons[11] & 0x80)	_padState[i] |= BUTTON_OPTION;

		// �\���L�[��
		if (dijs.rgdwPOV[0] == 0)		_padState[i] |= CROSS_BUTTON_UP;
		// �\���L�[��
		if (dijs.rgdwPOV[0] == 27000)	_padState[i] |= CROSS_BUTTON_LEFT;
		// �\���L�[��
		if (dijs.rgdwPOV[0] == 9000)	_padState[i] |= CROSS_BUTTON_RIGHT;
		// �\���L�[��
		if (dijs.rgdwPOV[0] == 18000)	_padState[i] |= CROSS_BUTTON_DOWN;

		// Trigger�ݒ�
		_padTrigger[i] = ((lastPadState ^ _padState[i])	// �O��ƈ���Ă���
			& _padState[i]);					// ��������ON�̂��

		// Release�ݒ�
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



