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
private://定義
	//今のシーン
	BaseScene* scene_ = nullptr;
	//次のシーン
	BaseScene* nextScene_ = nullptr;

	ShadowMap  m_shadowMapFar;
};