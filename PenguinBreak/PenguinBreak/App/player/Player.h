﻿#pragma once
#include <DirectXMath.h>
#include "DirectXBase.h"
#include"DebugText.h"
#include"Collision.h"
#include"Audio.h"
#include"FBXObject3d.h"
#include"Object.h"
#include"Sprite.h"
#include"Stage.h"
#include<memory>
#include"ParticleManager.h"
#include"Easing.h"


class Player {
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	void Initialize();

	void Init(Stage* stage);
	/// <summary>
	/// ステージ変わるごとの初期化
	/// </summary>
	/// <param name="stageNo">ステージ番号</param>
	void stageInit(int stageNo);

	void Update(Stage* stage);
	/// <summary>
	/// マウス移動
	/// </summary>

	void collide2Stage(Stage* stage);

	int CollisionCount(Stage* stage);

	bool OutStage(Vec2 position, Stage* stage, int num);
	void Draw();

	//getter
	//座標
	const Vec2& GetPosition() { return position; }
	//スプライトサイズ
	const Vec2 GetSize() const { return Vec2(width, height); }
	//回転
	const Vec2& GetRotation() { return rotation; }
	bool GetGoalFlag() { return goalFlag; }
	bool GetEffect() { return effect; }
	//死
	int GetDeathCount() { return deathCount; }
	//setter
	//スプライトサイズ
	void SetSize(const Vec2& size);

private:
	static const int DEATH_MAX = 30;

private:
	Audio* audio = Audio::Get();
	SoundData deathSound{};//死んだときの
	void Move();
	/// <summary>
	/// playerの座標を3dに変換
	/// </summary>
	void ConvertParticlePos();
	enum Direction
	{
		Left,
		Right
	};
	//2d
	SpriteData player = {};
	SpriteData hand_p = {};
	SpriteData hand_g = {};
	SpriteData death[DEATH_MAX] = {};
	// X,Y,Z軸回りのローカル回転角
	Vec2 rotation = { 0,0 };
	// プレイヤー座標
	Vec2 position = { 50,50 };
	int  direction = 0;
	Vec3 particlePos = { 0,0,0 };

	//スプライトサイズ
	Vec2 size = {};
	float width = 64.0f, height = 128.0f;
	bool flipFlag = false;
	//半径 
	Vec2 radius = { 28, 60 };
	//マウスの判定用
	Sphere circle;
	Ray ray;
	bool goalFlag = false;//ゴールに行ったか否か
	bool respawn = false;
	bool effect = false;
	int deathTime = 20;//死んだときの演出時間
	std::unique_ptr <ParticleManager> moveParticle;
	TextureData p_Texture = {};
	//描画
	bool isDraw = true;
	//死
	bool isDeathDraw[DEATH_MAX];
	int deathCount = 0;
	Vec2 deathPos[DEATH_MAX];

	bool hit = false;
	bool move = false;

	//タイマー
	float time = 0;
	Easing* easing = nullptr;

};