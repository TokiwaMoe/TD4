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
private://定義

	SpriteData backGround[2];					//背景

	SpriteData stageData[10];	//ステージナンバー
	Vec2 stagePos[10];	//座標

	SpriteData hand_p = {};
	SpriteData hand_g = {};
	bool isDraw = false;
};

