#pragma once
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

	void Init();
	/// <summary>
	/// ステージ変わるごとの初期化
	/// </summary>
	/// <param name="stageNo">ステージ番号</param>
	void stageInit(int stageNo);

	void Update(Stage *stage);
	/// <summary>
	/// マウス移動
	/// </summary>
	void Move();

	void collide2Stage(Stage *stage);

	int CollisionCount(Stage *stage);

	bool OutStage(Vec2 position, Stage *stage, int num);

	void Draw();

	//getter
	//座標
	const Vec2& GetPosition() { return position; }
	//回転
	const Vec2& GetRotation() { return rotation; }
	//setter

public:
	enum Direction
	{
		Left,
		Right
	};
	//2d
	SpriteData player;
	// X,Y,Z軸回りのローカル回転角
	Vec2 rotation = { 0,0 };
	// プレイヤー座標
	Vec2 position = { 50,50 };
	int  direction = 0;

	//スプライトサイズ
	float width = 64.0f, height = 128.0f;
	bool flipFlag = false;
	//半径 
	Vec2 radius = { 30, 62 };
	//マウスの判定用
	Sphere circle;
	Ray ray;
};