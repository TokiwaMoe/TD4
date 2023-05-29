#include "ResultScene.h"
#include"DebugText.h"
#include<Input.h>
#include"SceneManager.h"
#include <PostEffect.h>
#include"SelectScene.h"
#include"GameScene.h"
ResultScene::ResultScene()
{}
ResultScene::~ResultScene()
{}
void ResultScene::Init()
{	
	PostEffect::Get()->SetPipeline(static_cast<int>(PostEffectType::NORMAL));
	resultStating.Init();
}

void ResultScene::Update()
{
	
	resultStating.Update(nextStage);

	//ステージセレクトへ
	if (resultStating.NextSceneStatus() == ResultStating::BACK) {
		BaseScene* scene = new SelectScene();
		sceneManager_->SetNextScene(scene);
	}
	//次のステージへ
	if (resultStating.NextSceneStatus() == ResultStating::NEXT) {
		BaseScene* scene = new GameScene();
		scene->nextStage = nextStage;
		sceneManager_->SetNextScene(scene);
	}
}

void ResultScene::Draw()
{
	resultStating.Draw(nextStage);
}

void ResultScene::ShadowDraw()
{}

void ResultScene::Finalize()
{
	Texture::Get()->Delete();
	Sprite::Get()->Delete();
}

bool ResultScene::GetEffect()
{
	return false;
}
