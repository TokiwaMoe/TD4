#include "GameScene.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"FbxLoader.h"
#include"Shape.h"
#include"SceneManager.h"
#include"ResultScene.h"

GameScene::GameScene()
{}
GameScene::~GameScene()
{
}

void GameScene::Init()
{
	//ライトグループクラス作成
	lightGroup = std::make_unique<LightGroup>();
	lightGroup->Initialize();
	// 3Dオブエクトにライトをセット
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,-1,0,0 });
	lightGroup->SetShadowDir(Vec3(0, 1, 0));
	//音データ読み込み
	//カメラ位置をセット
	Camera::Get()->SetCamera(Vec3{ 0,0,-200 }, Vec3{ 0, 0, 0 }, Vec3{ 0, 1, 0 });
	FBXObject3d::SetLight(lightGroup.get());
	Object::SetLight(lightGroup.get());

	//FBX関連
	Model* model1 = FbxLoader::GetInstance()->LoadModelFromFile("pengin", "FBX/");
	m_model = std::make_unique<Model>();
	m_model = std::unique_ptr<Model>(model1);
	m_fbx = std::make_unique<FBXObject3d>();
	m_fbx->Initialize();
	m_fbx->SetModel(m_model.get());
	m_fbx->SetScale(Vec3(0.025f, 0.025f, 0.025f));

	//スプライト作成の仕方
	background = Sprite::Get()->SpriteCreate(L"Resources/background.png");
	//ヤシの木
	palm_1 = Sprite::Get()->SpriteCreate(L"Resources/palm.png");
	palm_2 = Sprite::Get()->SpriteCreate(L"Resources/palm.png");

	player = new Player();
	// ステージ
	stage = Stage::GetInstance();
	stage->Init(player->GetSize());
	//プレイヤー
	player->Initialize();
	player->Init();
}

void GameScene::Update()
{
	//シーンの変更の仕方
	if (player->GetGoalFlag() == true)
	{
		BaseScene* scene = new ResultScene();
		sceneManager_->SetNextScene(scene);
	}

	//FBX関連
	m_fbx->SetRotation({30.0f,180.0f,0.0f});
	m_fbx->Update();
	if (Input::Get()->KeybordTrigger(DIK_UP))
	{
		m_fbx->PlayAnimation(true);
	}

	// ステージ出力（デバッグ実行用）
	if (Input::Get()->KeybordTrigger(DIK_1))
	{
		stage->WriteStage("write_test");
	}

	stage->GimmickUpdate();
	player->Update(stage);

	//DebugText::Get()->Print(100.0f, 100.0f, 10, "Game");

	lightGroup->Update();

	palmSize_1 = SizeChange({ 305, 437 }, { 0,40 }, true, 2.0f);
	palmSize_2 = SizeChange({ 305, 437 }, { 0,40 }, true, 2.0f);
}

Vec2 GameScene::SizeChange(Vec2 startSize, Vec2 lim, bool flag, float speed)
{
	Vec2 limit = startSize;
	// 軸単位で動かすかどうか
	const Vec2 isMove = {
		(lim.x ? 1.0f : 0.0f),
		(lim.y ? 1.0f : 0.0f)
	};

	if (isChange)
	{
		limit += lim;
		size += isMove * speed;

		if ((isMove.x && (size.x >= limit.x)) || (isMove.y && (size.y >= limit.y)))
		{
			isChange = false;
		}
	}
	else
	{
		limit -= lim;
		size -= isMove * speed;

		if ((isMove.x && (size.x <= limit.x)) || (isMove.y && (size.y <= limit.y)))
		{
			isChange = true;
		}
	}

	return size;
}

void GameScene::Draw()
{
	float width = 1280, height = 720;
	Sprite::Get()->Draw(background, { 0,0 }, width, height);
	
	Sprite::Get()->Draw(palm_1, { 0,720 }, palmSize_1.x, palmSize_1.y, {0.0f, 1.0f});
	Sprite::Get()->Draw(palm_2, { width,720 }, palmSize_2.x, palmSize_2.y, { 0,1.0f }, {1,1,1,1}, true);
	m_fbx->Draw(true);
	stage->Draw();
	player->Draw();
}

void GameScene::ShadowDraw()
{
	
}

void GameScene::Finalize()
{

}
