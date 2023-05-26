#pragma once
#include "Road.h"
#include <vector>

class Stage final
{
private: //シングルトン化
	Stage();
	Stage(const Stage&) = delete;
	~Stage();
	Stage& operator=(const Stage&) = delete;
public:
	static Stage* GetInstance()
	{
		static Stage instance;
		return &instance;
	}

	struct JsonData
	{
		std::vector<Road> objects = {};
	};

public: //定数
	static const int STAGE_COUNT = 2;

private: //静的メンバ変数
	static Vec2 ROAD_SIZE;

private: //メンバ変数
	std::vector<Road> boxes;
	size_t startIndex;
	size_t goalIndex;
	size_t roadCount;

public: //メンバ関数
	// 初期化
	void Init();
	// ギミックの更新
	void GimmickUpdate();
	// 描画
	void Draw(float offsetX = 0.0f, float offsetY = 0.0f);

	// ステージの書き込み
	void WriteStage(const std::string& stageName);
	// ステージの読み込み
	JsonData* LoadStage(const std::string& jsonFile);
	// ステージの切り替え
	void ChengeStage(int stageNumber);

	size_t GetBoxSize() const { return boxes.size(); }
	// 座標の取得
	Vec2 GetPos(size_t num) const { return boxes[num].pos + boxes[num].offset; }
	// スプライトサイズの取得
	Vec2 GetSize(size_t num) const { return boxes[num].size; }
	// アンカーポイントの取得
	Vec2 GetAnchorpoint(size_t num) const { return boxes[num].anchorpoint; }
	// 種類の取得
	Road::RoadType GetType(size_t num) const { return boxes[num].type; }
	// 道の数
	size_t GetRoadCount() const { return roadCount; }
	Vec2 GetStartPos() const { return GetPos(startIndex); }
	size_t GetStart() const { return startIndex; }
	Vec2 GetGoalPos() const { return GetPos(goalIndex); }
	size_t GetGoal() const { return goalIndex; }

	// 座標の設定
	void SetPos(size_t num, const Vec2& pos) { boxes[num].offset = pos; }
	// サイズの設定
	void SetSize(size_t num, const Vec2& size) { boxes[num].size = size; }
	// 各インデックスの設定
	void SetIndex();

	// アンカーポイントを指定した場所に持ってくる
	void MoveAnchorpoint(size_t num, const Vec2& anchorpoint) { boxes[num].anchorpoint = anchorpoint; }

	/* エディター用関数*/
	// 初期化(エディター用)
	void EditerInit(const Vec2& playerSize);
	// 道の生成
	void Create();
	// 道の削除
	void Delete(size_t num);
private:
	// 道が狭まるギミック(全方向)
	void ScaleGimmick(Road& road);
	// 道が狭まるギミック(単一の方向)
	void DirectionScaleGimmick(Road& road);
	// 道が移動するギミック
	void MoveGimmick(Road& road);

	// 上限を超えたかどうか
	bool IsUpOver(float* pos, float* size, float limit, float speed, float scale);
	// 下限を超えたかどうか
	bool IsDownOver(float* pos, float* size, float limit, float speed, float scale);
};
