#include "Player.h"
#include <time.h>
#include <cassert>
#include "Input.h"
#include"Debugtext.h"
#include"Texture.h"
using namespace DirectX;

void Player::Initialize()
{
	//スプライト作成の仕方
	player = Sprite::Get()->SpriteCreate(L"Resources/Kari.png");
	hand_p = Sprite::Get()->SpriteCreate(L"Resources/hand_pa.png");
	hand_g = Sprite::Get()->SpriteCreate(L"Resources/hand_g.png");
	moveParticle = std::make_unique <ParticleManager>();
	moveParticle->Initialize();
	p_Texture = Texture::Get()->LoadTexture(L"Resources/Paricle/particle.jpg");
}

void Player::Init(Stage* stage)
{
	//position = { 50,50 };
	position = {
	stage->GetInstance()->GetStartPos().x/* + (stage->GetInstance()->GetSize(stage->GOAL).x / 2.0f)*/,
	stage->GetInstance()->GetStartPos().y/* + (stage->GetInstance()->GetSize(stage->GOAL).y / 2.0f)*/
	};
	flipFlag = false;
	goalFlag = false;
	deathTime = 20;
	respawn = false;
	effect = false;
}

void Player::stageInit(int stageNo)
{
	//初期ポジション
	//if(stageNo==){
	//position={};
	//}
}

void Player::Update(Stage* stage)
{
	ConvertParticlePos();
	//移動
	Move();
	collide2Stage(stage);
	Input::Get()->SetCursor(false);
}

void Player::Move()
{
	//円とレイの判定を使っている
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
		//パーティクルだす
		//手のspを表示するか
		isDraw = true;
	}
	else
	{
		isDraw = false;
	}
	moveParticle->ParticleAdd2(
		{0,0,0}, { 1,1,1,1 }, { 1,1,1,1 });
	moveParticle->Update();

#if _DEBUG 
	DebugText::Get()->Print(100.0f, 200.0f, 3, "%d", flipFlag);
#endif
}

void Player::ConvertParticlePos()
{
	//ワールド座標取得
	//particlePos=
	//ビューポート

	//プロジェクション

	//座標にプロジェクションを掛ける	
}

void Player::collide2Stage(Stage* stage)
{
	if (stage->GetInstance()->GetBoxSize() == CollisionCount(stage))
	{
		//エフェクトだす
		effect = true;
	}
	else if(!effect)
	{
		respawn = false;
	}
	//死亡時演出後の処理
	if (effect)
	{
		deathTime--;
		//一定時間に達したらリスポーンする
		if (deathTime <= 0) {
			deathTime = 20;
			respawn = true;
		}
		if (respawn == true) {
			effect = false;
			//ロード外に出たらスタート位置に戻す
			position = {
				stage->GetInstance()->GetStartPos().x,
				stage->GetInstance()->GetStartPos().y
			};
		}
		else if (respawn == false) {
			effect = true;
		}
	}
	//ゴールの判定
	if (!OutStage(position, stage, static_cast<int>(stage->GetGoal()))) {
		goalFlag = true;
		DebugText::Get()->Print(100.0f, 300.0f, 4, "GOAL");
	}
	DebugText::Get()->Print(100.0f, 200.0f, 3, "Pos:%d", deathTime);
}

int Player::CollisionCount(Stage* stage)
{
	int count = 0;
	for (int i = 0; i < stage->GetInstance()->GetBoxSize(); i++)
	{
		if (OutStage(position, stage, i))
		{
			count++;
		}
	}

	return count;
}

bool Player::OutStage(Vec2 position, Stage* stage, int num)
{
	//ステージスプライトの中心座標
	Vec2 stageCenter = {
		stage->GetInstance()->GetPos(num).x,
		stage->GetInstance()->GetPos(num).y
	};
	//X軸、Y軸の距離を算出
	Vec2 distance =
	{
		stageCenter.x - position.x,
		stageCenter.y - position.y
	};
	//絶対値にするため結果が負なら正にする
	if (distance.x < 0.0f) { distance.x *= -1.0f; }
	if (distance.y < 0.0f) { distance.y *= -1.0f; }
	//2つの矩形の和を算出
	Vec2 size_num =
	{
		((stage->GetInstance()->GetSize(num).x / 2.0f) - radius.x),
		((stage->GetInstance()->GetSize(num).y / 2.0f) - radius.y)
	};
	//距離がサイズの和より小さいor以下
	if (distance.x <= size_num.x && distance.y <= size_num.y)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Player::Draw()
{
	//2D描画
	//Vec2 position2D = { 200.0f,200.0f };
	float width = 64.0f, height = 128.0f;
	Sprite::Get()->Draw(player, position, width, height, { 0.5f,0.5f }, { 1,1,1,1 }, flipFlag);
	//moveParticle->Draw(p_Texture);

	if (isDraw)
	{
		Vec2 hPos = { Input::Get()->GetMousePos().x,Input::Get()->GetMousePos().y };
		Sprite::Get()->Draw(hand_g, hPos, 32, 32, { 0.5f,0.5f });
	}
	else
	{
		Vec2 hPos = { Input::Get()->GetMousePos().x,Input::Get()->GetMousePos().y };
		Sprite::Get()->Draw(hand_p, hPos, 32, 32, { 0.5f,0.5f });
	}
}
