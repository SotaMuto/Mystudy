#pragma once

/**
* @file Main.h
* @brief メイン処理
* @author GP12B183 12 小峰遥一
* @details
*/

// 本来はヘッダに書かない方が良い
using namespace DirectX;


#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")

#define SCREEN_WIDTH		(1280)					// ウインドウの幅
#define SCREEN_HEIGHT		( 720)					// ウインドウの高さ
#define FRAME_RATE			(  60)					// フレームレート
#define SCREEN_CENTER_X		(SCREEN_WIDTH  * 0.5f)	// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y		(SCREEN_HEIGHT * 0.5f)	// ウインドウの中心Ｙ座標
#define DEBUG_DISP_TEXTOUT							// デバッグ用画面テキスト出力を有効にする

HWND GetHWND(void);