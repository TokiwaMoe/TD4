#include "EditerScene.h"
#include<sstream>
#include "Input.h"
#include"Shape.h"
#include"SceneManager.h"
#include"TitleScene.h"
#include"GameScene.h"

EditerScene::EditerScene()
{}
EditerScene::~EditerScene()
{
}

void EditerScene::Init()
{
	cursor = Sprite::Get()->SpriteCreate(L"Resources/hand_pa.png");

	player = std::make_unique<Player>();
	// ステージ
	stage = Stage::GetInstance();
	stage->EditerInit(player->GetSize());

	//デバッグテキスト
	operationText.push_back("Mouse:Move");
	operationText.push_back("C:Create");
	operationText.push_back("S:Save");
	operationText.push_back("P:Clear Check");
	operationText.push_back("Esc:Title");
}

void EditerScene::Update()
{
	//シーンの変更の仕方
	if (Input::Get()->KeybordTrigger(DIK_ESCAPE))
	{
		BaseScene* scene = new TitleScene();
		sceneManager_->SetNextScene(scene);
	}
	else if(Input::Get()->KeybordTrigger(DIK_P))
	{
		GameScene::SetEditer();
		stage->SetIndex();
		BaseScene* scene = new GameScene();
		sceneManager_->SetNextScene(scene);
	}

	// 道の生成
	if (Input::Get()->KeybordTrigger(DIK_C))
	{
		stage->Create();
	}

	// ステージ出力
	if (Input::Get()->KeybordTrigger(DIK_S))
	{
		stage->WriteStage("write_stage");
	}

	static int roadIndex = -1;
	if (Input::Get()->MouseTriggerLeft())
	{
		roadIndex = GetStageIndex2MousePos();
	}
	else if (Input::Get()->KeybordPush(DIK_1)) //マウスを離した瞬間の入力にしたい
	{
		roadIndex = -1;
	}

	if (roadIndex != -1)
	{
		stage->SetPos(Input::Get()->GetMousePos(), roadIndex);
	}
}

void EditerScene::Draw()
{
	DebugText::Get()->Print(100.0f, 100.0f, 5, "Editer");

	stage->Draw();
	Sprite::Get()->Draw(cursor, Input::Get()->GetMousePos(), 32, 32, { 0.5f,0.5f });
	OperationDraw();
}

void EditerScene::ShadowDraw()
{
}

void EditerScene::OperationDraw()
{
	for (int i = 0; i < operationText.size(); i++)
	{
		DebugText::Get()->Print(16.0f, window_height - 32.0f * (operationText.size() - i) + 16.0f, 2, operationText[i].c_str());
	}
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
