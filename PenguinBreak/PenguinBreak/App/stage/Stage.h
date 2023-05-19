#pragma once
#include "Sprite.h"
#include <vector>
#include "./../../Library/Helper/LoadJson.h"

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

public: //サブクラス
	// 道のタイプ
	enum RoadType
	{
		ROAD,  //道
		START, //スタート
		GOAL,  //ゴール
		BACK,  //背景オブジェクト
	};

	// 背景オブジェクトのタイプ
	enum BackType
	{
		PALM, //ヤシの木
		DEER, //鹿
	};

	// ギミックのタイプ
	enum Gimmick
	{
		NO_GIMMICK,      //ギミック無し
		SCALE,           //道が狭まるギミック(全方向)
		MOVE,            //道が移動するギミック
		DIRECTION_SCALE, //道が狭まるギミック(単一の方向)
	};

	// ギミックを動かす為に使う値
	class GimmickParameter
	{
	private:
		bool flag;
		float speed;
		Vec2 limit;

	public:
		GimmickParameter();
		GimmickParameter(bool flag, float speed, const Vec2& limit);

		void ChangeFlag() { flag = !flag; }
		const bool GetFlag() const { return flag; }
		const float GetSpeed() const { return speed; }
		const Vec2 GetLimit() const { return limit; }
	};

	class Road
	{
	private:
		SpriteData sprite;

		Vec2 initPos;
		Vec2 initSize;
	public:
		RoadType type;
		Gimmick gimmick;
		GimmickParameter parameter;
		BackType back; //背景オブジェクトの種類
		bool isFlipX;
		bool isFlipY;

		Vec2 pos;
		Vec2 size;
		Vec2 offset;

	public:
		Road();
		void Init();

		SpriteData& GetSprite() { return sprite; }
		const Vec2 GetInitPos() const { return initPos; }
		const Vec2 GetInitSize() const { return initSize; }
	};

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
	// 種類の取得
	RoadType GetType(size_t num) const { return boxes[num].type; }
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

	void Anchorpoint2Center(size_t num, const Vec2& anchorpoint);

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
