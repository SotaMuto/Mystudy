/**
* @file GameSystem.h
* @brief �Q�[���̍��g�݂ƂȂ�V�X�e��
* @author GP12B183 12 �����y��
* @details
*/

#include "stdafx.h"

// �V���O���g���p�̐錾
GameSystem* GameSystem::_instance = nullptr;

/*
@brief ����������
*/
BOOL GameSystem::Init(HINSTANCE hInst, HWND hWnd)
{
	// �f�o�b�O�p�N���X�𐶐�
	Debug.CreateInstance();

	// ���̓V�X�e����������
	_inputSystem = InputSystem();
	_inputSystem.Init(hInst, hWnd);

	// �����_���[��������
	_renderer = Renderer();
	_renderer.Init();

	// �X�L�j���O�p�����_���[��������
	_skiningRenderer = SkinnedMeshRenderer();
	_skiningRenderer.Init();


	//-----------------------------------------------

	// �J�����𐶐������C���J�����Ƃ��ăZ�b�g
	CameraActor* cam = new OrbitCameraActor();
	SetMainCamera(cam);

	// �T���v���P�E�E�E���ʂ̃��f��
	{
		Actor* actor = new Actor();
		MeshComponent* meshComp = new MeshComponent(actor, LoadMesh("data/model/model_sample_torus_01.obj"));

		actor->SetPos(50.0f, 0.0f, 0.0f);

		_sample1 = actor;
	}

	// �T���v���Q�E�E�E�X�L�����b�V�����f��
	{
		Actor* actor = new Actor();
		SkinnedMeshComponent* skinMesh = new SkinnedMeshComponent(actor);
		SkinnedMesh* mesh = LoadSkinnedMesh("data/model/model_player_01.fbx");
		skinMesh->SetMesh(mesh);
		skinMesh->SetAnimStack(1);
		skinMesh->PlayAnimation();

		actor->SetPos(-50.0f, 0.0f, 0.0f);

		_sample2 = actor;
		_sampleSkinMesh = skinMesh;
	}

	//-----------------------------------------------


	return TRUE;
}


/*
@brief �I������
*/
void GameSystem::Uninit(void)
{
	// �X�L�j���O�p�����_���[�̏I������
	_skiningRenderer.Uninit();

	// �����_���[�̏I������
	_renderer.Uninit();

	// ���̓V�X�e���̏I������
	_inputSystem.Uninit();

	// �f�o�b�O�p�N���X���폜
	Debug.DeleteInstance();
}


/*
@brief ���͏���
*/
void GameSystem::ProcessInput(void)
{
	// ���̓V�X�e���̍X�V
	_inputSystem.Update();

	// �A�N�^�[�̓��͏���
	_actorMgr.ProcessInput(_inputSystem);


	// �A�j���[�V�����Đ��؂�ւ�
	static bool flg = false;	// �ق�Ƃ�static�g�킸�����Ƃ�����ق�������
	if (_inputSystem.GetKeyboardTrigger(DIK_SPACE))
	{
		if (flg)
		{
			_sampleSkinMesh->PlayAnimation();
		}
		else
		{
			_sampleSkinMesh->StopAnimation();
		}

		flg = !flg;
	}
}


/*
@brief �X�V����
*/
void GameSystem::Update(float deltaTime)
{
	// �A�N�^�[�̍X�V����
	_actorMgr.Update(deltaTime);

	Debug.Print("\n");
	Debug.Print("�}�E�X�Ŏ��_��]���Y�[���C���A�E�g\n");
	Debug.Print("�X�y�[�X�ŃA�j���[�V�����Đ�/��~\n\n");

	// �T���v���P����]�����Ă݂�
	_sample1->Rotate(0.01f, 0.0f, 0.0f);
}


/*
@brief �`�揈��
*/
void GameSystem::Draw(void)
{
	_skiningRenderer.Draw();

	_renderer.Draw();

	// �f�o�b�O�\��
#ifdef _DEBUG
	Debug.Draw();	
#endif // _DEBUG
}
