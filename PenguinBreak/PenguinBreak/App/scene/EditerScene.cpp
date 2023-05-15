#include "EditerScene.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"FbxLoader.h"
#include"Shape.h"
#include"SceneManager.h"
#include"TitleScene.h"

EditerScene::EditerScene()
{}
EditerScene::~EditerScene()
{
}

void EditerScene::Init()
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

	player = new Player();
	// ステージ
	stage = Stage::GetInstance();
	stage->EditerInit(player->GetSize());
	//プレイヤー
	player->Initialize();
	player->Init(stage);
}

void EditerScene::Update()
{
	//シーンの変更の仕方
	if (Input::Get()->KeybordTrigger(DIK_ESCAPE))
	{
		BaseScene* scene = new TitleScene();
		sceneManager_->SetNextScene(scene);
	}

	// ステージ出力
	if (Input::Get()->KeybordTrigger(DIK_S))
	{
		stage->WriteStage("write_stage");
	}

	int roadIndex = GetStageIndex2MousePos();
	if (Input::Get()->MousePushLeft() && roadIndex != -1)
	{
		stage->GetPos(roadIndex) = Input::Get()->GetMousePos();
	}

	lightGroup->Update();
}

void EditerScene::Draw()
{
	stage->Draw();
}

void EditerScene::ShadowDraw()
{
}

void EditerScene::Finalize()
{
}

bool EditerScene::GetEffect()
{
	return false;
}

int EditerScene::GetStageIndex2MousePos()
{
	int result = -1;

	for (int i = 0; i < stage->GetBoxSize(); i++)
	{
		const Vec2 leftUp = stage->GetPos(i) - (stage->GetSize(i) / 2.0f);
		const Vec2 rightDown = stage->GetPos(i) + (stage->GetSize(i) / 2.0f);

		if (leftUp.x < Input::Get()->GetMousePos().x &&
			rightDown.x > Input::Get()->GetMousePos().x &&
			leftUp.y < Input::Get()->GetMousePos().y &&
			rightDown.y > Input::Get()->GetMousePos().y)
		{
			result = i;
			break;
		}
	}

	return result;
}
