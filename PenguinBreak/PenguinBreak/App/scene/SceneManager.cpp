#include "SceneManager.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"Shape.h"
#include"PostEffect.h"
#include"GameScene.h"
#include"TitleScene.h"
#include"SelectScene.h"
#include"ResultScene.h"
#include"EditerScene.h"
#include<Easing.h>
SceneManager::SceneManager()
{}
SceneManager::~SceneManager()
{
	scene_->Finalize();
	delete scene_;
}
void SceneManager::Initialize()
{
	//汎用機能
	//ゲームウィンドウの作成
	Window::Get()->CreateGameWindow();
	// DirectX初期化処理
	DirectXBase::Get()->Initilize();
	//入力の初期化
	Input::Get()->Initialize();
	//シェーダーの読み込み
	ShaderManager::LoadShaders();
	//ライト静的初期化
	LightGroup::StaticInitialize(DirectXBase::Get()->GetDevice());
	//スプライト静的初期化
	Sprite::StaticInit(DirectXBase::Get()->GetDevice(), DirectXBase::Get()->GetCmandList());
	//テキストクラス初期化
	Texture::Get()->Init(DirectXBase::Get()->GetDevice());
	Pipeline::CreatePipeline(DirectXBase::Get()->GetDevice());
	//デバックテキスト初期化
	DebugText::Get()->Initialize();
	//スプライトクラス作成
	Sprite::Get()->Init();
	//FBX初期化
	FbxLoader::GetInstance()->Initialize(DirectXBase::Get()->GetDevice());
	FBXObject3d::SetDevice(DirectXBase::Get()->GetDevice());
	FBXObject3d::SetCmdList(DirectXBase::Get()->GetCmandList());
	FBXObject3d::CreateGraphicsPipeline();
	FBXObject3d::CreateShadowPipeline();
	//図形モデル初期化
	Shape::Init(DirectXBase::Get()->GetDevice());
	//パーティクル初期化
	ParticleManager::StaticInitialize(DirectXBase::Get()->GetDevice(), DirectXBase::Get()->GetCmandList(), window_width, window_height);
	//3Dオブジェクト初期化
	Object::Init(DirectXBase::Get()->GetDevice(), DirectXBase::Get()->GetCmandList());
	PostEffect::Get()->Initialize(DirectXBase::Get()->GetDevice());
	//音作成
	Audio::Get()->Init();
	blackOut = Sprite::Get()->SpriteCreate(L"Resources/white1x1.png");
	Texture::Get()->KeepTexture();
	//シーンをタイトルに設定
	BaseScene* scene = new TitleScene();
	SetNextScene(scene);
	if (nextScene_)
	{
		if (scene_)
		{
			scene_->Finalize();
			delete scene_;
		}
		//シーン切り替え
		scene_ = nextScene_;
		nextScene_ = nullptr;
		scene_->SetSceneManager(this);
		//次のシーンを初期化する
		scene_->Init();
	}
}

void SceneManager::Update()
{
	Input::Get()->Update();
	if (typeid(*scene_).name() != typeid(EditerScene).name()) time += 0.01f;
	BlackOut();
	//次のシーンの予約があるなら
	if (nextScene_ && blackOutFlag == Stand_By)
	{
		blackOutFlag = First_Half;
	}
	else if (blackOutFlag == Latter_Half && nextScene_)
	{
		if (scene_)
		{
			scene_->Finalize();
			delete scene_;
		}
		//シーン切り替え
		scene_ = nextScene_;
		nextScene_ = nullptr;
		scene_->SetSceneManager(this);
		//次のシーンを初期化する
		scene_->Init();
	}
	//更新
	scene_->Update();
}

void SceneManager::Draw()
{
	PostEffect::Get()->PreDrawScene(DirectXBase::Get()->GetCmandList());
	Object::InitDraw(), Sprite::Get()->PreDraw();
	//カメラ目線の描画
	scene_->Draw();
	BlackOutDraw();
	DebugText::Get()->DrawAll();

	PostEffect::Get()->PostDrawScene(DirectXBase::Get()->GetCmandList());
	DirectXBase::Get()->PreDraw();
	////ポストエフェクトの描画
	PostEffect::Get()->Draw(DirectXBase::Get()->GetCmandList(), Vec4{ 1.0f,1.0f,1.0f,1.0f }, time, scene_->GetEffect());
	DebugText::Get()->DrawAll();
	//UI描画
	DirectXBase::Get()->ResourceBarrier();
}

void SceneManager::Delete()
{
	Object::Delete();
	FbxLoader::GetInstance()->Finalize();
}

void SceneManager::BlackOut()
{
	if (blackOutFlag == Stand_By)
	{
		blackOutTime = 0.0f;
		for (int i = 0; i < 2; i++)
		{
			blackPos[0] = { -1280.0f,-720.0f,0.0f };

		}
	}
	if (blackOutFlag == First_Half)
	{
		const Vec3 outStartL = { -640.0f ,0.0f,0.0f };
		const Vec3 outEndL = { 0.0f ,0.0f,0.0f };
		//左
		blackPos[0] = Easing::easeOut(outStartL, outEndL, blackOutTime);
		const Vec3 outStarR = { 1280.0f,0.0f,0.0f };
		const Vec3 outEndR = { 640.0f ,0.0f,0.0f };
		//右
		blackPos[1] = Easing::easeOut(outStarR, outEndR, blackOutTime);
		blackOutTime += 0.02f;
		if (blackOutTime >= 1.0f)
		{
			blackOutFlag = Latter_Half;
			blackOutTime = 0.0f;
		}
	}
	if (blackOutFlag == Latter_Half)
	{
		const Vec3 outStartL = { 0.0f ,0.0f,0.0f };
		const Vec3 outEndL = { 0.0f ,-360.0f,0.0f };
		//左
		blackPos[0] = Easing::easeIn(outStartL, outEndL, blackOutTime);
		const Vec3 outStarR = { 0.0f,360.0f,0.0f };
		const Vec3 outEndR = { 0.0f ,720.0f,0.0f };
		//右
		blackPos[1] = Easing::easeIn(outStarR, outEndR, blackOutTime);
		blackOutTime += 0.02f;
		if (blackOutTime >= 1.0f)
		{
			blackOutFlag = Stand_By;
			blackOutTime = 0.0f;
		}
	}
}

void SceneManager::BlackOutDraw()
{
	if (blackOutFlag == First_Half)
	{
		const float width = 640.0f, height = 720.0f;
		Sprite::Get()->Draw(blackOut, Vec2(blackPos[0].x, 0.0f), width, height, {}, Vec4(0.0f, 0.0f, 0.0f, 1.0f));
		Sprite::Get()->Draw(blackOut, Vec2(blackPos[1].x, 0.0f), width, height, {}, Vec4(0.0f, 0.0f, 0.0f, 1.0f));
	}
	else if (blackOutFlag == Latter_Half)
	{
		const float width = 1280.0f, height = 360.0f;
		Sprite::Get()->Draw(blackOut, Vec2(0.0f, blackPos[0].y), width, height, {}, Vec4(0.0f, 0.0f, 0.0f, 1.0f));
		Sprite::Get()->Draw(blackOut, Vec2(0.0f, blackPos[1].y), width, height, {}, Vec4(0.0f, 0.0f, 0.0f, 1.0f));
	}
}
