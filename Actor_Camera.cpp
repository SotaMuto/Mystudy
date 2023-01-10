#include "stdafx.h"

/**
* @file CameraActor.h
* @brief カメラ
* @author GP12B183 12 小峰遥一
* @details
*/

const float CameraActor::NEAR_Z = 10.0f;
const float CameraActor::FAR_Z = 10000.0f;

/**
* @brief コンストラクタ
*/
CameraActor::CameraActor() :
	Actor()
{
	XMFLOAT4X4 mtxInit;
	XMStoreFloat4x4(&mtxInit, XMMatrixIdentity());
	_mtxView = _mtxProj = _mtxInvView = _mtxInvProj = mtxInit;

	CalcProjMatrix();
}

/**
* @brief デストラクタ
*/
CameraActor::~CameraActor()
{
}


/**
* ビュー行列を生成
*/
void CameraActor::CalcViewMatrix(void)
{
	XMMATRIX mtxView = XMMatrixLookAtLH(XMLoadFloat3(&_eye), XMLoadFloat3(&_at), XMLoadFloat3(&_up));
	XMStoreFloat4x4(&_mtxView, mtxView);

	XMMATRIX mtxInvView = XMMatrixInverse(nullptr, mtxView);
	XMStoreFloat4x4(&_mtxInvView, mtxInvView);
}


/**
* プロジェクション行列を生成
*/
void CameraActor::CalcProjMatrix(void)
{
	XMMATRIX mtxProj
		= XMMatrixPerspectiveFovLH(_fov, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, NEAR_Z, FAR_Z);
	XMStoreFloat4x4(&_mtxProj, mtxProj);

	XMMATRIX mtxInvProj = XMMatrixInverse(nullptr, mtxProj);
	XMStoreFloat4x4(&_mtxInvProj, mtxInvProj);
}



/**
* @brief ワールド座標→スクリーン座標に変換
*/
XMFLOAT3 CameraActor::CovertToScreen(XMFLOAT3 worldPos)
{
	float hw = SCREEN_WIDTH * 0.5f;
	float hh = SCREEN_HEIGHT * 0.5f;

	XMMATRIX mtxVp =
	{
		hw,  0, 0, 0,
		0, -hh, 0, 0,
		0,   0, 1, 0,
		hw, hh, 0, 1
	};

	// ビュー変換とプロジェクション変換
	XMVECTOR world = XMLoadFloat3(&worldPos);
	world = XMVector3Transform(world, XMLoadFloat4x4(&_mtxView));
	world = XMVector3Transform(world, XMLoadFloat4x4(&_mtxProj));

	XMFLOAT3 temp;
	XMStoreFloat3(&temp, world);

	// Zで割って-１～１の範囲に収める
	XMVECTOR viewVec = XMVectorSet(temp.x / temp.z, temp.y / temp.z, 1.0f, 1.0f);

	XMFLOAT3 ans;
	XMStoreFloat3(&ans, XMVector3Transform(viewVec, mtxVp));

	return ans;
}


/**
* @brief スクリーン座標→ワールド座標に変換
*/
XMFLOAT3 CameraActor::CovertToWorld(XMFLOAT3 screenPos)
{
	float hw = SCREEN_WIDTH * 0.5f;
	float hh = SCREEN_HEIGHT * 0.5f;

	// z = 1にしておく(0～1の範囲内)
	XMFLOAT3 sPos = { screenPos.x, screenPos.y, 1.0f };

	XMMATRIX mtxVp =
	{
		hw,  0, 0, 0,
		0, -hh, 0, 0,
		0,   0, 1, 0,
		hw, hh, 0, 1
	};


	XMMATRIX vpInv = XMMatrixInverse(NULL, mtxVp);

	XMVECTOR screen = XMLoadFloat3(&sPos);

	screen = XMVector3Transform(screen, vpInv);
	screen = XMVector3Transform(screen, XMLoadFloat4x4(&_mtxInvProj));
	screen = XMVector3Transform(screen, XMLoadFloat4x4(&_mtxInvView));


	XMFLOAT3 ans;
	XMStoreFloat3(&ans, screen);

	return ans;
}