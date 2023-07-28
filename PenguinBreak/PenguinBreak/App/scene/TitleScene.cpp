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
{
	audio->SoundUnload(&sound);
	audio->SoundUnload(&sound2);
}

void TitleScene::Init()
{
	//スプライト作成の仕方
	background = Sprite::Get()->SpriteCreate(L"Resources/Title/title_back.png");
	//手
	hand = Sprite::Get()->SpriteCreate(L"Resources/Title/hand.png");
	//馬
	player = Sprite::Get()->SpriteCreate(L"Resources/Title/uma.png");
	//なまけもの
	lazy = Sprite::Get()->SpriteCreate(L"Resources/Title/lazy.png");
	//猫
	cat = Sprite::Get()->SpriteCreate(L"Resources/Title/cat.png");
	//目
	eye = Sprite::Get()->SpriteCreate(L"Resources/Title/eye.png");
	//ふぐ
	fugu = Sprite::Get()->SpriteCreate(L"Resources/Title/fugu.png");

	titleGraph = Sprite::Get()->SpriteCreate(L"Resources/Title/title.png");
	button = Sprite::Get()->SpriteCreate(L"Resources/Title/start.png");
	hand_p = Sprite::Get()->SpriteCreate(L"Resources/hand_pa.png");
	hand_g = Sprite::Get()->SpriteCreate(L"Resources/hand_g.png");
	PostEffect::Get()->SetPipeline(static_cast<int>(PostEffectType::NORMAL));
	sound = Audio::SoundLoadWave("Resources/Sound/uma.wav");
	sound2 = Audio::SoundLoadWave("Resources/Sound/title.wav");
	audio->SoundBGMPlayLoopWave(sound2, audio->BGM);
}

void TitleScene::Update()
{
#ifdef _DEBUG
	//シーンの変更の仕方
	if (Input::Get()->KeybordTrigger(DIK_SPACE))
	{
		BaseScene* scene = new SelectScene();
		sceneManager_->SetNextScene(scene);
		audio->SoundStop(sound2,audio->BGM);
	}
	else if (Input::Get()->KeybordTrigger(DIK_E))
	{
		// エディター
		BaseScene* scene = new EditerScene();
		sceneManager_->SetNextScene(scene);
		audio->SoundStop(sound2, audio->BGM);
	}
#endif // _DEBUG

#ifdef _DEBUG
	DebugText::Get()->Print(16.0f, window_height - 16.0f, 2, "E:Editer");
#endif // _DEBUG
	StageDecision();

	DecisionScale();

	ChangeScale();

	//カーソル表示
	Input::Get()->SetCursor(false);
}

void TitleScene::Draw()
{
	Vec2  anchorPoint = { 0.5f,0.5f };
	//背景
	const float width = 1280, height = 720;
	Sprite::Get()->Draw(background, Vec2(), width, height);
	//なまけ
	Sprite::Get()->Draw(lazy, { 670,360 }, width * changeScale, height * changeScale, anchorPoint);
	//タイトル
	Sprite::Get()->Draw(titleGraph, { 640,360 }, width, height, anchorPoint);
	//猫
	Sprite::Get()->Draw(cat, { 600,390 }, width * changeScale, height * changeScale, anchorPoint);
	//手
	Sprite::Get()->Draw(hand, { 600,340 }, width * changeScale, height * changeScale, anchorPoint);
	//ふぐ
	Sprite::Get()->Draw(fugu, { 680,340 }, width * changeScale, height * changeScale, anchorPoint);
	//馬人間
	Sprite::Get()->Draw(player, { 620,340 }, width, height, anchorPoint);
	//目
	Sprite::Get()->Draw(eye, { 680,340 }, width * changeScale, height * changeScale, anchorPoint);

	if (isScale)
	{
		Sprite::Get()->Draw(button, bPos, bSize.x * scale, bSize.y * scale, anchorPoint);
	}
	else
	{
		Sprite::Get()->Draw(button, bPos, bSize.x, bSize.y, anchorPoint);
	}
	//手
	if (isDraw)
	{
		Sprite::Get()->Draw(hand_g, hPos, 32, 32, anchorPoint);
	}
	else
	{
		Sprite::Get()->Draw(hand_p, hPos, 32, 32, anchorPoint);
	}
}

void TitleScene::ShadowDraw()
{}

void TitleScene::Finalize()
{
	Texture::Get()->Delete();
	Sprite::Get()->Delete();
	audio->SoundUnload(&sound);
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
		if (Collision::BoxCollision(bPos, Input::Get()->GetMousePos(), bSize - Vec2(180.0f, 80.0f), mSize)) {
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
			audio->SoundSEPlayWave(sound);
			BaseScene* scene = new SelectScene();
			sceneManager_->SetNextScene(scene);
			audio->SoundStop(sound2, audio->BGM);
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

void TitleScene::ChangeScale()
{
	const float scaleMax = 1.0f;
	const float scaleMin = 0.97f;
	if (isChange)
	{
		changeScale += 0.01f;
		if (scaleMax < changeScale)
		{
			isChange = false;
		}
	}
	else
	{
		changeScale -= 0.01f;
		if (scaleMin > changeScale)
		{
			isChange = true;
		}
	}
}
