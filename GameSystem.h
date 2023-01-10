#pragma once

/**
* @file GameSystem.h
* @brief ゲームの骨組みとなるシステム
* @author GP12B183 12 小峰遥一
* @details
*/

/**
* @brief ゲームのメインシステムクラス
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
	InputSystem* GetInputSystem(void) { return &_inputSystem; }		// 要修正：使わないで
	Renderer* GetRenderer(void) { return &_renderer; }
	SkinnedMeshRenderer* GetSkinnedMeshRenderer(void) { return &_skiningRenderer; }
	ActorManager* GetActorManager(void) { return &_actorMgr; }

	void SetMainCamera(CameraActor* mainCam) { _mainCamera = mainCam; }
	CameraActor* GetMainCamera(void) { return _mainCamera; }
#pragma endregion


	// --------------シングルトンここから--------------
private:
	static GameSystem* _instance;

	GameSystem() {}	// コンストラクタはプライベートに隠す
public:
	// インスタンス生成
	static void CreateInstance()
	{
		DeleteInstance();

		_instance = new GameSystem();
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
	static GameSystem& GetInstance()
	{
		return *_instance;
	}
	// --------------シングルトンここまで--------------
};


// ↓作りすぎると分かりにくくなるので必要最低限にする

/**
* @def Game
* @brief ゲームシステムクラスのシングルトンインスタンスを簡単に取得するためのマクロ
*/
#define Game GameSystem::GetInstance()

/**
* @def LoadTexture
* @brief テクスチャ読み込み関数の呼び出しマクロ
*/
#define LoadTexture(filePath) GameSystem::GetInstance().GetRenderer()->GetTextureManager()->Load(filePath)

/**
* @def LoadMesh
* @brief メッシュ読み込み関数の呼び出しマクロ
*/
#define LoadMesh(filePath) GameSystem::GetInstance().GetRenderer()->GetMeshManager()->Load(filePath)

/**
* @def LoadMesh
* @brief スキンメッシュ読み込み関数の呼び出しマクロ
*/
#define LoadSkinnedMesh(filePath) GameSystem::GetInstance().GetSkinnedMeshRenderer()->GetSkinnedMeshManager()->Load(filePath)