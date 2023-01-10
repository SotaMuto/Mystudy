#pragma once

/**
* @file DebugProc.h
* @brief デバッグ処理
* @author GP12B183 12 小峰遥一
* @details
*/


/**
* @brief デバッグ用クラス
*/
class DebugProc
{
private:
	static const int		DEBUG_STR_MAX = 2048;	// 最大文字数
	static const RECT		DEBUG_PRINT_RECT;		// デバッグ表示箇所
	static const COLORREF	DEBUG_PRINT_COLOR;		// デバッグ文字色

	char _printBuff[DEBUG_STR_MAX] = { "\0" };	// 画面表示用のデバッグ文字列

public:
	//! 出力画面に表示
	void Out(const char *str, ...);
	//! 画面左上に表示
	void Print(const char *str, ...);
	//! ダイアログで表示
	void ErrorDialog(const char *str, ...);
	//! ファイルに書き込み
	void ExportFile(const char* fileName, const char *str, ...);

	void Draw(void);


	// --------------シングルトンここから--------------
private:
	static DebugProc* _instance;

	DebugProc() {}	// コンストラクタはプライベートに隠す
	~DebugProc() {}
public:
	// インスタンス生成
	static void CreateInstance()
	{
		DeleteInstance();

		_instance = new DebugProc();
	}
	// インスタンス削除
	static void DeleteInstance()
	{
		if (_instance != nullptr)
		{
			delete _instance;
			_instance = nullptr;
		}
	}
	// インスタンス取得
	static DebugProc& GetInstance()
	{
		return *_instance;
	}
	// --------------シングルトンここまで--------------
};

/**
* @brief インスタンスを簡単に取得するためのマクロ
*/
#define Debug DebugProc::GetInstance()