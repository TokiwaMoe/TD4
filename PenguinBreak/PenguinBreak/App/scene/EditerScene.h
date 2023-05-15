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
class EditerScene :public BaseScene
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EditerScene();
	/// <summary>
	/// デスコンストラクタ
	/// </summary>
	~EditerScene();
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
private:
	/// <summary>
	/// マウスカーソルがどの道にいるか
	/// </summary>
	/// <returns>ステージのインデックス（-1の時はどこにも属していない）</returns>
	int GetStageIndex2MousePos();

private://定義
	std::unique_ptr<LightGroup>lightGroup;

	Player* player = nullptr;
	Stage* stage = nullptr;
};