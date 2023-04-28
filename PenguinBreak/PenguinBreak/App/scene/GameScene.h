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
private:
	std::unique_ptr<LightGroup>lightGroup;
private://定義

	//ゴール時の演出ハンドサイン
	std::unique_ptr<Model> m_model;		  //ペンギンモデル
	std::unique_ptr<FBXObject3d>  m_fbx;//ペンギンFBX

	//2d
	SpriteData background;
	SpriteData palm_1;
	SpriteData palm_2;

	Player* player = nullptr;
	Stage* stage = nullptr;
};