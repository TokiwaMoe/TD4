#include "Player.h"
#include <time.h>
#include <cassert>
#include "Input.h"

using namespace DirectX;

void Player::Initialize()
{
	//スプライト作成の仕方
	player = Sprite::Get()->SpriteCreate(L"Resources/Kari.png");
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
	//スピード
	const float speed = 3;
	//移動
	if (Input::Get()->KeybordPush(DIK_D)) {
		position.x += speed;
		direction = Right;
	}
	else if (Input::Get()->KeybordPush(DIK_A)) {
		position.x -= speed;
		direction = Left;
	}
	else if (Input::Get()->KeybordPush(DIK_W)) {
		position.y -= speed;
	}
	else if (Input::Get()->KeybordPush(DIK_S)) {
		position.y += speed;
	}
}

void Player::Draw()
{
	//2D描画
	//Vec2 position2D = { 200.0f,200.0f };
	float width = 64.0f, height = 128.0f;
	Sprite::Get()->Draw(player, position, width, height);
}


