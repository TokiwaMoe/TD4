#include"TitleScene.h"
#include<sstream>
#include<iomanip>
#include"Camera.h"
#include"Shape.h"
#include"Input.h"
#include"Texture.h"
#include"SceneManager.h"
#include"GameScene.h"
#include"EditerScene.h"
TitleScene::TitleScene()
{}
TitleScene::~TitleScene()
{}

void TitleScene::Init()
{
	// ライトグループクラス作成
	lightGroup = std::make_unique<LightGroup>();
	lightGroup->Initialize();
	// 3Dオブエクトにライトをセット
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,-1,0,0 });
	lightGroup->SetShadowDir(Vec3(0, 1, 0));

	//スプライト作成の仕方
	sprite = Sprite::Get()->SpriteCreate(L"Resources/gutitubo.png");

	//カメラ設定
	Camera::Get()->SetCamera(Vec3{ 0,0,-35 }, Vec3{ 0, 0, 0 }, Vec3{ 0, 1, 0 });
	FBXObject3d::SetLight(lightGroup.get());
	Object::SetLight(lightGroup.get());
}

void TitleScene::Update()
{
	//シーンの変更の仕方
	if (Input::Get()->KeybordTrigger(DIK_SPACE))
	{
		BaseScene* scene = new GameScene();
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
	lightGroup->Update();
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


}

bool TitleScene::GetEffect()
{
	return false;
}
