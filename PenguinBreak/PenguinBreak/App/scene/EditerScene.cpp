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
	frame = Sprite::Get()->SpriteCreate(L"Resources/white1x1.png");

	player = std::make_unique<Player>();
	// ステージ
	stage = Stage::GetInstance();
	stage->EditerInit(player->GetSize());

	// デバッグテキスト
	operationText.push_back("Drag:Move");
	operationText.push_back("C:Create");
	operationText.push_back("drag corner:Size Change");
	operationText.push_back("Left Click+D:Delete");
	operationText.push_back("R:Reset");
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

	Vec2 distance;
	roadIndex = GetStageIndex2MousePos(&distance);
	isClick = Input::Get()->MousePushLeft() && roadIndex != -1;
	DebugText::Get()->Print(12.0f, 24.0f, 1.5f, "x:%f y:%f", distance.x, distance.y);

	if (isClick)
	{
		if (fabsf(distance.x) < (stage->GetSize(roadIndex) / 2.0f).x &&
			fabsf(distance.y) < (stage->GetSize(roadIndex) / 2.0f).y)
		{
			stage->SetPos(roadIndex, Input::Get()->GetMousePos());
		}
		else
		{
			const Vec2 center = { 0.5f, 0.5f };

			Vec2 anchorpoint = { distance.x / stage->GetSize(roadIndex).x, distance.y / stage->GetSize(roadIndex).y };
			anchorpoint += center;
			anchorpoint = Vec2(Extremism(anchorpoint.x, 3), Extremism(anchorpoint.y, 3));
			stage->MoveAnchorpoint(roadIndex, anchorpoint);

			Vec2 size = stage->GetSize(roadIndex);
			if (anchorpoint.x != 0.5f)
			{
				size.x = (distance * 2.0f).x;
			}
			if (anchorpoint.y != 0.5f)
			{
				size.y = (distance * 2.0f).y;
			}
			stage->SetSize(roadIndex, size);

			//stage->MoveAnchorpoint(roadIndex, center);
		}
	}

	// 道の生成
	if (Input::Get()->KeybordTrigger(DIK_C))
	{
		stage->Create();
	}
	// 道の削除
	if (Input::Get()->KeybordTrigger(DIK_D) && isClick)
	{
		stage->Delete(roadIndex);
		isClick = false;
	}
	// リセット
	if (Input::Get()->KeybordTrigger(DIK_R))
	{
		stage->EditerInit(player->GetSize());
	}

	// ステージ出力
	if (Input::Get()->KeybordTrigger(DIK_S))
	{
		stage->WriteStage("write_stage");
	}
}

void EditerScene::Draw()
{
	DebugText::Get()->Print(100.0f, 100.0f, 5, "Editer");

	if (roadIndex != -1)
	{
		Sprite::Get()->Draw(frame,
							stage->GetPos(roadIndex),
							stage->GetSize(roadIndex).x + frameWidth,
							stage->GetSize(roadIndex).y + frameWidth,
							{ 0.5f,0.5f },
							{ 0.5f, 0.5f, 0.5f, 1.0f });
	}
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

int EditerScene::GetStageIndex2MousePos(Vec2* distance)
{
	int result = -1;

	for (int i = 0; i < stage->GetBoxSize(); i++)
	{
		const float left = stage->GetPos(i).x - stage->GetSize(i).x * stage->GetAnchorpoint(i).x - frameWidth;
		const float right = stage->GetPos(i).x + stage->GetSize(i).x * stage->GetAnchorpoint(i).x + frameWidth;
		const float up = stage->GetPos(i).y - stage->GetSize(i).y * stage->GetAnchorpoint(i).y - frameWidth;
		const float down = stage->GetPos(i).y + stage->GetSize(i).y * stage->GetAnchorpoint(i).y + frameWidth;

		if (Input::Get()->GetMousePos().x > left &&
			Input::Get()->GetMousePos().x < right &&
			Input::Get()->GetMousePos().y > up &&
			Input::Get()->GetMousePos().y < down)
		{
			result = i;

			if (distance)
			{
				*distance = Input::Get()->GetMousePos() - stage->GetPos(i);
			}

			break;
		}
	}

	return result;
}

float EditerScene::Extremism(float x, int split)
{
	if (split < 2) return -1.0f;
	float result = 0.0f;

	for (int i = 0; i < split; i++)
	{
		if (x < (static_cast<float>(i + 1) / static_cast<float>(split)))
		{
			if (i == 0)
			{
				result = 0.0f;
			}
			else
			{
				result = (static_cast<float>(i) / static_cast<float>(split - 1));
			}

			break;
		}
	}

	return result;
}
