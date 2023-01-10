#include "stdafx.h"

/**
* @file DebugProc.cpp
* @brief デバッグ処理
* @author GP12B183 12 小峰遥一
* @details
*/


// シングルトンのインスタンス宣言
DebugProc* DebugProc::_instance = nullptr;

// デバッグ表示箇所
const RECT		DebugProc::DEBUG_PRINT_RECT = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
// デバッグ文字色
const COLORREF	DebugProc::DEBUG_PRINT_COLOR = RGB(255, 255, 255);


/**
* @brief メイン処理
*/
void DebugProc::Out(const char * str, ...)
{
#ifndef _DEBUG
	return;
#endif // !_DEBUG

	char buff[DEBUG_STR_MAX] = { "\0" };

	// 可変引数から変数を取り出す
	va_list ap;
	va_start(ap, str);

	if (!vsprintf_s(buff, DEBUG_STR_MAX, str, ap)) {

		OutputDebugStringA("dprintf error.");
	}
	va_end(ap);

	OutputDebugStringA(buff);
}


/**
* @brief 画面左上にデバッグ情報を表示
*/
void DebugProc::Print(const char * str, ...)
{
#ifndef _DEBUG
	return;
#endif // !_DEBUG

	char buff[DEBUG_STR_MAX] = { "\0" };

	// 可変引数から変数を取り出す
	va_list ap;
	va_start(ap, str);

	if (!vsprintf_s(buff, DEBUG_STR_MAX, str, ap)) {

		OutputDebugStringA("dprintf error.");
	}
	va_end(ap);


	// 連結（描画処理でまとめて描画するため）
	if ((strlen(_printBuff) + strlen(buff)) < ((sizeof _printBuff) - 1))
	{
		strcat_s(_printBuff, buff);
	}
}


/**
* @brief エラーダイアログを表示
*/
void DebugProc::ErrorDialog(const char * str, ...)
{
#ifndef _DEBUG
	return;
#endif // !_DEBUG

	char buff[DEBUG_STR_MAX] = { "\0" };

	// 可変引数から変数を取り出す
	va_list ap;
	va_start(ap, str);

	if (!vsprintf_s(buff, DEBUG_STR_MAX, str, ap)) {

		OutputDebugStringA("dprintf error.");
	}
	va_end(ap);

	// メッセージボックス表示
	MessageBox(NULL, buff, "ERROR", MB_ICONWARNING);
	PostQuitMessage(0);	// 終了メッセージを送る
}


/**
* @brief ログファイルを出力
*/
void DebugProc::ExportFile(const char * fileName, const char * str, ...)
{
#ifndef _DEBUG
	return;
#endif // !_DEBUG

	char buff[DEBUG_STR_MAX] = { "\0" };

	// 可変引数から変数を取り出す
	va_list ap;
	va_start(ap, str);

	if (!vsprintf_s(buff, DEBUG_STR_MAX, str, ap)) {

		OutputDebugStringA("dprintf error.");
	}
	va_end(ap);

	// ファイルに上書きモードで書き込み
	FILE *fp = fopen(fileName, "a");
	if (fp == NULL) return;
	fprintf(fp, buff);
	fclose(fp);
}


/**
* @brief 描画処理
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

	// バックバッファからサーフェスを取得する
	IDXGISurface1* pBackSurface = NULL;
	hr = DX11.GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface1), (void**)&pBackSurface);
	if (hr != S_OK) return;

	// 取得したサーフェスからデバイスコンテキストを取得する
	HDC hdc;
	hr = pBackSurface->GetDC(FALSE, &hdc);
	if (hr != S_OK) return;

	// 文字色を変更
	SetTextColor(hdc, DEBUG_PRINT_COLOR);

	// 背景を透明に変更
	SetBkMode(hdc, TRANSPARENT);

	//テキスト出力
	RECT rect = DEBUG_PRINT_RECT;
	DrawText(hdc, _printBuff, (int)strlen(_printBuff), &rect, DT_LEFT);

	//デバイスコンテキストを解放する
	pBackSurface->ReleaseDC(NULL);

	//サーフェスを解放する
	pBackSurface->Release();

	//レンダリングターゲットがリセットされるのでセットしなおす
	ID3D11RenderTargetView *rtv = DX11.GetDefaultRenderTargetView();
	DX11.GetDeviceContext()->OMSetRenderTargets(1, &rtv, nullptr);

	// バッファリセット
	memset(_printBuff, 0, sizeof(_printBuff));
}