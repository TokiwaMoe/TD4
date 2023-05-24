#pragma once
#include "ShadowMap.h"
#include"BaseScene.h"
#include"Audio.h"
#include<Sprite.h>
extern const int window_width;
extern const int window_height;

class SceneManager :public Singleton<SceneManager>
{
private:
	friend Singleton<SceneManager>;

	enum BlockOutType
	{
		Stand_By,
		First_Half,
		Latter_Half
	};
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneManager();
	/// <summary>
	/// デスコンストラクタ
	/// </summary>
	~SceneManager();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// 削除
	/// </summary>
	void Delete();

	//次のシーン予約
	void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }
	//シーンチェンジ時の演出
	void BlackOut();
	//シーンチェンジ時の演出
	void BlackOutDraw();
private://定義
	//今のシーン
	BaseScene* scene_ = nullptr;
	//次のシーン
	BaseScene* nextScene_ = nullptr;

	ShadowMap  m_shadowMapFar;

	float time = 0;


	SpriteData blackOut;//暗転用画像
	Vec3 blackPos[2] = {};
	float blackOutTime = 0.0f;
	int blackOutFlag = Stand_By;
};