#pragma once
#include "Sprite.h"

class Road
{
public: //サブクラス
	// 道のタイプ
	enum RoadType
	{
		ROAD,  //道
		START, //スタート
		GOAL,  //ゴール
		WALL,  //穴
		BACK,  //背景オブジェクト
	};

	// ギミックのタイプ
	enum Gimmick
	{
		NO_GIMMICK,      //ギミック無し
		SCALE,           //道が狭まるギミック(全方向)
		MOVE,            //道が移動するギミック（折り返し）
		DIRECTION_SCALE, //道が狭まるギミック(単一の方向)
		LOOP_MOVE,       //道が移動するギミック（ループ）
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

private: //変数
	Vec2 initPos;
	Vec2 initSize;
protected:
	SpriteData sprite;
public:
	RoadType type;
	Gimmick gimmick;
	GimmickParameter parameter;

	Vec2 pos;
	Vec2 size;
	Vec2 offset;
	Vec2 anchorpoint;

public: //関数
	Road();
	virtual void Init();

	SpriteData& GetSprite() { return sprite; }
	const Vec2 GetPos() const { return pos + offset; }
	const Vec2 GetInitPos() const { return initPos; }
	const Vec2 GetInitSize() const { return initSize; }
	const Vec2 GetAnchorpointPos(const Vec2& uv) const;
};
