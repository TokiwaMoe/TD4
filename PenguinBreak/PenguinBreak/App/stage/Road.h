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
		HOLE,  //穴
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

private: //変数
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
	Vec2 anchorpoint;

public: //関数
	Road();
	void Init();

	SpriteData& GetSprite() { return sprite; }
	const Vec2 GetInitPos() const { return initPos; }
	const Vec2 GetInitSize() const { return initSize; }
};
