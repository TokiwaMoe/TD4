#pragma once
#include"BaseScene.h"
#include<Sprite.h>
#include<Stage.h>
class SelectScene :public BaseScene
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SelectScene();
	/// <summary>
	/// デスコンストラクタ
	/// </summary>
	~SelectScene();

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
	//ステージ決定
	void StageDecision();
	//選択時大きさが変わる処理
	void DecisionScale();
	//うごくスプライト
	void Move();
private://定義

	SpriteData backGround[2];            //背景
	SpriteData frame;            //背景
	SpriteData stageData[10];            //ステージナンバー
	Vec2 stagePos[10];                   //座標


	SpriteData backButton;               //戻りボタン
	Vec2 backButtonPos = { 60.0f,60.0f };//戻りボタンの位置
	float scale = 0.8f;                  //選択時の大きさ演出用
	int scaleNumber = -1;               //大きさを変える数字
	bool decScaleFlag = false;          //小さくするか大きくするか

	SpriteData hand_p = {};
	SpriteData hand_g = {};
	bool isDraw = false;

	Vec2 rulePos = { 670,410 };
	SpriteData rule = {};
	bool isChange = false;
};

