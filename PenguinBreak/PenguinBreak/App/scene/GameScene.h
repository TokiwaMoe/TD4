﻿#pragma once
#include "DirectXBase.h"
#include"DebugText.h"
#include"ParticleManager.h"
#include<memory>
#include"BaseScene.h"
#include<LightGroup.h>
#include"FBXObject3d.h"
#include"Player.h"
#include"../stage/Stage.h"
extern const int window_width;
extern const int window_height;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene :public BaseScene
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameScene();
	/// <summary>
	/// デスコンストラクタ
	/// </summary>
	~GameScene();
	/// <summary>
	/// 初期化
	/// </summary>
	void Init()override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;
	/// <summary>
	/// 影描画
	/// </summary>
	void ShadowDraw()override;

	void Finalize()override;
	bool GetEffect()override;

	static bool GetEditer() { return isEditer; }
	static void SetEditer() { isEditer = true; }

private://定義
	//2d
	SpriteData background;

	std::unique_ptr<Player>player;
	Stage* stage = nullptr;
	Audio* audio = Audio::Get();
	SoundData sound = {};
	int stageNumber = 1;
	int backNumber = 1;
	static bool isEditer;
};