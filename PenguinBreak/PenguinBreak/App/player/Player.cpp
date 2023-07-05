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
	stage->GetInstance()->GetStartPos().x,
	stage->GetInstance()->GetStartPos().y
	};
	oldPos = position;
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
	}
	//DebugText::Get()->Print(100.0f, 100.0f, 3, "%f,%f", static_cast<float>(Input::Get()->GetMouseMove().lX), static_cast<float>(Input::Get()->GetMouseMove().lY));
	DebugText::Get()->Print(100.0f, 300.0f, 3, "%f,%f", oldPos.x, oldPos.y);
	DebugText::Get()->Print(100.0f, 400.0f, 3, "%f,%f", position.x, position.y);
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
	if (/*stage->GetRoadCount() == CollisionCount(stage) || */LineCollisionCount(stage) == stage->GetRoadCount())
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
				stage->GetPos(stage->GetRestart()).x,
				stage->GetPos(stage->GetRestart()).y
			};
			oldPos = position;

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
	if (!OutStage(position, stage, static_cast<int>(stage->GetGoal()))) {
		goalFlag = true;
		//DebugText::Get()->Print(100.0f, 300.0f, 4, "GOAL");
	}
	//DebugText::Get()->Print(100.0f, 200.0f, 3, "Pos:%d", deathTime);
	
}

int Player::CollisionCount(Stage* stage)
{
	int count = 0;
	for (int i = 0; i < stage->GetBoxSize(); i++)
	{
		if (stage->GetType(i) == Road::RoadType::WALL)
		{
			if (!OutStage(position, stage, i))
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
			if (OutStage(position, stage, i))
			{
				count++;
			}
			else
			{
				if (stage->GetType(i) == Road::RoadType::START || stage->GetType(i) == Road::RoadType::SAVE)
				{
					stage->ChangeRestart(i);
				}
			}
		}
	}
	//
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
	distance =
	{
		stageCenter.x - position.x,
		stageCenter.y - position.y
	};
	//絶対値にするため結果が負なら正にする
	if (distance.x < 0.0f) { distance.x *= -1.0f; }
	if (distance.y < 0.0f) { distance.y *= -1.0f; }
	//2つの矩形の和を算出
	const float outSize = 12.0f;
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

bool Player::Line2Line(Vec2 stagePoint1, Vec2 stagePoint2, Vec2 oldPos, Vec2 position)
{
	//プレイヤーの線分
	Vec2 base2player = { oldPos.x - position.x, oldPos.y - position.y };
	//ステージの点1と前座標の線分
	Vec2 p12oldPos = { stagePoint1.x - oldPos.x, stagePoint1.y - oldPos.y };
	//ステージの点2と前座標の線分
	Vec2 p22oldPos = { stagePoint2.x - oldPos.x, stagePoint2.y - oldPos.y };

	float a = base2player.x * p12oldPos.y - base2player.y * p12oldPos.x;
	float b = base2player.x * p22oldPos.y - base2player.y * p22oldPos.x;
	float len1 = a * b;
	if (len1 > 0)
	{
		return false;
	}

	//ステージ線分
	Vec2 base2stage = { stagePoint2.x - stagePoint1.x, stagePoint2.y - stagePoint1.y };
	//ステージの点1と前座標の線分
	Vec2 oldPos2p1 = { oldPos.x - stagePoint1.x, oldPos.y - stagePoint1.y };
	//ステージの点2と現在座標の線分
	Vec2 pos2p2 = { position.x - stagePoint1.x, position.y - stagePoint1.y };

	float c = base2stage.x * oldPos2p1.y - base2stage.y * oldPos2p1.x;
	float d = base2stage.x * pos2p2.y - base2stage.y * pos2p2.x;
	float len2 = c * d;
	if (len2 > 0)
	{
		return false;
	}
	return true;
}

bool Player::Rect2Line(Vec2 leftTop, Vec2 rightBottom, Vec2 oldPos, Vec2 position)
{
	//上
	if (Line2Line(leftTop, { rightBottom.x, leftTop.y }, oldPos, position)) { return true; }
	//下
	if (Line2Line({ leftTop.x, rightBottom.y }, rightBottom, oldPos, position)) { return true; }
	//左
	if (Line2Line(leftTop, { leftTop.x, rightBottom.y }, oldPos, position)) { return true; }
	//右
	if (Line2Line({ rightBottom.x, leftTop.y }, rightBottom, oldPos, position)) { return true; }
	return false;
}

bool Player::Old2Now(Vec2 leftTop, Vec2 rightBottom, Vec2 oldPos, Vec2 position)
{
	float x = oldPos.x - position.x;
	float y = oldPos.y - position.y;
	//正に直す
	if (x < 0) { x *= -1; }
	if (y < 0) { y *= -1; }
	//カウント
	int count = 0;
	//差が大きい方でfor分を回す
	if (x > y)
	{
		//現在の座標より前座標の方が大きかったら現在座標からfor分を回す
		if (oldPos.x > position.x)
		{
			for (float i = position.x; i <= oldPos.x; i++)
			{
				//現在の座標より前座標の方が大きかったら現在座標からfor分を回す
				if (oldPos.y > position.y)
				{
					for (float j = position.y; j <= oldPos.y; j++)
					{
						if (Rect2Line(leftTop, rightBottom, oldPos, {i, j}))
						{
							count++;
						}
					}
				}
				else
				{
					for (float j = oldPos.y; j <= position.y; j++)
					{
						if (Rect2Line(leftTop, rightBottom, oldPos, { i, j }))
						{
							count++;
						}
					}
				}
			}
		}
		else
		{
			for (float i = oldPos.x; i <= position.x; i++)
			{
				//現在の座標より前座標の方が大きかったら現在座標からfor分を回す
				if (oldPos.y > position.y)
				{
					for (float j = position.y; j <= oldPos.y; j++)
					{
						if (Rect2Line(leftTop, rightBottom, oldPos, { i, j }))
						{
							count++;
						}
					}
				}
				else
				{
					for (float j = oldPos.y; j <= position.y; j++)
					{
						if (Rect2Line(leftTop, rightBottom, oldPos, { i, j }))
						{
							count++;
						}
					}
				}
			}
		}
	}
	else
	{
		//現在の座標より前座標の方が大きかったら現在座標からfor分を回す
		if (oldPos.y > position.y)
		{
			for (float i = position.y; i <= oldPos.y; i++)
			{//現在の座標より前座標の方が大きかったら現在座標からfor分を回す
				if (oldPos.x > position.x)
				{
					for (float j = position.x; j <= oldPos.x; j++)
					{
						if (Rect2Line(leftTop, rightBottom, oldPos, { j, i }))
						{
							count++;
						}
					}
				}
				else
				{
					for (float j = oldPos.x; j <= position.x; j++)
					{
						if (Rect2Line(leftTop, rightBottom, oldPos, { j, i }))
						{
							count++;
						}
					}
				}
			}
		}
		else
		{
			for (float i = position.y; i <= oldPos.y; i++)
			{
				//現在の座標より前座標の方が大きかったら現在座標からfor分を回す
				if (oldPos.x > position.x)
				{
					for (float j = position.x; j <= oldPos.x; j++)
					{
						if (Rect2Line(leftTop, rightBottom, oldPos, { j, i }))
						{
							count++;
						}
					}
				}
				else
				{
					for (float j = oldPos.x; j <= position.x; j++)
					{
						if (Rect2Line(leftTop, rightBottom, oldPos, { j, i }))
						{
							count++;
						}
					}
				}
			}
		}
	}

	if (count != 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Player::LineCollisionCount(Stage* stage)
{
	int count = 0;
	
	for (int i = 0; i < stage->GetBoxSize(); i++)
	{
		//ステージ左上
		Vec2 leftTop = { stage->GetPos(i).x - stage->GetSize(i).x, stage->GetPos(i).y - stage->GetSize(i).y };
		//ステージ右下
		Vec2 rightBottom = { stage->GetPos(i).x + stage->GetSize(i).x, stage->GetPos(i).y + stage->GetSize(i).y };

		if (stage->GetType(i) == Road::RoadType::WALL)
		{
			if (!Old2Now(leftTop, rightBottom, oldPos, position))
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
			if (Old2Now(leftTop, rightBottom, oldPos, position))
			{
				count++;
			}
			
		}
	}
	//
	return count;
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
