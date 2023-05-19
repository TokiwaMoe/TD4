#pragma once
#include "DirectXBase.h"
#include"DebugText.h"
#include<memory>
#include"BaseScene.h"
#include"Player.h"
#include"../stage/Stage.h"
#include<vector>
#include<string>

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
	/// 操作説明の描画
	/// </summary>
	void OperationDraw();
	/// <summary>
	/// マウスカーソルがどの道にいるか
	/// </summary>
	/// <returns>ステージのインデックス（-1の時はどこにも属していない）</returns>
	int GetStageIndex2MousePos();

private://定義
	SpriteData cursor;
	SpriteData frame;

	std::unique_ptr<Player>player;
	Stage* stage = nullptr;

	int roadIndex = -1;
	bool isClick = false;

	std::vector<std::string> operationText;
};