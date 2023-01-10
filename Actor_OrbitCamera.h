#pragma once


/**
* @file CameraActor.h
* @brief ‹O“¹ƒJƒƒ‰i’‹“_‚ğ’†S‚É‰ñ“]‚·‚éj
* @author GP12B183 12 ¬•ô—yˆê
* @details
*/


/**
* @brief ‹O“¹ƒJƒƒ‰
*/
class OrbitCameraActor : public CameraActor
{
private:
	static const float ROT_SPD;
	static const float ZOOM_SPD;
	static const float PHI_MIN;
	static const float PHI_MAX;
	static const float THETA_MIN;
	static const float THETA_MAX;
	static const float ZOOM_MIN;
	static const float ZOOM_MAX;

	float _phi    = 0.0f;
	float _theta  = XMConvertToRadians(45.0f);
	float _length = (ZOOM_MIN + ZOOM_MAX) * 0.5f;

	XMFLOAT3 _input;

public:
	OrbitCameraActor();
	virtual ~OrbitCameraActor();

	void ActorInput(const InputSystem& input) override;
	void UpdateActor(float deltaTime) override;
};