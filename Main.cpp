/**
* @file Main.cpp
* @brief Win32 メイン処理
* @author GP12B183 12 小峰遥一
* @details 
*/

#include "stdafx.h"

// マクロ定義
#define CLASS_NAME		"Application"	// ウインドウのクラス名
#define WINDOW_NAME		"GameBase"		// ウインドウのキャプション名


// グローバル変数
HWND hWnd;								// 現在のウィンドウ
HINSTANCE hInst;						// 現在のインターフェイス

#ifdef _DEBUG
int		cntFPS;							// FPSカウンタ
char	debugStr[2048] = WINDOW_NAME;	// デバッグ文字表示用
#endif


// プロトタイプ宣言
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

/**
* @brief メイン処理
*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// メモリリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	

	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど警告が出る（未使用宣言）

	//// リソースファイルから文字列読み取り
	//LoadStringW(hInstance, IDS_APP_TITLE,   szTitle,       MAX_LOADSTRING);
	//LoadStringW(hInstance, IDC_APPLICATION, szWindowClass, MAX_LOADSTRING);

	// ウィンドウクラスの登録
	MyRegisterClass(hInstance);

	// アプリケーションの初期化
	if (!InitInstance(hInstance, nCmdShow)) return FALSE;

	// ゲームシステムの初期化
	Game.CreateInstance();
	if (!Game.Init(hInstance, hWnd)) return FALSE;

#ifdef _IMGUI
	//ImGuiの初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(DX11.GetDevice(), DX11.GetDeviceContext());
#endif // _IMGUI

	// フレームカウント初期化
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	timeBeginPeriod(1);								// 分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// システム時刻をミリ秒単位で取得
	dwCurrentTime = dwFrameCount = 0;

	// メッセージループ
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
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)				// 1秒ごとに実行
			{
#ifdef _DEBUG
				cntFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;							// FPSを測定した時刻を保存
				dwFrameCount = 0;										// カウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / FRAME_RATE))// フレームレートごとに実行
			{
				// 前回フレームとの時刻の差を秒に変換した値
				float deltaTime = (dwCurrentTime - dwExecLastTime) / 1000.0f;

				// デバッグ時などに大きくなり過ぎないように制限する
				if (deltaTime > 0.05f)
				{
					deltaTime = 0.05f;
				}

				dwExecLastTime = dwCurrentTime;							// 処理した時刻を保存

#ifdef _DEBUG	// デバッグ版の時だけFPSを表示する
				wsprintf(debugStr, WINDOW_NAME);
				wsprintf(&debugStr[strlen(debugStr)], " FPS:%d", cntFPS);
				SetWindowText(hWnd, debugStr);
#endif

				// *************ここにゲームの処理を書く*************

				// ゲームの入力処理
				Game.ProcessInput();

				// ゲームの更新処理
				Game.Update(deltaTime);

				// バックバッファクリア
				DX11.ClearRender();

				// ゲームの描画処理
				Game.Draw();

				// バッファ入れ替え
				DX11.Present();

				dwFrameCount++;
			}
		}
	}

	// ゲームシステムを削除
	Game.Uninit();
	Game.DeleteInstance();

	// Direct3D管理クラスを削除
	DX11.Uninit();
	DX11.DeleteInstance();

	// 分解能を戻す
	timeEndPeriod(1);

	// ウィンドウクラスの登録を解除
	UnregisterClass(WINDOW_NAME, hInst);

	// メモリリーク検出
	_CrtDumpMemoryLeaks();

	return (int)msg.wParam;
}



/**
* @brief ウィンドウクラスの登録
*/
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	// 参照:https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-wndclassexw

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
* @brief インスタンスハンドルを保存してメインウィンドウを作成
*/
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	hWnd = CreateWindow(
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW - WS_THICKFRAME,	// WS_THICKFRAMEフラグを除外するとマウスでウィンドウサイズを変更できなくなる
		CW_USEDEFAULT,																			// ウィンドウの左座標
		CW_USEDEFAULT,																			// ウィンドウの上座標
		SCREEN_WIDTH  + GetSystemMetrics(SM_CXDLGFRAME) * 2,									// ウィンドウ横幅
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	// ウィンドウ縦幅
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd) return FALSE;
 

	BOOL isNotFullScr = TRUE;

#ifndef _DEBUG
	int id = MessageBox(NULL, "全画面モードでプレイしますか？",
		"起動モード", MB_YESNOCANCEL | MB_ICONQUESTION);
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

	// Direct3Dの初期化
	DirectX11Device::CreateInstance();
	DX11.Init(hWnd, isNotFullScr);

	// ランダムの初期化処理
	srand((unsigned short)time(NULL));

	ShowWindow(hWnd, nCmdShow);	// 初期化処理の後に呼ぶ
	UpdateWindow(hWnd);
	ShowCursor(TRUE);

	return TRUE;
}

/**
* @brief プロシージャ
* @details ウィンドウのメッセージを処理する
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