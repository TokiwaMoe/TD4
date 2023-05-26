#include "ResultScene.h"
#include"DebugText.h"
#include"Shape.h"
#include <FBXObject3d.h>
#include<Input.h>
#include"TitleScene.h"
#include"SceneManager.h"
#include <PostEffect.h>
ResultScene::ResultScene()
{}
ResultScene::~ResultScene()
{}
void ResultScene::Init()
{	
	PostEffect::Get()->SetPipeline(static_cast<int>(PostEffectType::NORMAL));
}

void ResultScene::Update()
{
	
	//シーンの変更の仕方
	if (Input::Get()->KeybordTrigger(DIK_SPACE))
	{
		BaseScene* scene = new TitleScene();
		sceneManager_->SetNextScene(scene);
	}


	DebugText::Get()->Print(100.0f, 100.0f, 10, "Result");
}

void ResultScene::Draw()
{
	
}

void ResultScene::ShadowDraw()
{
	
}

void ResultScene::Finalize()
{
	Texture::Get()->Delete();
	Sprite::Get()->Delete();
}

bool ResultScene::GetEffect()
{
	return false;
}
