#include"TitleScene.h"
#include<sstream>
#include<iomanip>
#include"Input.h"
#include"Texture.h"
#include"SceneManager.h"
#include"EditerScene.h"
#include"SelectScene.h"
TitleScene::TitleScene()
{}
TitleScene::~TitleScene()
{}

void TitleScene::Init()
{
	//スプライト作成の仕方
	background = Sprite::Get()->SpriteCreate(L"Resources/background.png");

	titleGraph = Sprite::Get()->SpriteCreate(L"Resources/Title/title_test.png");
	button = Sprite::Get()->SpriteCreate(L"Resources/Title/start.png");
	hand_p = Sprite::Get()->SpriteCreate(L"Resources/hand_pa.png");
	hand_g = Sprite::Get()->SpriteCreate(L"Resources/hand_g.png");

	PostEffect::Get()->SetPipeline(static_cast<int>(PostEffectType::NORMAL));
}

void TitleScene::Update()
{
#ifdef _DEBUG
	//シーンの変更の仕方
	if (Input::Get()->KeybordTrigger(DIK_SPACE))
	{
		BaseScene* scene = new SelectScene();
		sceneManager_->SetNextScene(scene);
	}
	else if (Input::Get()->KeybordTrigger(DIK_E))
	{
		// エディター
		BaseScene* scene = new EditerScene();
		sceneManager_->SetNextScene(scene);
	}
#endif // _DEBUG

#ifdef _DEBUG
	DebugText::Get()->Print(16.0f, window_height - 16.0f, 2, "E:Editer");
#endif // _DEBUG
	StageDecision();

	DecisionScale();
}

void TitleScene::Draw()
{
	//背景
	const float width = 1280, height = 720;
	Sprite::Get()->Draw(background, Vec2(), width, height);

	Vec2  anchorpoint = { 0.5f,0.5f };
	Sprite::Get()->Draw(titleGraph, { 0,0 }, width, height);

	if (isScale)
	{
		Sprite::Get()->Draw(button, bPos, bSize.x * scale, bSize.y * scale, anchorpoint);
	}
	else
	{
		Sprite::Get()->Draw(button, bPos, bSize.x, bSize.y, anchorpoint);
	}
	//手
	if (isDraw)
	{
		Sprite::Get()->Draw(hand_g, hPos, 32, 32, anchorpoint);
	}
	else
	{
		Sprite::Get()->Draw(hand_p, hPos, 32, 32, anchorpoint);
	}
}

void TitleScene::ShadowDraw()
{}

void TitleScene::Finalize()
{
	Texture::Get()->Delete();
	Sprite::Get()->Delete();
}

bool TitleScene::GetEffect()
{
	return false;
}

void TitleScene::StageDecision()
{
	const Vec2 mSize = { 6.0f,6.0f };
	//手の動きの処理
	if (Input::Get()->MousePushLeft())
	{
		isDraw = true;
		if (Collision::BoxCollision(bPos, Input::Get()->GetMousePos(), bSize - Vec2(180.0f, 80.0f), mSize)){
			isScale = true;
		}
		else
		{
			isScale = false;
		}
	}
	else 
	{
		if (Collision::BoxCollision(bPos, Input::Get()->GetMousePos(), bSize - Vec2(180.0f, 80.0f), mSize))
		{
			isScale = true;
		}
		else
		{
			isScale = false;
		}
		isDraw = false;
	}
	//次のシーンへ
	if (Input::Get()->MouseReleaseLeft()) {
		if (Collision::BoxCollision(bPos, Input::Get()->GetMousePos(), bSize - Vec2(180.0f, 80.0f), mSize)) {
			BaseScene* scene = new SelectScene();
			sceneManager_->SetNextScene(scene);
		}
	}
	hPos = Input::Get()->GetMousePos();
}

void TitleScene::DecisionScale()
{
	const float scaleMax = 1.05f;
	const float scaleMin = 0.95f;
	if (decScaleFlag)
	{
		scale += 0.005f;
		if (scaleMax < scale)
		{
			decScaleFlag = false;
		}
	}
	else
	{
		scale -= 0.005f;
		if (scaleMin > scale)
		{
			decScaleFlag = true;
		}
	}
}
