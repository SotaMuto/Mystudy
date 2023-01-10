#pragma once

/**
* @file stdafx.h
* @brief プリコンパイル済みヘッダー
* @author GP12B183 12 小峰遥一
* @details
* ここでまとめてヘッダーファイルをインクルードすることで
* ビルドを高速化できる（参照：https://bituse.info/c/33）
*/


#pragma warning(push)
#pragma warning(disable:4005)			// マクロ再定義のwarning防止？

#define _CRT_SECURE_NO_WARNINGS			// scanf のwarning防止
#define DIRECTINPUT_VERSION 0x0800		// 警告対処

//#pragma warning(pop)

// プロジェクトで使用するヘッダーファイルを全てインクルードする
// 継承元のクラスが定義されているヘッダーなどは順番に注意する
#include <stdio.h>
#include <time.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <random>
#include <ctime>
#include <tchar.h>
#include <functional>
#include <cassert>
#include <math.h>
#include <ctype.h>
#include <memory>
#include <numeric>
#include <string>
#include <unordered_map>
#include <stack>

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <windows.h>

#include "dinput.h"
#include "xaudio2.h"

#include "ManageMemory.h"
#include "DebugProc.h"

#include "DirectX11Device.h"
#include "Main.h"
#include "Input.h"

#include "Texture.h"
#include "Mesh.h"

#include "miniz.h"
#include "FBXLoader.h"
#include "Bone.h"
#include "SkinnedMesh.h"

#include "MeshLoader.h"
#include "OBJLoader.h"

#include "XMFLOATHelper.h"
#include "Math.h"

#include "TextureManager.h"
#include "MeshManager.h"
#include "SkinnedMeshManager.h"

#include "Component.h"
#include "Comp_Mesh.h"
#include "Comp_SkinnedMesh.h"

#include "Actor.h"
#include "ActorManager.h"
#include "Actor_Camera.h"
#include "Actor_OrbitCamera.h"

#include "SkinnedMeshRenderer.h"
#include "Renderer.h"

#include "GameSystem.h"

