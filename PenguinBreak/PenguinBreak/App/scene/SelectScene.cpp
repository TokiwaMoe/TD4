#include "SelectScene.h"
#include<Input.h>
#include"GameScene.h"
#include"SceneManager.h"
#include"TitleScene.h"
#include"PostEffect.h"
SelectScene::SelectScene()
{
}

SelectScene::~SelectScene()
{
}

void SelectScene::Init()
{
	//スプライト作成の仕方
	backGround[0] = Sprite::Get()->SpriteCreate(L"Resources/background.png");
	backGround[1] = Sprite::Get()->SpriteCreate(L"Resources/Select/select.png");


	stageData[0] = Sprite::Get()->SpriteCreate(L"Resources/Select/selectNum1.png");
	stageData[1] = Sprite::Get()->SpriteCreate(L"Resources/Select/selectNum2.png");
	stageData[2] = Sprite::Get()->SpriteCreate(L"Resources/Select/selectNum3.png");
	stageData[3] = Sprite::Get()->SpriteCreate(L"Resources/Select/selectNum4.png");
	stageData[4] = Sprite::Get()->SpriteCreate(L"Resources/Select/selectNum5.png");
	stageData[5] = Sprite::Get()->SpriteCreate(L"Resources/Select/selectNum6.png");

	backButton = Sprite::Get()->SpriteCreate(L"Resources/Select/backButton.png");

	hand_p = Sprite::Get()->SpriteCreate(L"Resources/hand_pa.png");
	hand_g = Sprite::Get()->SpriteCreate(L"Resources/hand_g.png");

	Vec2 size = {};
	for (int i = 0; i < Stage::STAGE_COUNT; i++)
	{
		stagePos[i] = { 430.0f + size.x * 200.0f,300.0f + size.y * 140.0f };
		size.x += 1.0f;
		if (i == 2) { size.x = 0.0f, size.y++; }
	}

	PostEffect::Get()->SetPipeline(static_cast<int>(PostEffectType::NORMAL));
}

void SelectScene::Update()
{
	StageDecision();

	DecisionScale();
}

void SelectScene::Draw()
{
	const float width = 1280, height = 720;
	Sprite::Get()->Draw(backGround[0], {}, width, height);
	Sprite::Get()->Draw(backGround[1], {}, width, height);
	const float length = 94.0f;
	//ステージ数字
	for (int i = 0; i < Stage::STAGE_COUNT; i++)
	{
		if (scaleNumber == i)
		{
			Sprite::Get()->Draw(stageData[i], stagePos[i], length * scale, length * scale, { 0.5f,0.5f });
		}
		else
		{
			Sprite::Get()->Draw(stageData[i], stagePos[i], length, length, { 0.5f,0.5f });
		}
	}
	//戻るボタン
	if (scaleNumber == -2)
	{
		Sprite::Get()->Draw(backButton, backButtonPos, length * scale, length * scale, { 0.5f,0.5f });
	}
	else
	{
		Sprite::Get()->Draw(backButton, backButtonPos, length, length, { 0.5f,0.5f });
	}
	//手
	if (isDraw)
	{
		Vec2 hPos = { Input::Get()->GetMousePos().x,Input::Get()->GetMousePos().y };
		Sprite::Get()->Draw(hand_g, hPos, 32, 32, { 0.5f,0.5f });
	}
	else
	{
		Vec2 hPos = { Input::Get()->GetMousePos().x,Input::Get()->GetMousePos().y };
		Sprite::Get()->Draw(hand_p, hPos, 32, 32, { 0.5f,0.5f });
	}
}

void SelectScene::ShadowDraw()
{}

void SelectScene::Finalize()
{
	Texture::Get()->Delete();
	Sprite::Get()->Delete();
}

bool SelectScene::GetEffect()
{
	return false;
}

void SelectScene::StageDecision()
{
	//ステージの数字
	const Vec2 sSize = { 40.0f ,40.0f };
	const Vec2 mSize = { 6.0f,6.0f };
	//マウスでステージ決定
	if (Input::Get()->MousePushLeft()) {
		for (int i = 0; i < Stage::STAGE_COUNT; i++)
		{
			if (Collision::BoxCollision(stagePos[i], Input::Get()->GetMousePos(), sSize, mSize)) {
				BaseScene* scene = new GameScene();
				scene->nextStage = i + 1;
				sceneManager_->SetNextScene(scene);
				break;
			}
		}
		//タイトル戻るボタン
		if (Collision::BoxCollision(backButtonPos, Input::Get()->GetMousePos(), sSize, mSize)) {
			BaseScene* scene = new TitleScene();
			sceneManager_->SetNextScene(scene);
		}
		//手のspを表示するか
		isDraw = true;
	}
	else
	{
		scaleNumber = -1;//リセット数字
		for (int i = 0; i < Stage::STAGE_COUNT; i++)
		{
			if (Collision::BoxCollision(stagePos[i], Input::Get()->GetMousePos(), sSize, mSize)) {
				scaleNumber = i;
			}
		}
		//タイトル戻るボタン
		if (Collision::BoxCollision(backButtonPos, Input::Get()->GetMousePos(), sSize, mSize)) {
			scaleNumber = -2;
		}
		isDraw = false;
	}
}

void SelectScene::DecisionScale()
{
	const float scaleMax = 1.0f;
	const float scaleMin = 0.8f;
	if (decScaleFlag == true)
	{
		scale += 0.01f;
		if (scale >= scaleMax)
		{
			decScaleFlag = false;
		}
	}
	else
	{
		scale -= 0.01f;
		if (scale <= scaleMin)
		{
			decScaleFlag = true;
		}
	}
}

