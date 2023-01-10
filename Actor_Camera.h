#pragma once

/**
* @file CameraActor.h
* @brief ÉJÉÅÉâ
* @author GP12B183 12 è¨ïÙóyàÍ
* @details
*/


/**
* @brief ÉJÉÅÉâ
*/
class CameraActor : public Actor
{
private:
	static const float NEAR_Z;
	static const float FAR_Z;

	float _fov = XMConvertToRadians(45.0f);

	XMFLOAT3   _eye;
	XMFLOAT3   _at;
	XMFLOAT3   _up;

	XMFLOAT4X4 _mtxView;
	XMFLOAT4X4 _mtxProj;
	XMFLOAT4X4 _mtxInvView;
	XMFLOAT4X4 _mtxInvProj;

public:
	CameraActor();
	virtual ~CameraActor();

#pragma region Getter/Setter
	float GetFOV(void) { return _fov; }
	void SetFOV(float fov) {
		_fov = fov;
		CalcProjMatrix();
	}

	void SetEyeAtUp(XMFLOAT3 eye, XMFLOAT3 at, XMFLOAT3 up = { 0.0f, 1.0f, 0.0f }) {
		_eye = eye;
		_at = at;
		_up = up;
		CalcViewMatrix();
	}

	XMFLOAT3 GetEye(void) const { return _eye; }
	XMFLOAT3 GetAt(void) const { return _at; }
	XMFLOAT3 GetUp(void) const { return _up; }
	XMFLOAT4X4* GetViewMatrix(void) { return &_mtxView; }
	XMFLOAT4X4* GetProjMatrix(void) { return &_mtxProj; }
	XMFLOAT4X4* GetInvViewMatrix(void) { return &_mtxInvView; }
	XMFLOAT4X4* GetInvProjMatrix(void) { return &_mtxInvProj; }
#pragma endregion

	virtual void ActorInput(const InputSystem& input) override {};
	virtual void UpdateActor(float deltaTime) override {}

	XMFLOAT3 CovertToScreen(XMFLOAT3 worldPos);
	XMFLOAT3 CovertToWorld(XMFLOAT3 screenPos);


private:
	void CalcViewMatrix(void);
	void CalcProjMatrix(void);
};