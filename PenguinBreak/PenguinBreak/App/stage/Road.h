#pragma once
#include "Sprite.h"
#include <vector>

class Road
{
public: //サブクラス
	// 道のタイプ
	enum RoadType
	{
		START,  //スタート
		GOAL,   //ゴール
		ROAD,   //道
		WALL,   //穴
		SAVE,   //セーブポイント
		SWITCH, //スイッチ
		BACK,   //背景オブジェクト
	};

	// ギミックのタイプ
	enum Gimmick
	{
		NO_GIMMICK,      //ギミック無し
		SCALE,           //道が狭まるギミック(全方向)
		MOVE,            //道が移動するギミック（折り返し）
		DIRECTION_SCALE, //道が狭まるギミック(単一の方向)
		LOOP_MOVE,       //道が移動するギミック（ループ）
		ONLY_MOVE,       //道が移動するギミック（1回きり）
		ONLY_SCALE,      //道が狭まるギミック（1回きり）
	};

	// ギミックを動かす為に使う値
	class GimmickParameter
	{
	private:
		Gimmick type;
		bool flag;
		float speed;
		Vec2 limit;

	public:
		GimmickParameter();
		GimmickParameter(Gimmick type, bool flag, float speed, const Vec2& limit);

		void ChangeFlag() { flag = !flag; }
		const Gimmick GetGimmick() const { return type; }
		const bool GetFlag() const { return flag; }
		const float GetSpeed() const { return speed; }
		const Vec2 GetLimit() const { return limit; }
	};

private: //変数
	Vec2 initPos;
	Vec2 initSize;
protected:
	SpriteData sprite;
public:
	RoadType type;
	std::vector<GimmickParameter> parameter;
	bool isPlayer;
	bool isOldPlayer;

	Vec2 pos;
	Vec2 size;
	Vec2 offset;
	Vec2 anchorpoint;

public: //関数
	Road();
	virtual void Init();
	void ResetInitPos();

	SpriteData& GetSprite() { return sprite; }
	const Vec2 GetPos() const { return pos; }
	const Vec2 GetInitPos() const { return initPos; }
	const Vec2 GetInitSize() const { return initSize; }
	const Vec2 GetAnchorpointPos(const Vec2& uv) const;
	GimmickParameter& GetGimmickParameter(const size_t& num);
	const size_t GetGimmickCount() const { return parameter.size(); }

	void SetPlayerFlag(bool flag) { isOldPlayer = isPlayer; isPlayer = flag; }
};
