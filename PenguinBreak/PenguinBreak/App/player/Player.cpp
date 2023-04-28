#include "Player.h"
#include <time.h>
#include <cassert>
#include "Input.h"
#include"Debugtext.h"
using namespace DirectX;

void Player::Initialize()
{
	//スプライト作成の仕方
	player = Sprite::Get()->SpriteCreate(L"Resources/Kari.png");
}

void Player::Init()
{
	position = { 50,50 };
	flipFlag = false;
}

void Player::stageInit(int stageNo)
{
	//初期ポジション
	//if(stageNo==){
	//position={};
	//}
}


void Player::Update(Stage *stage)
{
	//移動
	Move();
	collide2Stage(stage);
}

void Player::Move()
{
	circle.center = { position.x, position.y, 0 };
	circle.radius = 128;
	ray.start = { Input::Get()->GetMousePos().x,Input::Get()->GetMousePos().y,0 };
	ray.dir = { 1,0,0,0 };
	if (Input::Get()->MousePushLeft()) {
		if (Collision::CheckRay2Sphere(ray, circle)) {
			position = Input::Get()->GetMousePos();
			//プレイヤーの画像によってはいらない処理
			if ((float)Input::Get()->GetMouseMove().lX > 0) {
				flipFlag = true;
			}
			else if ((float)Input::Get()->GetMouseMove().lX < 0) {
				flipFlag = false;
			}
		}
	}


	//DebugText::Get()->Print(100.0f, 200.0f, 3, "Pos:%f", (float)Input::Get()->GetMouseMove().lX);
	DebugText::Get()->Print(100.0f, 200.0f, 3, "%d", flipFlag);
}

void Player::collide2Stage(Stage *stage) 
{

	//ロード外に出たらスタート位置に戻す
	for (int i = 0; i < 3; i++)
	{
		if (OutStage(position, stage, i))
		{
			DebugText::Get()->Print(100.0f, 500.0f, 2, "out stage");
			position = { stage->GetInstance()->GetStartPos().x - radius.x + 5.0f, stage->GetInstance()->GetStartPos().y + radius.y + 5.0f};
		}
	}
	
}

bool Player::OutStage(Vec2 position, Stage *stage, int num)
{
	float boxTop = stage->GetInstance()->GetBox(num).pos.y;
	float boxBottom = stage->GetInstance()->GetBox(num).pos.y + stage->GetInstance()->GetBox(num).size.y;

	float boxRight = stage->GetInstance()->GetBox(num).pos.x + stage->GetInstance()->GetSize(num).x;
	float boxLeft = stage->GetInstance()->GetBox(num).pos.x;

	//プレイヤーの上部分がステージの上部分より上だったら
	if (boxTop < position.y - radius.y && boxBottom > position.y + radius.y)
	{
		return false;
	}
	/*if (boxLeft < position.x - radius.x && boxRight < position.x + radius.x)
	{
		return false;
	}*/
	else{
		return true;
	}
}


void Player::Draw()
{
	//2D描画
	//Vec2 position2D = { 200.0f,200.0f };
	float width = 64.0f, height = 128.0f;
	Sprite::Get()->Draw(player, position, width, height, { 0.5f,0.5f }, { 1,1,1,1 }, flipFlag);
}


