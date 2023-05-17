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

	//スプライト作成の仕方
	background = Sprite::Get()->SpriteCreate(L"Resources/background.png");

	player = std::make_unique<Player>();
	// ステージ
	stage = Stage::GetInstance();
	stage->Init();
	stage->ChengeStage(stageNumber);
	//プレイヤー
	player->Initialize();
	player->Init(stage);
}

void GameScene::Update()
{
	//シーンの変更の仕方
	if (player->GetGoalFlag() == true ||
		Input::Get()->KeybordTrigger(DIK_SPACE))
	{
		if (stageNumber < Stage::STAGE_COUNT)
		{
			// ステージ切り替え
			stageNumber++;
			stage->ChengeStage(stageNumber);
			player->Init(stage);
		}
		else
		{
			BaseScene* scene = new ResultScene();
			sceneManager_->SetNextScene(scene);
		}
	}

	// ステージ出力（デバッグ実行用）
	if (Input::Get()->KeybordTrigger(DIK_1))
	{
		stage->WriteStage("write_test");
	}

	stage->GimmickUpdate();
	player->Update(stage);

	lightGroup->Update();
}
void GameScene::Draw()
{
	float width = 1280, height = 720;
	Sprite::Get()->Draw(background, { 0,0 }, width, height);
	stage->Draw();
	player->Draw();
}

void GameScene::ShadowDraw()
{
	
}

void GameScene::Finalize()
{
	Texture::Get()->Delete();
	Sprite::Get()->Delete();
}

bool GameScene::GetEffect()
{
	return player->GetEffect();
}
