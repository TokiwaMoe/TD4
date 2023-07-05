#include "SelectScene.h"
#include<Input.h>
#include"GameScene.h"
#include"SceneManager.h"
#include"TitleScene.h"
#include"PostEffect.h"
SelectScene::SelectScene()
{}

SelectScene::~SelectScene()
{}

void SelectScene::Init()
{
	//スプライト作成の仕方
	backGround[0] = Sprite::Get()->SpriteCreate(L"Resources/Select/selectBack.png");
	backGround[1] = Sprite::Get()->SpriteCreate(L"Resources/Select/frame.png");

	stageData[0] = Sprite::Get()->SpriteCreate(L"Resources/Select/selectNum1.png");
	stageData[1] = Sprite::Get()->SpriteCreate(L"Resources/Select/selectNum2.png");
	stageData[2] = Sprite::Get()->SpriteCreate(L"Resources/Select/selectNum3.png");
	stageData[3] = Sprite::Get()->SpriteCreate(L"Resources/Select/selectNum4.png");
	stageData[4] = Sprite::Get()->SpriteCreate(L"Resources/Select/selectNum5.png");
	stageData[5] = Sprite::Get()->SpriteCreate(L"Resources/Select/selectNum6.png");
	stageData[6] = Sprite::Get()->SpriteCreate(L"Resources/Select/selectNum7.png");
	stageData[7] = Sprite::Get()->SpriteCreate(L"Resources/Select/selectNum8.png");
	stageData[8] = Sprite::Get()->SpriteCreate(L"Resources/Select/selectNum9.png");
	stageData[9] = Sprite::Get()->SpriteCreate(L"Resources/Select/selectNum10.png");
	stageData[10] = Sprite::Get()->SpriteCreate(L"Resources/Select/selectNum11.png");
	stageData[11] = Sprite::Get()->SpriteCreate(L"Resources/Select/selectNum12.png");
	stageData[12] = Sprite::Get()->SpriteCreate(L"Resources/Select/selectNum13.png");

	selectGraph[0] = Sprite::Get()->SpriteCreate(L"Resources/Select/backButton.png");
	selectGraph[1] = Sprite::Get()->SpriteCreate(L"Resources/Select/selectRight.png");
	selectGraph[2] = Sprite::Get()->SpriteCreate(L"Resources/Select/selectLeft.png");

	hand_p = Sprite::Get()->SpriteCreate(L"Resources/hand_pa.png");
	hand_g = Sprite::Get()->SpriteCreate(L"Resources/hand_g.png");

	rule = Sprite::Get()->SpriteCreate(L"Resources/rule.png");

	Vec2 size = {};
	selectMax = 0;
	for (int i = 0; i < Stage::STAGE_COUNT; i++)
	{
		stagePos[i] = { 430.0f + size.x * 200.0f + selectMax * window_width,300.0f + size.y * 140.0f };
		size.x += 1.0f;
		if (i % 3 == 2) { size.x = 0.0f, size.y++; }
		if (i % 6 == 5) { size = {}, selectMax++; }
	}

	PostEffect::Get()->SetPipeline(static_cast<int>(PostEffectType::NORMAL));
}

void SelectScene::Update()
{
	StageDecision();

	DecisionScale();

	Move();
}

void SelectScene::Draw()
{
	const float width = 1280, height = 720;
	Sprite::Get()->Draw(backGround[0], {}, width, height);
	Sprite::Get()->Draw(rule, rulePos, 500, 600, { 0.5,0.5 });
	Sprite::Get()->Draw(backGround[1], {}, width, height);

	const float length = 94.0f;

	//ステージ数字
	for (int i = 0; i < Stage::STAGE_COUNT; i++)
	{
		if (scaleNumber == i)
		{
			Sprite::Get()->Draw(stageData[i], Vec2(stagePos[i].x - selectNum * window_width, stagePos[i].y),
				length * scale, length * scale, { 0.5f,0.5f });
		}
		else
		{
			Sprite::Get()->Draw(stageData[i], Vec2(stagePos[i].x - selectNum * window_width, stagePos[i].y),
				length, length, { 0.5f,0.5f });
		}
	}

	//右左矢印

	if (scaleNumber == BACKSELECT)
	{
		Sprite::Get()->Draw(selectGraph[0], selectPos[0],
			length * scale, length * scale, { 0.5f,0.5f });
	}
	else
	{
		Sprite::Get()->Draw(selectGraph[0], selectPos[0],
			length, length, { 0.5f,0.5f });
	}
	if (selectNum != selectMax)
	{
		if (scaleNumber == RIGHTSELECT)
		{
			Sprite::Get()->Draw(selectGraph[1], selectPos[1],
				length * scale, length * scale, { 0.5f,0.5f });
		}
		else
		{
			Sprite::Get()->Draw(selectGraph[1], selectPos[1],
				length, length, { 0.5f,0.5f });
		}
	}
	if (selectNum != SELECTONE)
	{
		if (scaleNumber == LEFTSELECT)
		{
			Sprite::Get()->Draw(selectGraph[2], selectPos[2],
				length * scale, length * scale, { 0.5f,0.5f });
		}
		else
		{
			Sprite::Get()->Draw(selectGraph[2], selectPos[2],
				length, length, { 0.5f,0.5f });
		}
	}

	//手
	Vec2 hPos = Input::Get()->GetMousePos();
	if (isDraw)
	{
		Sprite::Get()->Draw(hand_g, hPos, 32, 32, { 0.5f,0.5f });
	}
	else
	{
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
	//演出処理
	if (Input::Get()->MousePushLeft())
	{
		//手のspを表示するか
		isDraw = true;
		HandProcess();

	}
	else
	{
		isDraw = false;
		HandProcess();
	}
	//ステージの数字
	const Vec2 sSize = { 40.0f ,40.0f };
	const Vec2 mSize = { 6.0f,6.0f };
	//マウスでステージ決定
	if (Input::Get()->MouseReleaseLeft()) {
		for (int i = 0; i < Stage::STAGE_COUNT; i++)
		{
			if (Collision::BoxCollision(Vec2(stagePos[i].x - selectNum * window_width, stagePos[i].y), Input::Get()->GetMousePos(), sSize, mSize)) {
				BaseScene* scene = new GameScene();
				scene->nextStage = i + 1;
				sceneManager_->SetNextScene(scene);
				break;
			}
		}
		//タイトル戻るボタン
		if (Collision::BoxCollision(selectPos[0], Input::Get()->GetMousePos(), sSize, mSize)) {
			BaseScene* scene = new TitleScene();
			sceneManager_->SetNextScene(scene);
		}
		if (Collision::BoxCollision(selectPos[1], Input::Get()->GetMousePos(), sSize, mSize)
			&& selectNum != selectMax) {

			selectNum++;
		}
		if (Collision::BoxCollision(selectPos[2], Input::Get()->GetMousePos(), sSize, mSize)
			&& selectNum != SELECTONE) {

			selectNum--;
		}
	}
}

void SelectScene::DecisionScale()
{
	const float scaleMax = 1.0f, scaleMin = 0.8f, speed = 0.01f;
	if (decScaleFlag == true)
	{
		scale += speed;
		if (scale >= scaleMax)
		{
			decScaleFlag = false;
		}
	}
	else
	{
		scale -= speed;
		if (scale <= scaleMin)
		{
			decScaleFlag = true;
		}
	}
}

void SelectScene::Move()
{
	Vec2 limit = { 670,430 };
	Vec2 lim = { 300,0 };
	const float speed = 3;
	// 軸単位で動かすかどうか
	const Vec2 isMove = {
		(lim.x ? 1.0f : 0.0f),
		(lim.y ? 1.0f : 0.0f)
	};

	if (isChange)
	{
		// +方向に移動する
		limit += lim;
		rulePos += isMove * speed;

		if ((isMove.x && (rulePos.x >= limit.x)) || (isMove.y && (rulePos.y >= limit.y)))
		{
			isChange = false;
		}
	}
	else
	{
		// -方向に移動する
		limit -= lim;
		rulePos -= isMove * speed;

		if ((isMove.x && (rulePos.x <= limit.x)) || (isMove.y && (rulePos.y <= limit.y)))
		{
			isChange = true;
		}
	}
}

void SelectScene::HandProcess()
{
	//ステージの数字
	const Vec2 sSize = { 40.0f ,40.0f };
	const Vec2 mSize = { 6.0f,6.0f };
	scaleNumber = RESETSELECT;//リセット数字
	for (int i = 0; i < Stage::STAGE_COUNT; i++)
	{
		if (Collision::BoxCollision(Vec2(stagePos[i].x - selectNum * window_width, stagePos[i].y),
			Input::Get()->GetMousePos(), sSize, mSize)) {
			scaleNumber = i;
		}
	}
	for (int i = 0; i < 3; i++)
	{
		//タイトル戻るボタン
		if (Collision::BoxCollision(selectPos[i], Input::Get()->GetMousePos(), sSize, mSize)) {
			scaleNumber = BACKSELECT - i;
		}
	}
}