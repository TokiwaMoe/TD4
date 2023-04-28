#include "GameScene.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"FbxLoader.h"
#include"Shape.h"
#include"SceneManager.h"
#include"ResultScene.h"


GameScene::GameScene()
{}
GameScene::~GameScene()
{
}

void GameScene::Init()
{
	//ライトグループクラス作成
	lightGroup = std::make_unique<LightGroup>();
	lightGroup->Initialize();
	// 3Dオブエクトにライトをセット
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,-1,0,0 });
	lightGroup->SetShadowDir(Vec3(0, 1, 0));
	//音データ読み込み
	//カメラ位置をセット
	Camera::Get()->SetCamera(Vec3{ 0,0,-200 }, Vec3{ 0, 0, 0 }, Vec3{ 0, 1, 0 });
	FBXObject3d::SetLight(lightGroup.get());
	Object::SetLight(lightGroup.get());

	//FBX関連
	Model* model1 = FbxLoader::GetInstance()->LoadModelFromFile("pengin", "FBX/");
	m_model = std::make_unique<Model>();
	m_model = std::unique_ptr<Model>(model1);
	m_fbx = std::make_unique<FBXObject3d>();
	m_fbx->Initialize();
	m_fbx->SetModel(m_model.get());
	m_fbx->SetScale(Vec3(0.025f, 0.025f, 0.025f));

	//スプライト作成の仕方
	background = Sprite::Get()->SpriteCreate(L"Resources/background.png");

	//プレイヤー
	player = new Player();
	player->Initialize();
	player->Init();
	stage = Stage::GetInstance();
	stage->Init();
}

void GameScene::Update()
{
	//シーンの変更の仕方
	if (Input::Get()->KeybordTrigger(DIK_SPACE))
	{
		BaseScene* scene = new ResultScene();
		sceneManager_->SetNextScene(scene);
	}

	//FBX関連
	m_fbx->SetRotation({30.0f,180.0f,0.0f});
	m_fbx->Update();
	if (Input::Get()->KeybordTrigger(DIK_UP))
	{
		m_fbx->PlayAnimation(true);
	}

	// ステージ出力（デバッグ実行用）
	if (Input::Get()->KeybordTrigger(DIK_1))
	{
		stage->WriteStage("write_test");
	}

	player->Update();

	DebugText::Get()->Print(100.0f, 100.0f, 10, "Game");

	lightGroup->Update();
}

void GameScene::Draw()
{
	float width = 1280, height = 720;
	Sprite::Get()->Draw(background, { 0,0 }, width, height);
	m_fbx->Draw(true);
	stage->Draw();
	player->Draw();
}

void GameScene::ShadowDraw()
{
	
}


void GameScene::Finalize()
{

}

