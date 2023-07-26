#include "ResultStating.h"
#include <Collision.h>
#include <Stage.h>
#include<Input.h>

ResultStating::ResultStating()
{
}

ResultStating::~ResultStating()
{
}

void ResultStating::Init()
{
	backGround = Sprite::Get()->SpriteCreate(L"Resources/stage/night_back.png");
	selectGraph = Sprite::Get()->SpriteCreate(L"Resources/Result/back.png");
	nextStageGraph = Sprite::Get()->SpriteCreate(L"Resources/Result/next.png");
	hand_p = Sprite::Get()->SpriteCreate(L"Resources/hand_pa.png");
	hand_g = Sprite::Get()->SpriteCreate(L"Resources/hand_g.png");

	clearGraph[0] = Sprite::Get()->SpriteCreate(L"Resources/Result/ku.png");
	clearGraph[1] = Sprite::Get()->SpriteCreate(L"Resources/Result/ri.png");
	clearGraph[2] = Sprite::Get()->SpriteCreate(L"Resources/Result/a.png");
	clearGraph[3] = Sprite::Get()->SpriteCreate(L"Resources/Result/-.png");

	for (int i = 0; i < HAND_MAX_Y; i++)
	{
		for (int j = 0; j < HAND_MAX_X; j++)
		{
			hand[j][i] = Sprite::Get()->SpriteCreate(L"Resources/Result/clear.png");
		}
	}

	frame = Sprite::Get()->SpriteCreate(L"Resources/Result/frame.png");

	houseHumanGraph = Sprite::Get()->SpriteCreate(L"Resources/uma.png");
	houseHumanGraph.rotation = 90.0f;

	for (size_t i = 0; i < 4; i++)
	{
		clearMovePos[i] = i * 5.0f;
		clearMoveFlag[i] = {};
	}
}

void ResultStating::Update(const int nextStage)
{
	//�X�e�[�W����
	StageDecision(nextStage);
	//�I�����傫�����ς�鏈��
	DecisionScale();
	//�N���A����������
	ClearMove();
	//�n�l�Ԃ̓���
	DoubleHMove();
	//��̉�]
	HandRotation();
}

void ResultStating::Draw(int nextStage)
{
	//�w�i
	const float width = 1280, height = 720;
	Sprite::Get()->Draw(backGround, {}, width, height);

	//��
	const Vec2 handSize = { 320, 285 };
	Vec2 handPos;
	
	for (int i = 0; i < HAND_MAX_Y; i++)
	{
		for (int j = 0; j < HAND_MAX_X; j++)
		{
			Sprite::Get()->Draw(hand[j][i], { handSize.x / 2 + j * handSize.x, handSize.y / 2 + (height - handSize.y) * i }, handSize.x, handSize.y, { 0.5f,0.5f });
		}
	}

	//�z
	Sprite::Get()->Draw(frame, {}, width, height, {0.0f,0.0f});


	//�N���A����
	const Vec2 clearSize = { 93.0f,155.0f };
	const Vec2 clearBasicPos = { 350.0f,184.0f };
	float clearMove = 160.0f;
	for (int i = 0; i < 4; i++)
	{
		Sprite::Get()->Draw(clearGraph[i], Vec2(clearBasicPos.x + clearMove * i, clearBasicPos.y + clearMovePos[i]),
			clearSize.x, clearSize.y);
	}

	//�n�l��
	const float houseWidth = 64.0f, houseHeight = 128.0f;
	Sprite::Get()->Draw(houseHumanGraph, houseHumanPos, houseWidth, houseHeight);

	//���ǂ�
	if (scaleNumber == BACK)
	{
		Sprite::Get()->Draw(selectGraph, selectPos, selectSize.x * scale, selectSize.y * scale, { 0.5f,0.5f });
	}
	else
	{
		Sprite::Get()->Draw(selectGraph, selectPos, selectSize.x, selectSize.y, { 0.5f,0.5f });
	}
	//����
	if (nextStage < Stage::STAGE_COUNT + 1)
	{
		if (scaleNumber == NEXT)
		{
			Sprite::Get()->Draw(nextStageGraph, nextStagePos, selectSize.x * scale, selectSize.y * scale, { 0.5f,0.5f });
		}
		else
		{
			Sprite::Get()->Draw(nextStageGraph, nextStagePos, selectSize.x, selectSize.y, { 0.5f,0.5f });
		}
	}
	//��
	Vec2 hPos = { Input::Get()->GetMousePos().x,Input::Get()->GetMousePos().y };
	if (isDraw)
	{
		Sprite::Get()->Draw(hand_g, hPos, 32, 32, { 0.5f,0.5f });
	}
	else
	{
		Sprite::Get()->Draw(hand_p, hPos, 32, 32, { 0.5f,0.5f });
	}
}

void ResultStating::StageDecision(const int nextStage)
{
	//�X�e�[�W�̐���
	const Vec2 mSize = { 1.0f,1.0f };
	//�}�E�X�ŃX�e�[�W����
	if (Input::Get()->MousePushLeft()) {

		scaleNumber = -1;//���Z�b�g����
		if (Collision::BoxCollision(selectPos, Input::Get()->GetMousePos(), selectSize / 2, mSize)) {
			scaleNumber = BACK;
		}
		//�^�C�g���߂�{�^��
		if (Collision::BoxCollision(nextStagePos, Input::Get()->GetMousePos(), selectSize / 2, mSize)) {
			scaleNumber = NEXT;
		}
		//���sp��\�����邩
		isDraw = true;
	}
	else
	{
		scaleNumber = -1;//���Z�b�g����
		if (Collision::BoxCollision(selectPos, Input::Get()->GetMousePos(), selectSize / 2, mSize)) {
			scaleNumber = BACK;
		}
		//�^�C�g���߂�{�^��
		if (Collision::BoxCollision(nextStagePos, Input::Get()->GetMousePos(), selectSize / 2, mSize)) {
			scaleNumber = NEXT;
		}
		isDraw = false;
	}
	//�}�E�X�ŃX�e�[�W����
	if (Input::Get()->MouseReleaseLeft())
	{
		//�X�e�[�W�Z���N�g��
		if (Collision::BoxCollision(selectPos, Input::Get()->GetMousePos(), selectSize / 2, mSize)) {
			nextSceneStatus = BACK;
		}
		//���̃X�e�[�W��
		if (nextStage < Stage::STAGE_COUNT + 1)
		{
			if (Collision::BoxCollision(nextStagePos, Input::Get()->GetMousePos(), selectSize / 2, mSize)) {
				nextSceneStatus = NEXT;
			}
		}
	}
}

void ResultStating::DecisionScale()
{
	const float scaleMax = 1.0f;
	const float scaleMin = 0.8f;
	const float speed = 0.01f;
	if (decScaleFlag == true)
	{
		scale += speed;
		if (scale >= scaleMax)
		{
			decScaleFlag = false;
		}
	}
	else
	{
		scale -= speed;
		if (scale <= scaleMin)
		{
			decScaleFlag = true;
		}
	}
}

void ResultStating::ClearMove()
{
	const float moveMax = 10.0f, moveMin = -10.0f, speed = 0.3f;
	for (size_t i = 0; i < 4; i++)
	{
		if (clearMoveFlag[i] == true)
		{
			clearMovePos[i] += speed;
			if (clearMovePos[i] > moveMax)
			{
				clearMoveFlag[i] = false;

			}
		}
		else
		{
			clearMovePos[i] -= speed;
			if (clearMovePos[i] < moveMin)
			{
				clearMoveFlag[i] = true;

			}
		}
	}
}

void ResultStating::DoubleHMove()
{
	const float widthMax = 1440.0f, widthMin = 150.0f, speed = 3.0f;

	if (houseHumanPos.x > widthMax)
	{
		houseHumanPos.x = -widthMin;
	}
	houseHumanPos.x += speed;





}

void ResultStating::HandRotation()
{
	for (int i = 0; i < HAND_MAX_Y; i++)
	{
		for (int j = 0; j < HAND_MAX_X; j++)
		{
			if (j < 2)
			{
				hand[j][i].rotation -= 5;
			}
			else
			{
				hand[j][i].rotation += 5;
			}
		}
	}
}
