#include "stdafx.h"

/**
* @file CameraActor.h
* @brief �O���J�����i�����_�𒆐S�ɉ�]����j
* @author GP12B183 12 �����y��
* @details
*/


const float OrbitCameraActor::ROT_SPD   = 0.2f;
const float OrbitCameraActor::ZOOM_SPD  = -5.0f;
const float OrbitCameraActor::PHI_MIN   = 0.0f;
const float OrbitCameraActor::PHI_MAX   = XM_2PI;
const float OrbitCameraActor::THETA_MIN = XM_PI * 0.1f;
const float OrbitCameraActor::THETA_MAX = XM_PI * 0.9f;
const float OrbitCameraActor::ZOOM_MIN  = 10.0f;
const float OrbitCameraActor::ZOOM_MAX  = 300.0f;


/**
* @brief �R���X�g���N�^
*/
OrbitCameraActor::OrbitCameraActor() : 
	CameraActor()
{
}


/**
* @brief �f�X�g���N�^
*/
OrbitCameraActor::~OrbitCameraActor()
{
}


/**
* @brief ���͏���
*/
void OrbitCameraActor::ActorInput(const InputSystem& input)
{
	_input = {
		(float)input.GetMouseX(),
		(float)input.GetMouseY(),
		(float)input.GetMouseZ()
	};
}

/**
* @brief �X�V����
*/
void OrbitCameraActor::UpdateActor(float deltaTime)
{
	float dx = _input.x * ROT_SPD * deltaTime;
	_phi = Math::Loop(_phi + dx, PHI_MIN, PHI_MAX);

	float dy = -_input.y * ROT_SPD * deltaTime;
	_theta = Math::Clamp(_theta + dy, THETA_MIN, THETA_MAX);

	float dz = _input.z * ZOOM_SPD * deltaTime;
	_length = Math::Clamp(_length + dz, ZOOM_MIN, ZOOM_MAX);

	// �����_
	XMFLOAT3 at = Actor::GetPos();

	// 3���ɍ��W�����߂�
	XMFLOAT3 eye;
	eye.z = at.z - sinf(_theta) * cosf(_phi) * _length;
	eye.x = at.x - sinf(_theta) * sinf(_phi) * _length;
	eye.y = at.y + cosf(_theta) * _length;

	// ��x�N�g��
	XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };

	// �r���[�s����v�Z
	CameraActor::SetEyeAtUp(eye, at, up);

	// ���͂�������
	_input = { 0.0f, 0.0f, 0.0f };

	Debug.Print("Camera Eye (%f, %f, %f)\n", eye.x, eye.y, eye.z);
	Debug.Print("Camera At  (%f, %f, %f)\n", at.x, at.y, at.z);
	Debug.Print("Camera Up  (%f, %f, %f)\n", up.x, up.y, up.z);
}
