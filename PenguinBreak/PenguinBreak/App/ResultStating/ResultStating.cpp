#include "ResultStating.h"
#include <Collision.h>
#include <Stage.h>
#include<Input.h>

ResultStating::ResultStating()
{
}

ResultStating::~ResultStating()
{
}

void ResultStating::Init()
{
	backGround = Sprite::Get()->SpriteCreate(L"Resources/background.png");
	selectGraph = Sprite::Get()->SpriteCreate(L"Resources/Result/back.png");
	nextStageGraph = Sprite::Get()->SpriteCreate(L"Resources/Result/next.png");
	hand_p = Sprite::Get()->SpriteCreate(L"Resources/hand_pa.png");
	hand_g = Sprite::Get()->SpriteCreate(L"Resources/hand_g.png");

	clearGraph[0] = Sprite::Get()->SpriteCreate(L"Resources/Result/ku.png");
	clearGraph[1] = Sprite::Get()->SpriteCreate(L"Resources/Result/ri.png");
	clearGraph[2] = Sprite::Get()->SpriteCreate(L"Resources/Result/a.png");
	clearGraph[3] = Sprite::Get()->SpriteCreate(L"Resources/Result/-.png");

	houseHumanGraph = Sprite::Get()->SpriteCreate(L"Resources/kari.png");
	houseHumanGraph.rotation = 90.0f;

	for (size_t i = 0; i < 4; i++)
	{
		clearMovePos[i] = i * 5.0f;
		clearMoveFlag[i] = {};
	}
}

void ResultStating::Update(const int nextStage)
{
	//ステージ決定
	StageDecision(nextStage);
	//選択時大きさが変わる処理
	DecisionScale();
	//クリア文字動かす
	ClearMove();
	//馬人間の動き
	DoubleHMove();
}

void ResultStating::Draw(int nextStage)
{
	//背景
	const float width = 1280, height = 720;
	Sprite::Get()->Draw(backGround, {}, width, height);

	//馬人間
	const float houseWidth = 64.0f, houseHeight = 128.0f;
	Sprite::Get()->Draw(houseHumanGraph, houseHumanPos, houseWidth, houseHeight);


	//クリア文字
	const Vec2 clearSize = { 133.0f,195.0f };
	const Vec2 clearBasicPos = { 276.0f,64.0f };
	float clearMove = 220.0f;
	for (int i = 0; i < 4; i++)
	{
		Sprite::Get()->Draw(clearGraph[i], Vec2(clearBasicPos.x + clearMove * i, clearBasicPos.y + clearMovePos[i]),
			clearSize.x, clearSize.y);
	}

	//もどる
	if (scaleNumber == BACK)
	{
		Sprite::Get()->Draw(selectGraph, selectPos, selectSize.x * scale, selectSize.y * scale, { 0.5f,0.5f });
	}
	else
	{
		Sprite::Get()->Draw(selectGraph, selectPos, selectSize.x, selectSize.y, { 0.5f,0.5f });
	}
	//つぎへ
	if (nextStage < Stage::STAGE_COUNT + 1)
	{
		if (scaleNumber == NEXT)
		{
			Sprite::Get()->Draw(nextStageGraph, nextStagePos, selectSize.x * scale, selectSize.y * scale, { 0.5f,0.5f });
		}
		else
		{
			Sprite::Get()->Draw(nextStageGraph, nextStagePos, selectSize.x, selectSize.y, { 0.5f,0.5f });
		}
	}
	//手
	Vec2 hPos = { Input::Get()->GetMousePos().x,Input::Get()->GetMousePos().y };
	if (isDraw)
	{
		Sprite::Get()->Draw(hand_g, hPos, 32, 32, { 0.5f,0.5f });
	}
	else
	{
		Sprite::Get()->Draw(hand_p, hPos, 32, 32, { 0.5f,0.5f });
	}
}

void ResultStating::StageDecision(const int nextStage)
{
	//ステージの数字
	const Vec2 mSize = { 1.0f,1.0f };
	//マウスでステージ決定
	if (Input::Get()->MousePushLeft()) {

		//ステージセレクトへ
		if (Collision::BoxCollision(selectPos, Input::Get()->GetMousePos(), selectSize, mSize)) {
			nextSceneStatus = BACK;
		}
		//次のステージへ
		if (nextStage < Stage::STAGE_COUNT + 1)
		{
			if (Collision::BoxCollision(nextStagePos, Input::Get()->GetMousePos(), selectSize, mSize)) {
				nextSceneStatus = NEXT;
			}
		}
		//手のspを表示するか
		isDraw = true;
	}
	else
	{
		scaleNumber = -1;//リセット数字
		if (Collision::BoxCollision(selectPos, Input::Get()->GetMousePos(), selectSize / 2, mSize)) {
			scaleNumber = BACK;
		}
		//タイトル戻るボタン
		if (Collision::BoxCollision(nextStagePos, Input::Get()->GetMousePos(), selectSize / 2, mSize)) {
			scaleNumber = NEXT;
		}
		isDraw = false;
	}
}

void ResultStating::DecisionScale()
{
	const float scaleMax = 1.0f;
	const float scaleMin = 0.8f;
	const float speed = 0.01f;
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

void ResultStating::ClearMove()
{
	const float moveMax = 10.0f, moveMin = -10.0f, speed = 0.3f;
	for (size_t i = 0; i < 4; i++)
	{
		if (clearMoveFlag[i] == true)
		{
			clearMovePos[i] += speed;
			if (clearMovePos[i] > moveMax)
			{
				clearMoveFlag[i] = false;

			}
		}
		else
		{
			clearMovePos[i] -= speed;
			if (clearMovePos[i] < moveMin)
			{
				clearMoveFlag[i] = true;

			}
		}
	}
}

void ResultStating::DoubleHMove()
{
	const float widthMax = 1440.0f, widthMin = -250.0f, speed = 3.0f;

	if (houseHumanPos.x > widthMax)
	{
		houseHumanPos.x = -widthMin;
	}
	houseHumanPos.x += speed;





}
