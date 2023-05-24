#include"TitleScene.h"
#include<sstream>
#include<iomanip>
#include"Camera.h"
#include"Shape.h"
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
	sprite = Sprite::Get()->SpriteCreate(L"Resources/gutitubo.png");
	PostEffect::Get()->SetPipeline(static_cast<int>(PostEffectType::NORMAL));
}

void TitleScene::Update()
{
	//シーンの変更の仕方
	if (Input::Get()->KeybordTrigger(DIK_SPACE))
	{
		BaseScene* scene = new SelectScene();
		sceneManager_->SetNextScene(scene);
	}
#ifdef _DEBUG
	else if (Input::Get()->KeybordTrigger(DIK_E))
	{
		// エディター
		BaseScene* scene = new EditerScene();
		sceneManager_->SetNextScene(scene);
	}
#endif // _DEBUG

	DebugText::Get()->Print(100.0f, 100.0f, 5, "Title");
#ifdef _DEBUG
	DebugText::Get()->Print(16.0f, window_height - 16.0f, 2, "E:Editer");
#endif // _DEBUG
}

void TitleScene::Draw()
{
	Vec3 position = {};
	Vec3 scale = { 1.0f,1.0f,1.0f };
	Vec3 angle = {};

	//2D描画
	Vec2 position2D = { 200.0f,200.0f };
	float width = 140.0f, height = 100.0f;
	Sprite::Get()->Draw(sprite, position2D, width, height);

}

void TitleScene::ShadowDraw()
{


}

void TitleScene::Finalize()
{
	Texture::Get()->Delete();
	Sprite::Get()->Delete();
}

bool TitleScene::GetEffect()
{
	return false;
}
