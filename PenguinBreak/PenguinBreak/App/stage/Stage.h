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

public: //サブクラス
	class Back : public Road
	{
	public:
		// 背景オブジェクトのタイプ
		enum BackType
		{
			PALM,		//ヤシの木
			DEER,		//鹿
			HAND,		//手
			FUGU,		//ふぐ
			CAT,		//猫
			LAZY,		//なまけもの
			EYE,		//目
			FISH,		//サカナ
			SEA,		//海
			SUN,		//太陽
			FISH_BIG,	//大きい魚
			DINOSAUR,	//恐竜
			CYPYBARA,	//カピバラ
			JELLYFISH,	//クラゲ
			LIZARD,		//トカゲ
			SNAKE		//蛇
		};

	public: //メンバ変数
		BackType back = BackType::PALM; //背景オブジェクトの種類
		bool isFlipX = false;
		bool isFlipY = false;

	public: //メンバ関数
		Road ConvertRoad() { return *this; }
		void Init() override;
	};

	struct JsonData
	{
		std::vector<Back> objects = {};
	};

public: //定数
	static const int STAGE_COUNT = 4; //ステージの数
	static const int BACK_COUNT = 3;  //背景の数
	static const Vec2 ROAD_OFFSET; //道の余白分

private: //静的メンバ変数
	static Vec2 ROAD_SIZE; //道の最低限の大きさ
	static Vec2 PLAYER_SIZE; //プレイヤーの大きさ（エディター用変数）

private: //メンバ変数
	std::vector<Road> boxes; //道のオブジェクト
	std::vector<Back> backObjects; //背景オブジェクト
	size_t startIndex;
	size_t restartIndex;
	size_t goalIndex;
	size_t roadCount;
	unsigned short scale; //プレイヤーのサイズ ※数字が小さいほどプレイヤーのサイズは大きくなる
	size_t switchCount;

	SpriteData starSprite; //リスタート地点に描画する星のスプライトデータ

public: //メンバ関数
	// 初期化
	void Init(const Vec2& playerSize);
	// ギミックの更新
	void Update();
	// 描画
	void Draw(float offsetX = 0.0f, float offsetY = 0.0f);

	// ステージの書き込み
	void WriteStage(const std::string& stageName);
	// ステージの読み込み
	JsonData* LoadStage(const std::string& jsonFile);
	// ステージの切り替え
	void ChangeStage(int stageNumber);
	// 背景オブジェクトの読み込み
	JsonData* LoadBack(const std::string& jsonFile);
	// 背景オブジェクトの切り替え
	void ChangeBack(int backNumber);
	// セーブポイントの切り替え
	void ChangeRestart(size_t num);

	size_t GetBoxSize() const { return boxes.size(); }
	// 道の数の取得
	size_t GetRoadCount() const { return roadCount; }
	// 中心座標の取得
	Vec2 GetPos(size_t num) const { return boxes[num].GetAnchorpointPos(Vec2(0.5f, 0.5f)); }
	// スプライトサイズの取得
	Vec2 GetSize(size_t num) const { return boxes[num].size; }
	// アンカーポイントの取得
	Vec2 GetAnchorpoint(size_t num) const { return boxes[num].anchorpoint; }
	// 指定したアンカーポイントに対応する座標の取得
	Vec2 GetAnchorpointPos(size_t num, const Vec2& anchorpoint) const { return boxes[num].GetAnchorpointPos(anchorpoint); }
	// プレイヤーのサイズ
	unsigned short GetScale() const { return scale; }
	// 種類の取得
	Road::RoadType GetType(size_t num) const { return boxes[num].type; }
	Vec2 GetStartPos() const { return GetPos(startIndex); }
	size_t GetStart() const { return startIndex; }
	size_t GetRestart() const { return restartIndex; }
	Vec2 GetGoalPos() const { return GetPos(goalIndex); }
	size_t GetGoal() const { return goalIndex; }

	// 座標の設定
	void SetPos(size_t num, const Vec2& pos) { boxes[num].pos = pos; }
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
	// リセット
	void EditerReset();
	// 最前面に持ってくる
	void BringForefront(size_t num);
	// プレイヤーのサイズ
	void SetScale(unsigned short scale);
private:
	// ギミックの更新
	template<class T>
	void GimmickUpdate(std::vector<T>& object);
	// 道が狭まるギミック(全方向)
	void ScaleGimmick(Road& road);
	// 道が狭まるギミック(単一の方向)
	void DirectionScaleGimmick(Road& road);
	// 道が移動するギミック（折り返し）
	void MoveGimmick(Road& road);
	// 道が移動するギミック（ループ）
	void LoopMoveGimmick(Road& road);
	// 道が移動するギミック（一回きり）
	void OnlyMoveGimmick(Road& road);

	// 上限を超えたかどうか
	bool IsUpOver(float* pos, float* size, float limit, float speed, float scale);
	// 下限を超えたかどうか
	bool IsDownOver(float* pos, float* size, float limit, float speed, float scale);
};

template<class T>
inline void Stage::GimmickUpdate(std::vector<T>& object)
{
	for (auto& i : object)
	{
		switch (i.gimmick)
		{
		case Road::Gimmick::SCALE:
			ScaleGimmick(i);
			break;
		case Road::Gimmick::DIRECTION_SCALE:
			DirectionScaleGimmick(i);
			break;
		case Road::Gimmick::MOVE:
			MoveGimmick(i);
			break;
		case Road::Gimmick::LOOP_MOVE:
			LoopMoveGimmick(i);
			break;
		case Road::Gimmick::ONLY_MOVE:
			OnlyMoveGimmick(i);
			break;
		default:
			continue;
			break;
		}
	}
}
