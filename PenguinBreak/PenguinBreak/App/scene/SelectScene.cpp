#include "SelectScene.h"
#include<Input.h>
#include"GameScene.h"
#include"SceneManager.h"
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
	hand_p = Sprite::Get()->SpriteCreate(L"Resources/hand_pa.png");
	hand_g = Sprite::Get()->SpriteCreate(L"Resources/hand_g.png");

	Vec2 size = {};
	for (int i = 0; i < Stage::STAGE_COUNT; i++)
	{
		stagePos[i] = { 430.0f + size.x * 200.0f,300.0f + size.y * 140.0f };
		size.x += 1.0f;
		if (i == 2) { size.x = 0.0f, size.y++; }
	}
}

void SelectScene::Update()
{

	//マウスでステージ決定
	if (Input::Get()->MousePushLeft()) {

		for (int i = 0; i < Stage::STAGE_COUNT; i++)
		{
			//円とレイの判定を使っている
			Box mBox = {};
			Box sBox = {};
			const Vec2 sSize = { 40.0f ,40.0f };
			const Vec2 mSize = { 6.0f,6.0f };
			if (Collision::BoxCollision(stagePos[i], Input::Get()->GetMousePos(), sSize, mSize)) {
				BaseScene* scene = new GameScene();
				scene->nextStage = i + 1;
				sceneManager_->SetNextScene(scene);
				break;
			}
		}
		//手のspを表示するか
		isDraw = true;
	}
	else
	{
		isDraw = false;
	}
}

void SelectScene::Draw()
{
	const float width = 1280, height = 720;
	Sprite::Get()->Draw(backGround[0], {}, width, height);
	Sprite::Get()->Draw(backGround[1], {}, width, height);
	const float length = 94.0f;

	for (int i = 0; i < Stage::STAGE_COUNT; i++)
	{
		Sprite::Get()->Draw(stageData[i], stagePos[i], length, length, { 0.5f,0.5f });
	}
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
{
}

void SelectScene::Finalize()
{
	Texture::Get()->Delete();
	Sprite::Get()->Delete();
}

bool SelectScene::GetEffect()
{
	return false;
}

