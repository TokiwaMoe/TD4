#pragma once
#include "DirectXBase.h"
#include"DebugText.h"
#include"Collision.h"
#include"Audio.h"
#include<PostEffect.h>
#include<memory>
#include"BaseScene.h"
extern const int window_width;
extern const int window_height;
/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene :public BaseScene
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleScene();
	/// <summary>
	/// デスコンストラクタ
	/// </summary>
	~TitleScene();

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
private:
	//決定
	void StageDecision();
	//選択時大きさが変わる処理
	void DecisionScale();
private://定義
	SpriteData background = {};//背景
	SpriteData titleGraph = {};
	SpriteData button = {};
	SpriteData explanation = {};
	const Vec2 bPos = { 630.0f, 580.0f };
	const Vec2 bSize = { 400.0f, 160.0f };
	float scale = 1.0f;
	bool isScale = false;
	bool decScaleFlag = false;          //小さくするか大きくするか

	bool isDraw = false;
	SpriteData hand_p = {};
	SpriteData hand_g = {};
	Vec2 hPos = {};
};


