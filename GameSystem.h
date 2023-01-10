#pragma once

/**
* @file GameSystem.h
* @brief �Q�[���̍��g�݂ƂȂ�V�X�e��
* @author GP12B183 12 �����y��
* @details
*/

/**
* @brief �Q�[���̃��C���V�X�e���N���X
*/
class GameSystem
{
private:
	InputSystem			_inputSystem;
	Renderer			_renderer;
	SkinnedMeshRenderer _skiningRenderer;
	ActorManager		_actorMgr;

	CameraActor* _mainCamera;

	Actor* _sample1;
	Actor* _sample2;
	SkinnedMeshComponent* _sampleSkinMesh;

public:
	BOOL Init(HINSTANCE hInst, HWND hWnd);
	void Uninit(void);
	void ProcessInput(void);
	void Update(float deltaTime);
	void Draw(void);

#pragma region Getter/Setter
	InputSystem* GetInputSystem(void) { return &_inputSystem; }		// �v�C���F�g��Ȃ���
	Renderer* GetRenderer(void) { return &_renderer; }
	SkinnedMeshRenderer* GetSkinnedMeshRenderer(void) { return &_skiningRenderer; }
	ActorManager* GetActorManager(void) { return &_actorMgr; }

	void SetMainCamera(CameraActor* mainCam) { _mainCamera = mainCam; }
	CameraActor* GetMainCamera(void) { return _mainCamera; }
#pragma endregion


	// --------------�V���O���g����������--------------
private:
	static GameSystem* _instance;

	GameSystem() {}	// �R���X�g���N�^�̓v���C�x�[�g�ɉB��
public:
	// �C���X�^���X����
	static void CreateInstance()
	{
		DeleteInstance();

		_instance = new GameSystem();
	}
	// �C���X�^���X�폜
	static void DeleteInstance()
	{
		if (_instance != nullptr)
		{
			delete _instance;
			_instance = nullptr;
		}
	}
	// �C���X�^���X�擾
	static GameSystem& GetInstance()
	{
		return *_instance;
	}
	// --------------�V���O���g�������܂�--------------
};


// ����肷����ƕ�����ɂ����Ȃ�̂ŕK�v�Œ���ɂ���

/**
* @def Game
* @brief �Q�[���V�X�e���N���X�̃V���O���g���C���X�^���X���ȒP�Ɏ擾���邽�߂̃}�N��
*/
#define Game GameSystem::GetInstance()

/**
* @def LoadTexture
* @brief �e�N�X�`���ǂݍ��݊֐��̌Ăяo���}�N��
*/
#define LoadTexture(filePath) GameSystem::GetInstance().GetRenderer()->GetTextureManager()->Load(filePath)

/**
* @def LoadMesh
* @brief ���b�V���ǂݍ��݊֐��̌Ăяo���}�N��
*/
#define LoadMesh(filePath) GameSystem::GetInstance().GetRenderer()->GetMeshManager()->Load(filePath)

/**
* @def LoadMesh
* @brief �X�L�����b�V���ǂݍ��݊֐��̌Ăяo���}�N��
*/
#define LoadSkinnedMesh(filePath) GameSystem::GetInstance().GetSkinnedMeshRenderer()->GetSkinnedMeshManager()->Load(filePath)