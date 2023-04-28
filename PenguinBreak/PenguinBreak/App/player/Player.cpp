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
	//colCenter = Sprite::Get()->SpriteCreate(L"Resources/gutitubo.png");
}

void Player::Init()
{

}


void Player::Update()
{
	//移動
	Move();
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
		}
	}
	DebugText::Get()->Print(100.0f, 200.0f, 3, "Pos:%f,%f",position.x,position.y);
}

//void Player::collide2Stage(Stage stage) {
//	
//	// 四隅の座標のマップチップ番号
//	const int size = stage.GetInstance()->GetSize();
//
//	for (int i = 0; i < size; i++)
//	{
//
//	}
//
//	//当たったらスタート位置に戻る
//	
//}


void Player::Draw()
{
	//2D描画
	//Vec2 position2D = { 200.0f,200.0f };
	float width = 64.0f, height = 128.0f;
	Sprite::Get()->Draw(player, position, width, height, { 0.5f,0.5f });
}


