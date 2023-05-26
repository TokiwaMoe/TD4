#include "Road.h"

Road::GimmickParameter::GimmickParameter() :
	flag(false),
	speed(0.0f),
	limit{}
{
}

Road::GimmickParameter::GimmickParameter(bool flag, float speed, const Vec2& limit) :
	flag(flag),
	speed(speed),
	limit(limit)
{
}

Road::Road() :
	sprite{},
	pos{},
	size{},
	anchorpoint(0.5f, 0.5f),
	type(RoadType::ROAD),
	gimmick(Gimmick::NO_GIMMICK),
	parameter{},
	back(BackType::PALM),
	isFlipX(false),
	isFlipY(false),
	initPos(pos),
	initSize(size)
{
}

void Road::Init()
{
	initPos = pos;
	initSize = size;

	if (type != BACK)
	{
		sprite = Sprite::Get()->SpriteCreate(L"Resources/white1x1.png");
		return;
	}

	switch (back)
	{
		// ヤシの木
	case PALM:
		sprite = Sprite::Get()->SpriteCreate(L"Resources/palm.png");
		break;
		// 鹿
	case DEER:
		sprite = Sprite::Get()->SpriteCreate(L"Resources/shika.png");
		break;
	default:
		break;
	}
}
