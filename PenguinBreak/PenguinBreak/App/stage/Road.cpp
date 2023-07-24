#include "Road.h"

Road::GimmickParameter::GimmickParameter() :
	type(Gimmick::NO_GIMMICK),
	flag(false),
	speed(0.0f),
	limit{}
{
}

Road::GimmickParameter::GimmickParameter(Gimmick type, bool flag, float speed, const Vec2& limit) :
	type(type),
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
	parameter{},
	isPlayer(false),
	isOldPlayer(isPlayer),
	initPos(pos),
	initSize(size)
{
}

void Road::Init()
{
	initPos = pos;
	initSize = size;

	if (type == BACK) return;
	sprite = Sprite::Get()->SpriteCreate(L"Resources/white1x1.png");
}

void Road::ResetInitPos()
{
	initPos = pos;
}

const Vec2 Road::GetAnchorpointPos(const Vec2& uv) const
{
	Vec2 uvPos = uv - anchorpoint;
	return GetPos() + Vec2(size.x * uvPos.x, size.y * uvPos.y);
}

Road::GimmickParameter& Road::GetGimmickParameter(const size_t& num)
{
	if (num >= parameter.size()) return parameter.back();
	return parameter[num];
}
