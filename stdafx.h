#pragma once

/**
* @file stdafx.h
* @brief �v���R���p�C���ς݃w�b�_�[
* @author GP12B183 12 �����y��
* @details
* �����ł܂Ƃ߂ăw�b�_�[�t�@�C�����C���N���[�h���邱�Ƃ�
* �r���h���������ł���i�Q�ƁFhttps://bituse.info/c/33�j
*/


#pragma warning(push)
#pragma warning(disable:4005)			// �}�N���Ē�`��warning�h�~�H

#define _CRT_SECURE_NO_WARNINGS			// scanf ��warning�h�~
#define DIRECTINPUT_VERSION 0x0800		// �x���Ώ�

//#pragma warning(pop)

// �v���W�F�N�g�Ŏg�p����w�b�_�[�t�@�C����S�ăC���N���[�h����
// �p�����̃N���X����`����Ă���w�b�_�[�Ȃǂ͏��Ԃɒ��ӂ���
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

