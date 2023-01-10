/**
* @file GameSystem.h
* @brief ゲームの骨組みとなるシステム
* @author GP12B183 12 小峰遥一
* @details
*/

#include "stdafx.h"

// シングルトン用の宣言
GameSystem* GameSystem::_instance = nullptr;

/*
@brief 初期化処理
*/
BOOL GameSystem::Init(HINSTANCE hInst, HWND hWnd)
{
	// デバッグ用クラスを生成
	Debug.CreateInstance();

	// 入力システムを初期化
	_inputSystem = InputSystem();
	_inputSystem.Init(hInst, hWnd);

	// レンダラーを初期化
	_renderer = Renderer();
	_renderer.Init();

	// スキニング用レンダラーを初期化
	_skiningRenderer = SkinnedMeshRenderer();
	_skiningRenderer.Init();


	//-----------------------------------------------

	// カメラを生成＆メインカメラとしてセット
	CameraActor* cam = new OrbitCameraActor();
	SetMainCamera(cam);

	// サンプル１・・・普通のモデル
	{
		Actor* actor = new Actor();
		MeshComponent* meshComp = new MeshComponent(actor, LoadMesh("data/model/model_sample_torus_01.obj"));

		actor->SetPos(50.0f, 0.0f, 0.0f);

		_sample1 = actor;
	}

	// サンプル２・・・スキンメッシュモデル
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
@brief 終了処理
*/
void GameSystem::Uninit(void)
{
	// スキニング用レンダラーの終了処理
	_skiningRenderer.Uninit();

	// レンダラーの終了処理
	_renderer.Uninit();

	// 入力システムの終了処理
	_inputSystem.Uninit();

	// デバッグ用クラスを削除
	Debug.DeleteInstance();
}


/*
@brief 入力処理
*/
void GameSystem::ProcessInput(void)
{
	// 入力システムの更新
	_inputSystem.Update();

	// アクターの入力処理
	_actorMgr.ProcessInput(_inputSystem);


	// アニメーション再生切り替え
	static bool flg = false;	// ほんとはstatic使わずちゃんとやったほうがいい
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
@brief 更新処理
*/
void GameSystem::Update(float deltaTime)
{
	// アクターの更新処理
	_actorMgr.Update(deltaTime);

	Debug.Print("\n");
	Debug.Print("マウスで視点回転＆ズームインアウト\n");
	Debug.Print("スペースでアニメーション再生/停止\n\n");

	// サンプル１を回転させてみる
	_sample1->Rotate(0.01f, 0.0f, 0.0f);
}


/*
@brief 描画処理
*/
void GameSystem::Draw(void)
{
	_skiningRenderer.Draw();

	_renderer.Draw();

	// デバッグ表示
#ifdef _DEBUG
	Debug.Draw();	
#endif // _DEBUG
}
