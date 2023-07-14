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
	for (auto& i : whiteBox)
	{
		i = Sprite::Get()->SpriteCreate(L"Resources/white1x1.png");
	}

	player = std::make_unique<Player>();
	// ステージ
	stage = Stage::GetInstance();
	stage->EditerInit(player->GetSize());

	// デバッグテキスト
	operationText.push_back("Drag:Move");
	operationText.push_back("C:Create");
	operationText.push_back("drag corner:Size Change");
	operationText.push_back("Arrow Up/Down:Change Scale");
	operationText.push_back("D:Delete");
	operationText.push_back("R:Reset");
	operationText.push_back("S:Save");
	operationText.push_back("P:Clear Check");
	operationText.push_back("Esc:Title");
}

void EditerScene::Update()
{
	if (isSave)
	{
		if (Input::Get()->KeybordTrigger(DIK_RIGHT) && saveFileNumber + 1 <= 32)
		{
			saveFileNumber++;
		}
		if (Input::Get()->KeybordTrigger(DIK_LEFT) && saveFileNumber - 1 >= 1)
		{
			saveFileNumber--;
		}
		if (Input::Get()->KeybordTrigger(DIK_RETURN))
		{
			stage->WriteStage("stage" + std::to_string(saveFileNumber));
			isSave = false;
		}
		if (Input::Get()->KeybordTrigger(DIK_BACK))
		{
			isSave = false;
		}
	}
	else
	{
		//シーンの変更の仕方
		if (Input::Get()->KeybordTrigger(DIK_ESCAPE))
		{
			BaseScene* scene = new TitleScene();
			sceneManager_->SetNextScene(scene);
		}
		else if (Input::Get()->KeybordTrigger(DIK_P))
		{
			GameScene::SetEditer();
			stage->SetIndex();
			BaseScene* scene = new GameScene();
			sceneManager_->SetNextScene(scene);
		}

		Vec2 mousePos = Input::Get()->GetMousePos();
		Vec2 leftTop;
		Vec2 rightBottom;

		if (roadIndex == -1)
		{
			cursorState = CursorState::NONE;
		}
		else
		{
			leftTop = stage->GetAnchorpointPos(roadIndex, Vec2(0.0f, 0.0f));
			rightBottom = stage->GetAnchorpointPos(roadIndex, Vec2(1.0f, 1.0f));

			if (Input::Get()->MousePushLeft() == false)
			{
				if (mousePos.x > leftTop.x &&
					mousePos.x < rightBottom.x &&
					mousePos.y > leftTop.y &&
					mousePos.y < rightBottom.y)
				{
					cursorState = CursorState::MOVE;
				}
				else if (mousePos.x > leftTop.x - frameWidth &&
						 mousePos.x < rightBottom.x + frameWidth &&
						 mousePos.y > leftTop.y - frameWidth &&
						 mousePos.y < rightBottom.y + frameWidth)
				{
					cursorState = CursorState::SCALE;
				}
				else
				{
					cursorState = CursorState::NONE;
				}
			}
		}

		if (cursorState == CursorState::NONE)
		{
			roadIndex = GetStageIndex2MousePos();

			if (roadIndex != -1)
			{
				stage->BringForefront(roadIndex);
			}
		}
		else if (Input::Get()->MousePushLeft())
		{
			if (cursorState == CursorState::MOVE)
			{
				stage->SetPos(roadIndex, Input::Get()->GetMousePos());
			}
			else if (cursorState == CursorState::SCALE)
			{
				ChangeSize(roadIndex);
			}
		}

		// 道の生成
		if (Input::Get()->KeybordTrigger(DIK_C))
		{
			stage->Create();
		}
		// 道の削除
		if (Input::Get()->KeybordTrigger(DIK_D))
		{
			stage->Delete(roadIndex);
			roadIndex = -1;
			cursorState = CursorState::NONE;
		}
		// ステージサイズ
		if (Input::Get()->KeybordTrigger(DIK_UP))
		{
			if (scale - 1 >= 1)
			{
				scale--;
				stage->SetScale(scale);
			}
		}
		else if (Input::Get()->KeybordTrigger(DIK_DOWN))
		{
			if (scale + 1 <= 4)
			{
				scale++;
				stage->SetScale(scale);
			}
		}
		// リセット
		if (Input::Get()->KeybordTrigger(DIK_R))
		{
			stage->EditerReset();
		}

		// ステージ出力
		if (Input::Get()->KeybordTrigger(DIK_S))
		{
			isSave = true;
		}
	}
}

void EditerScene::Draw()
{
	DebugText::Get()->Print(100.0f, 100.0f, 5, "Editer");

	if (isSave == false)
	{
		if (cursorState == CursorState::MOVE)
		{
			Sprite::Get()->Draw(frame,
								stage->GetPos(roadIndex),
								stage->GetSize(roadIndex).x + frameWidth,
								stage->GetSize(roadIndex).y + frameWidth,
								stage->GetAnchorpoint(roadIndex),
								{ 0.5f, 0.5f, 0.5f, 1.0f });
		}
		if (cursorState == CursorState::SCALE)
		{
			for (size_t i = 0; i < whiteBox.size(); i++)
			{
				Vec2 pos = stage->GetPos(roadIndex);
				size_t x = (i + (i >= (whiteBox.size() / 2))) % 3;
				size_t y = (i + (i >= (whiteBox.size() / 2))) / 3;
				if (x == 0)
				{
					pos.x -= (stage->GetSize(roadIndex) / 2.0f).x + frameWidth;
				}
				else if (x == 2)
				{
					pos.x += (stage->GetSize(roadIndex) / 2.0f).x + frameWidth;
				}
				if (y == 0)
				{
					pos.y -= (stage->GetSize(roadIndex) / 2.0f).y + frameWidth;
				}
				else if (y == 2)
				{
					pos.y += (stage->GetSize(roadIndex) / 2.0f).y + frameWidth;
				}

				Sprite::Get()->Draw(whiteBox[i],
									pos,
									frameWidth,
									frameWidth,
									stage->GetAnchorpoint(roadIndex),
									{ 1.0f, 1.0f, 1.0f, 1.0f });
			}
		}

		OperationDraw();
	}

	stage->Draw();
	DebugText::Get()->Print(16.0f, 16.0f, 2, "Scale:%d", scale);

	if (isSave)
	{
		std::string numText;
		if (saveFileNumber <= 1)
		{
			numText += ' ';
		}
		else
		{
			numText += '<';
		}
		numText += "%d";
		if (saveFileNumber < 32)
		{
			numText += '>';
		}

		// スプライトデータは適当な物
		Sprite::Get()->Draw(frame,
							Vec2(window_width / 2.0f, window_height / 2.0f - 22.0f),
							184.0f,
							146.0f,
							Vec2(0.5f, 0.5f),
							{ 0.0f, 0.0f, 0.0f, 1.0f });

		DebugText::Get()->Print(window_width / 2.0f - 82.0f, window_height / 2.0f - 80.0f, 2, "save stage");
		DebugText::Get()->Print(window_width / 2.0f - 64.0f, window_height / 2.0f, 7, numText, saveFileNumber);

		DebugText::Get()->Print(16.0f, window_height - 16.0f, 2, "Enter:Save");
		DebugText::Get()->Print(16.0f, window_height - 48.0f, 2, "Back Space:Cancel");
	}
}

void EditerScene::ShadowDraw()
{
}

void EditerScene::OperationDraw()
{
	for (int i = 0; i < operationText.size(); i++)
	{
		DebugText::Get()->Print(16.0f, window_height - 32.0f * (operationText.size() - i) + 16.0f, 2, operationText[i]);
	}
}

void EditerScene::Finalize()
{
}

bool EditerScene::GetEffect()
{
	return false;
}

void EditerScene::ChangeSize(size_t num)
{
	const int splitNum = 3;
	const Vec2 center = { 0.5f, 0.5f };
	const Vec2 centerPos = stage->GetAnchorpointPos(num, center);
	Vec2 distance;
	GetStageIndex2MousePos(&distance);
	Vec2 anchorpoint = { distance.x / stage->GetSize(num).x, distance.y / stage->GetSize(num).y };
	anchorpoint += center;
	anchorpoint = Vec2(Extremism(anchorpoint.x, splitNum), Extremism(anchorpoint.y, splitNum));

	Vec2 size = stage->GetSize(num);
	Vec2 oldSize = size;

	Vec2 dSign = Vec2(); //distanceの符号
	if (distance.x != 0.0f) dSign.x = distance.x / fabsf(distance.x);
	if (distance.y != 0.0f) dSign.y = distance.y / fabsf(distance.y);

	Vec2 radius = distance - (dSign * (frameWidth / 2.0f));
	if (anchorpoint.x != 0.5f)
	{
		size.x = fabsf(radius.x * 2.0f);
	}
	if (anchorpoint.y != 0.5f)
	{
		size.y = fabsf(radius.y * 2.0f);
	}

	Vec2 diff = size - oldSize;
	Vec2 pos = dSign;
	pos.x *= (diff / 2.0f).x;
	pos.y *= (diff / 2.0f).y;

	stage->SetSize(num, size);
	stage->SetPos(num, centerPos + pos);
}

int EditerScene::GetStageIndex2MousePos(Vec2* distance)
{
	int result = -1;

	for (int i = 0; i < stage->GetBoxSize(); i++)
	{
		const Vec2 leftTop = stage->GetAnchorpointPos(i, Vec2(0.0f, 0.0f)) - Vec2(frameWidth, frameWidth);
		const Vec2 rightBottom = stage->GetAnchorpointPos(i, Vec2(1.0f, 1.0f)) + Vec2(frameWidth, frameWidth);

		if (Input::Get()->GetMousePos().x > leftTop.x &&
			Input::Get()->GetMousePos().x < rightBottom.x &&
			Input::Get()->GetMousePos().y > leftTop.y &&
			Input::Get()->GetMousePos().y < rightBottom.y)
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

	if (x < 0.0f)
	{
		return 0.0f;
	}
	else if (x > 1.0f)
	{
		return 1.0f;
	}

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
