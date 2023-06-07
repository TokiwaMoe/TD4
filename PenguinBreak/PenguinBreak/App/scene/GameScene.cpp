#include "GameScene.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"FbxLoader.h"
#include"Shape.h"
#include"SceneManager.h"
#include"ResultScene.h"
#include"EditerScene.h"
#include <PostEffect.h>

bool GameScene::isEditer = false;

GameScene::GameScene()
{}
GameScene::~GameScene()
{
}

void GameScene::Init()
{
	PostEffect::Get()->SetPipeline(static_cast<int>(PostEffectType::WATERRIPPLES));
	//スプライト作成の仕方
	background = Sprite::Get()->SpriteCreate(L"Resources/background.png");

	player = std::make_unique<Player>();
	// ステージ
	stage = Stage::GetInstance();
	stage->Init();
	stageNumber = nextStage;
	if (isEditer == false)
	{
		stage->ChengeStage(stageNumber);
	}
	stage->ChengeBack(backNumber);
	//プレイヤー
	player->Initialize();
	player->Init(stage);
}

void GameScene::Update()
{
	//シーンの変更の仕方
	if (player->GetGoalFlag() == true
#ifdef _DEBUG
		|| Input::Get()->KeybordTrigger(DIK_SPACE)
#endif // _DEBUG
		)
	{
		if (isEditer == true)
		{
			isEditer = false;
			BaseScene* scene = new EditerScene();
			sceneManager_->SetNextScene(scene);
		}
		else
		{
			// ステージ切り替え
			BaseScene* scene = new ResultScene();
			scene->nextStage = stageNumber + 1;
			sceneManager_->SetNextScene(scene);
		}
	}

	stage->Update();
	player->Update(stage);

}
void GameScene::Draw()
{
	float width = 1280, height = 720;
	Sprite::Get()->Draw(background, { 0,0 }, width, height);
	//stage->Draw();
	player->Draw();

	if (isEditer == true)
	{
		DebugText::Get()->Print(100.0f, 100.0f, 2, "Clear Check");
	}
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
