#pragma once
#include"BaseScene.h"
#include<Sprite.h>
#include<Stage.h>
class SelectScene :public BaseScene
{
private:
	enum Selection
	{
		LEFTSELECT = -4,
		RIGHTSELECT,
		BACKSELECT,
		RESETSELECT,
	};

	enum SelectionNumber
	{
		SELECTONE,
		SELECTTWO,
		SELECTTHREE,
	};
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
	//手と当たり判定
	void HandProcess();
private://定義

	SpriteData backGround[2];            //背景
	SpriteData stageData[20];            //ステージナンバー
	Vec2 stagePos[20];                   //座標
	SpriteData selectGraph[3] = {};			//右左
	Vec2 selectPos[1] = { {60.0f,60.0f}};

	float scale = 0.8f;                  //選択時の大きさ演出用
	int scaleNumber = -1;               //大きさを変える数字
	bool decScaleFlag = false;          //小さくするか大きくするか

	int selectNum = SELECTONE;
	int selectMax = 0;


	SpriteData hand_p = {};
	SpriteData hand_g = {};
	bool isDraw = false;

	Vec2 rulePos = { 670,410 };
	SpriteData rule = {};
	bool isChange = false;
};

