#pragma once
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
class GameScene:public BaseScene
{
private:
	//サイズや座標変更用
	struct Change
	{
		Vec2 initPos;
		Vec2 pos;
		Vec2 lim;
		bool flag;
		float speed;
	};
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
	/// 更新
	/// </summary>
	void SizeChange(Change change);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;
	/// <summary>
	/// 影描画
	/// </summary>
	void ShadowDraw()override;

	void Finalize()override;
private:
	std::unique_ptr<LightGroup>lightGroup;
private://定義
	//2d
	SpriteData background;
	SpriteData palm_1;
	SpriteData palm_2;
	SpriteData deer;

	Player* player = nullptr;
	Stage* stage = nullptr;
	int stageNumber = 1;

	//
	Vec2 size = { 305,437 };
	Change palmSize_1;
	Change palmSize_2;
	Change deerPos;
	bool isChange = true;

};