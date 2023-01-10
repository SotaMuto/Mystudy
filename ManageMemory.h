#pragma once

/**
* @file ManageMemory.h
* @brief メモリ管理用マクロの定義
* @author GP12B183 12 小峰遥一
* @details
*/

// インターフェイス解放
#define SAFE_RELEASE(x)       if(x){ x->Release();x=NULL; }
#define SAFE_DELETE(x)        if(x){ delete x;x=NULL; }
#define SAFE_DELETE_ARRAY(x)  if(x){ delete[] x;x=NULL; }
#define SAFE_DELOBJ(p)        if(p){ DeleteObject(p); (p)=NULL; }
