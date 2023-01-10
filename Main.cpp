/**
* @file Main.cpp
* @brief Win32 ���C������
* @author GP12B183 12 �����y��
* @details 
*/

#include "stdafx.h"

// �}�N����`
#define CLASS_NAME		"Application"	// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"GameBase"		// �E�C���h�E�̃L���v�V������


// �O���[�o���ϐ�
HWND hWnd;								// ���݂̃E�B���h�E
HINSTANCE hInst;						// ���݂̃C���^�[�t�F�C�X

#ifdef _DEBUG
int		cntFPS;							// FPS�J�E���^
char	debugStr[2048] = WINDOW_NAME;	// �f�o�b�O�����\���p
#endif


// �v���g�^�C�v�錾
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

/**
* @brief ���C������
*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ���������[�N���o
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	

	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǌx�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǌx�����o��i���g�p�錾�j

	//// ���\�[�X�t�@�C�����當����ǂݎ��
	//LoadStringW(hInstance, IDS_APP_TITLE,   szTitle,       MAX_LOADSTRING);
	//LoadStringW(hInstance, IDC_APPLICATION, szWindowClass, MAX_LOADSTRING);

	// �E�B���h�E�N���X�̓o�^
	MyRegisterClass(hInstance);

	// �A�v���P�[�V�����̏�����
	if (!InitInstance(hInstance, nCmdShow)) return FALSE;

	// �Q�[���V�X�e���̏�����
	Game.CreateInstance();
	if (!Game.Init(hInstance, hWnd)) return FALSE;

#ifdef _IMGUI
	//ImGui�̏�����
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(DX11.GetDevice(), DX11.GetDeviceContext());
#endif // _IMGUI

	// �t���[���J�E���g������
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	timeBeginPeriod(1);								// ����\��ݒ�
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// �V�X�e���������~���b�P�ʂŎ擾
	dwCurrentTime = dwFrameCount = 0;

	// ���b�Z�[�W���[�v
	MSG msg;
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)				// 1�b���ƂɎ��s
			{
#ifdef _DEBUG
				cntFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;							// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;										// �J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / FRAME_RATE))// �t���[�����[�g���ƂɎ��s
			{
				// �O��t���[���Ƃ̎����̍���b�ɕϊ������l
				float deltaTime = (dwCurrentTime - dwExecLastTime) / 1000.0f;

				// �f�o�b�O���Ȃǂɑ傫���Ȃ�߂��Ȃ��悤�ɐ�������
				if (deltaTime > 0.05f)
				{
					deltaTime = 0.05f;
				}

				dwExecLastTime = dwCurrentTime;							// ��������������ۑ�

#ifdef _DEBUG	// �f�o�b�O�ł̎�����FPS��\������
				wsprintf(debugStr, WINDOW_NAME);
				wsprintf(&debugStr[strlen(debugStr)], " FPS:%d", cntFPS);
				SetWindowText(hWnd, debugStr);
#endif

				// *************�����ɃQ�[���̏���������*************

				// �Q�[���̓��͏���
				Game.ProcessInput();

				// �Q�[���̍X�V����
				Game.Update(deltaTime);

				// �o�b�N�o�b�t�@�N���A
				DX11.ClearRender();

				// �Q�[���̕`�揈��
				Game.Draw();

				// �o�b�t�@����ւ�
				DX11.Present();

				dwFrameCount++;
			}
		}
	}

	// �Q�[���V�X�e�����폜
	Game.Uninit();
	Game.DeleteInstance();

	// Direct3D�Ǘ��N���X���폜
	DX11.Uninit();
	DX11.DeleteInstance();

	// ����\��߂�
	timeEndPeriod(1);

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(WINDOW_NAME, hInst);

	// ���������[�N���o
	_CrtDumpMemoryLeaks();

	return (int)msg.wParam;
}



/**
* @brief �E�B���h�E�N���X�̓o�^
*/
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	// �Q��:https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-wndclassexw

	WNDCLASSEX wcex;
	wcex.cbSize        = sizeof(WNDCLASSEX);
	wcex.style         = CS_CLASSDC;
	wcex.lpfnWndProc   = WndProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hInstance     = hInstance;
	wcex.hIcon         = NULL;
	wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName  = NULL;
	wcex.lpszClassName = CLASS_NAME;
	wcex.hIconSm       = NULL;

	return RegisterClassEx(&wcex);
}


/**
* @brief �C���X�^���X�n���h����ۑ����ă��C���E�B���h�E���쐬
*/
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	hWnd = CreateWindow(
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW - WS_THICKFRAME,	// WS_THICKFRAME�t���O�����O����ƃ}�E�X�ŃE�B���h�E�T�C�Y��ύX�ł��Ȃ��Ȃ�
		CW_USEDEFAULT,																			// �E�B���h�E�̍����W
		CW_USEDEFAULT,																			// �E�B���h�E�̏���W
		SCREEN_WIDTH  + GetSystemMetrics(SM_CXDLGFRAME) * 2,									// �E�B���h�E����
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	// �E�B���h�E�c��
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd) return FALSE;
 

	BOOL isNotFullScr = TRUE;

#ifndef _DEBUG
	int id = MessageBox(NULL, "�S��ʃ��[�h�Ńv���C���܂����H",
		"�N�����[�h", MB_YESNOCANCEL | MB_ICONQUESTION);
	switch (id)
	{
	case IDYES:
		isNotFullScr = FALSE;
		break;
	case IDNO:
		isNotFullScr = TRUE;
		break;
	case IDCANCEL:
	default:
		exit(-1);
		break;
	}
#endif // !_DEBUG

	// Direct3D�̏�����
	DirectX11Device::CreateInstance();
	DX11.Init(hWnd, isNotFullScr);

	// �����_���̏���������
	srand((unsigned short)time(NULL));

	ShowWindow(hWnd, nCmdShow);	// �����������̌�ɌĂ�
	UpdateWindow(hWnd);
	ShowCursor(TRUE);

	return TRUE;
}

/**
* @brief �v���V�[�W��
* @details �E�B���h�E�̃��b�Z�[�W����������
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_MOUSEMOVE:
		//mouseX = LOWORD(lParam);
		//mouseY = HIWORD(lParam);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

HWND GetHWND(void)
{
	return hWnd;
}