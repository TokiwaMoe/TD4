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


	//スプライト作成の仕方
	background = Sprite::Get()->SpriteCreate(L"Resources/background.png");
	//ヤシの木
	palm_1 = Sprite::Get()->SpriteCreate(L"Resources/palm.png");
	palm_2 = Sprite::Get()->SpriteCreate(L"Resources/palm.png");
	//鹿
	deer = Sprite::Get()->SpriteCreate(L"Resources/shika.png");

	player = new Player();
	// ステージ
	stage = Stage::GetInstance();
	stage->Init(player->GetSize());
	//プレイヤー
	player->Initialize();
	player->Init();

	//背景スプで動くやつの変数
	palmSize_1.pos = { 305,437 };
	palmSize_1.lim = { 0,40 };
	palmSize_1.flag = true;
	palmSize_1.speed = 2.0f;

	palmSize_2.pos = { 305,437 };
	palmSize_2.lim = { 0,40 };
	palmSize_2.flag = false;
	palmSize_2.speed = 2.0f;

	deerPos.pos = { 640,687 };
	deerPos.lim = { 0,40 };
	deerPos.flag = true;
	deerPos.speed = 1.0f;
}

void GameScene::Update()
{
	//シーンの変更の仕方
	if (player->GetGoalFlag() == true)
	{
		if (stageNumber < Stage::STAGE_COUNT)
		{
			// ステージ切り替え
			stageNumber++;
			stage->ChengeStage(stageNumber);
			player->Init();
		}
		else
		{
			BaseScene* scene = new ResultScene();
			sceneManager_->SetNextScene(scene);
		}
	}

	// ステージ出力（デバッグ実行用）
	if (Input::Get()->KeybordTrigger(DIK_1))
	{
		stage->WriteStage("write_test");
	}

	stage->GimmickUpdate();
	player->Update(stage);

	lightGroup->Update();

	SizeChange(palmSize_1);
	SizeChange(palmSize_2);
	SizeChange(deerPos);
}

void GameScene::SizeChange(Change change)
{
	Vec2 limit = change.initPos;
	// 軸単位で動かすかどうか
	const Vec2 isMove = {
		(change.lim.x ? 1.0f : 0.0f),
		(change.lim.y ? 1.0f : 0.0f)
	};

	if (change.flag)
	{
		limit += change.lim;
		change.pos += isMove * change.speed;

		if ((isMove.x && (change.pos.x >= limit.x)) || (isMove.y && (change.pos.y >= limit.y)))
		{
			change.flag = false;
		}
	}
	else
	{
		limit -= change.lim;
		change.pos -= isMove * change.speed;

		if ((isMove.x && (change.pos.x <= limit.x)) || (isMove.y && (change.pos.y <= limit.y)))
		{
			change.flag = true;
		}
	}
}

void GameScene::Draw()
{
	float width = 1280, height = 720;
	Sprite::Get()->Draw(background, { 0,0 }, width, height);
	
	Sprite::Get()->Draw(palm_1, { 0,720 }, palmSize_1.pos.x, palmSize_1.pos.y, {0.0f, 1.0f});
	Sprite::Get()->Draw(palm_2, { width,height }, palmSize_2.pos.x, palmSize_2.pos.x, { 0,1.0f }, {1,1,1,1}, true);
	Sprite::Get()->Draw(deer, deerPos.pos, 96, 160, {0.5f, 1.0f}, {1,1,1,1}, false);
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
