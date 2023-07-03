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
	player = Sprite::Get()->SpriteCreate(L"Resources/uma.png");
	hand_p = Sprite::Get()->SpriteCreate(L"Resources/hand_pa.png");
	hand_g = Sprite::Get()->SpriteCreate(L"Resources/hand_g.png");
	moveParticle = std::make_unique <ParticleManager>();
	moveParticle->Initialize();
	p_Texture = Texture::Get()->LoadTexture(L"Resources/paricle/particle3.png");
	for (int i = 0; i < DEATH_MAX; i++)
	{
		death[i] = Sprite::Get()->SpriteCreate(L"Resources/death.png");
	}
	deathSound = Audio::SoundLoadWave("Resources/Sound/death.wav");
}

void Player::Init(Stage* stage)
{
	position = {
	stage->GetInstance()->GetStartPos().x/* + (stage->GetInstance()->GetSize(stage->GOAL).x / 2.0f)*/,
	stage->GetInstance()->GetStartPos().y/* + (stage->GetInstance()->GetSize(stage->GOAL).y / 2.0f)*/
	};
	flipFlag = false;
	goalFlag = false;
	deathTime = 20;
	respawn = false;
	effect = false;
	deathCount = 0;
	for (int i = 0; i < DEATH_MAX; i++)
	{
		deathPos[i] = { 0,0 };
		isDeathDraw[i] = false;
	}
	size = Vec2(width, height) / static_cast<float>(stage->GetScale());
	radius = size / 2;
	SetSize(size);
	move = false;
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
	//スクリーン座標からワールド座標に変換する処理
	ConvertParticlePos();
	//移動
	Move();
	collide2Stage(stage);
	Input::Get()->SetCursor(false);
}

void Player::Move()
{
	if (Input::Get()->MousePushLeft() && !effect) {
		const Vec2 mouseSize = { 32,32 };
		if (Collision::BoxCollision(Input::Get()->GetMousePos(), position, mouseSize, radius) && !goalFlag) {
			position = Input::Get()->GetMousePos();
			
			//プレイヤーの画像によってはいらない処理
			if (static_cast<float>(Input::Get()->GetMouseMove().lX) > 0) {
				flipFlag = true;
			}
			else {
				flipFlag = false;
			}
		}
		//パーティクルだす
		//手のspを表示するか
		isDraw = true;
		moveParticle->ParticleAdd2(particlePos, { 1,1,0.5,1 }, { 1,1,0.5,1 });
	}
	else {
		isDraw = false;
		move = false;
	}
	if (move == true) {
		position = Input::Get()->GetMousePos();
	}
	/*DebugText::Get()->Print(100.0f, 100.0f, 3, "%f,%f", static_cast<float>(Input::Get()->GetMouseMove().lX), static_cast<float>(Input::Get()->GetMouseMove().lY));
	DebugText::Get()->Print(100.0f, 300.0f, 3, "%f,%f", position.x, position.y);*/

}


void Player::ConvertParticlePos()
{
	//ビューポート行列
	XMMATRIX mvp = XMMatrixIdentity();
	mvp.r[0].m128_f32[0] = 1280.0f / 2.0f;
	mvp.r[1].m128_f32[1] = -720.0f / 2.0f;
	mvp.r[3].m128_f32[0] = 1280.0f / 2.0f;
	mvp.r[3].m128_f32[1] = 720.0f / 2.0f;
	//ビュープロジェクションビューポート合成行列
	XMMATRIX mvpv = moveParticle->GetMat() * mvp;
	//上記の行列の逆行列
	XMMATRIX mvpvInv = XMMatrixInverse(nullptr, mvpv);
	//スクリーン座標
	Vec3 posNear = Vec3(position.x, position.y, 0);
	Vec3 posFar = Vec3(position.x, position.y, 1);
	XMVECTOR posNearV = XMLoadFloat3(&posNear);
	XMVECTOR posFarV = XMLoadFloat3(&posFar);
	//スクリーン座標系からワールド座標系へ
	posNearV = XMVector3TransformCoord(posNearV, mvpvInv);//座標に行列をかけてwを除算
	posFarV = XMVector3TransformCoord(posFarV, mvpvInv);
	//レイの方向
	XMVECTOR direction = posFarV - posNearV;
	//ベクトルの正規化
	direction = XMVector3Normalize(direction);
	const float distance = 0.0f;

	particlePos.x = posNearV.m128_f32[0] - direction.m128_f32[0] * distance;
	particlePos.y = posNearV.m128_f32[1] - direction.m128_f32[1] * distance;
	particlePos.z = posNearV.m128_f32[2] - direction.m128_f32[2] * distance;

	/*DebugText::Get()->Print(100.0f, 200.0f, 3, "%f,%f", particlePos.x, particlePos.z);
	DebugText::Get()->Print(100.0f, 300.0f, 3, "%f,%f", position.x, position.y);*/
	moveParticle->Update();
}



void Player::collide2Stage(Stage* stage)
{
	if (stage->GetRoadCount() == CollisionCount(stage))
	{
		//エフェクトだす
		effect = true;
	}
	else if (!effect)
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
		else if (deathTime >= 19) {
			audio->SoundSEPlayWave(deathSound);
		}
		if (respawn == true) {
			effect = false;
			//死のカウントをプラス
			deathCount++;
			//死んだ場所の位置コピー
			deathPos[deathCount - 1] = position;
			//死んだ場所に描画するスプライトのフラグをture
			isDeathDraw[deathCount - 1] = true;
			//ロード外に出たらスタート位置に戻す
			position = {
				stage->GetInstance()->GetStartPos().x,
				stage->GetInstance()->GetStartPos().y
			};

			if (deathCount >= DEATH_MAX)
			{
				deathCount = 0;
			}
		}
		else if (respawn == false) {
			effect = true;
		}
	}
	//ゴールの判定
	if (!OutStageX(position, stage, static_cast<int>(stage->GetGoal()))) {
		goalFlag = true;
		//DebugText::Get()->Print(100.0f, 300.0f, 4, "GOAL");
	}
	//DebugText::Get()->Print(100.0f, 200.0f, 3, "Pos:%d", deathTime);
	
}

int Player::CollisionCount(Stage* stage)
{
	int count = 0;
	Vec2 oldPos = position;
	for (int i = 0; i < stage->GetBoxSize(); i++)
	{
		if (stage->GetType(i) == Road::RoadType::WALL || effect)
		{
			if (!OutStageX(position, stage, i))
			{
				count = static_cast<int>(stage->GetRoadCount());
				break;
			}
		}
		else if (stage->GetType(i) == Road::RoadType::BACK)
		{
			continue;
		}
		else
		{
			if (OutStageX(position, stage, i))
			{
				count++;
			}
		}
	}
	//
	return count;
}

bool Player::OutStageX(Vec2 position, Stage* stage, int num)
{
	//ステージスプライトの中心座標
	Vec2 stageCenter = {
		stage->GetInstance()->GetPos(num).x,
		stage->GetInstance()->GetPos(num).y
	};
	//X軸、Y軸の距離を算出
	distance =
	{
		stageCenter.x - position.x,
		stageCenter.y - position.y
	};
	//絶対値にするため結果が負なら正にする
	if (distance.x < 0.0f) { distance.x *= -1.0f; }
	if (distance.y < 0.0f) { distance.y *= -1.0f; }
	//2つの矩形の和を算出
	const float outSize = 10.0f;
	size_num =
	{
		((stage->GetInstance()->GetSize(num).x) / 2.0f) - radius.x + outSize,
		((stage->GetInstance()->GetSize(num).y) / 2.0f) - radius.y + outSize
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

bool Player::OutStageY(float posY, Stage* stage, int num)
{
	return false;
}

void Player::Draw()
{
	//2D描画
	for (int i = 0; i < DEATH_MAX; i++)
	{
		if (isDeathDraw[i])
		{
			Sprite::Get()->Draw(death[i], deathPos[i], 32, 32, { 0.5f,0.5f });
		}
	}
	moveParticle->Draw(p_Texture);
	Sprite::Get()->Draw(player, position, width, height, { 0.5f,0.5f }, { 1,1,1,1 }, flipFlag);

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

void Player::SetSize(const Vec2& size)
{
	width = size.x;
	height = size.y;
	radius = (size / 2) - Vec2(4.0f, 4.0f);
}
