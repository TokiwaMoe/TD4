﻿#include "Stage.h"
#include <fstream>
#include <json.hpp>

Stage::GimmickParameter::GimmickParameter() :
	flag(false),
	speed(0.0f),
	limit{}
{
}

Stage::GimmickParameter::GimmickParameter(bool flag,float speed, const Vec2& limit) :
	flag(flag),
	speed(speed),
	limit(limit)
{
}

Stage::Road::Road() :
	sprite{},
	pos{},
	size{},
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

void Stage::Road::Init()
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

Stage::Stage() :
	boxes{},
	startIndex(0),
	goalIndex(0)
{
}

Stage::~Stage()
{
}

void Stage::Init()
{
}

void Stage::GimmickUpdate()
{
	for (auto& i : boxes)
	{
		switch (i.gimmick)
		{
		case Gimmick::SCALE:
			ScaleGimmick(i);
			break;
		case Gimmick::DIRECTION_SCALE:
			DirectionScaleGimmick(i);
			break;
		case Gimmick::MOVE:
			MoveGimmick(i);
			break;
		default:
			continue;
			break;
		}
	}
}

void Stage::Draw(float offsetX, float offsetY)
{
	for (auto& i : boxes)
	{
		Vec4 color = Vec4();

		switch (i.type)
		{
		case RoadType::START:
			color = Vec4(0.0f, 0.0f, 1.0f, 1.0f);
			break;
		case RoadType::GOAL:
			color = Vec4(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		default:
			color = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		}

		Sprite::Get()->Draw(i.GetSprite(), i.pos + i.offset + Vec2(offsetX, offsetY), i.size.x, i.size.y, Vec2(0.5f, 0.5f), color, i.isFlipX, i.isFlipY);
	}
}

Stage::JsonData* Stage::LoadStage(const std::string& jsonFile)
{
	// 連結してフルパスを得る
	const std::string fullpath = std::string("Resources/levels/") + jsonFile + ".json";

	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// レベルデータ格納用インスタンスを生成
	JsonData* levelData = new JsonData();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"])
	{
		assert(object.contains("type"));

		// 要素を追加
		levelData->objects.emplace_back(Road());
		auto& objectData = levelData->objects.back();
		objectData.type = object["type"];
		objectData.pos = Vec2(object["pos"][0], object["pos"][1]);
		objectData.size = Vec2(object["size"][0], object["size"][1]);
		objectData.offset = Vec2(object["offset"][0], object["offset"][1]);

		// 背景オブジェクトの読み込み
		if (objectData.type == RoadType::BACK)
		{
			objectData.back = object["back"];
			objectData.isFlipX = object["flipX"];
			objectData.isFlipY = object["flipY"];
		}

		objectData.Init();

		// ギミックタイプの読み込み
		if (objectData.type == RoadType::ROAD || objectData.type == RoadType::BACK)
		{
			objectData.gimmick = object["gimmick"];
		}
		else
		{
			objectData.gimmick = Gimmick::NO_GIMMICK;
		}

		// ギミック用のパラメータの読み込み
		if (objectData.gimmick != Gimmick::NO_GIMMICK)
		{
			bool flag = object["parameter"]["flag"];
			float speed = object["parameter"]["speed"];
			Vec2 limit = { object["parameter"]["limit"][0], object["parameter"]["limit"][1]};

			objectData.parameter = GimmickParameter(flag, speed, limit);
		}
	}

	startIndex = levelData->objects.size() - 2;
	goalIndex = levelData->objects.size() - 1;

	return levelData;
}

void Stage::ChengeStage(int stageNumber)
{
	auto file = LoadStage("stage" + std::to_string(stageNumber));
	boxes.clear();
	boxes = file->objects;
	delete file;
}

void Stage::WriteStage(const std::string& stageName)
{
	using namespace std;
	using json = nlohmann::json;

	// 連結してフルパスを得る
	const string fullpath = string("Resources/levels/") + stageName + ".json";

	// ファイルストリーム
	ofstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	json data;
	data = json::object();
	data["name"] = json::string_t("stage");
	data["objects"] = json::array();

	for (size_t i = 0; i < boxes.size(); i++)
	{
		auto& objectData = data["objects"][i];

		objectData = {
			{"type", boxes[i].type},
			{"pos", {boxes[i].pos.x, boxes[i].pos.y}},
			{"size", {boxes[i].size.x, boxes[i].size.y}},
			{"offset", {boxes[i].offset.x, boxes[i].offset.y}},
		};

		// 背景オブジェクトの種類の書き込み
		if (boxes[i].type == RoadType::BACK)
		{
			objectData["back"] = boxes[i].gimmick;
			objectData["flipX"] = boxes[i].isFlipX;
			objectData["flipY"] = boxes[i].isFlipY;
		}

		// ギミックタイプの書き込み
		if (boxes[i].type == RoadType::ROAD || boxes[i].type == RoadType::BACK)
		{
			objectData["gimmick"] = boxes[i].gimmick;
		}

		// ギミック用のパラメータの書き込み
		if (boxes[i].gimmick != Gimmick::NO_GIMMICK)
		{
			objectData["parameter"] = {
				{"flag", boxes[i].parameter.GetFlag()},
				{"speed", boxes[i].parameter.GetSpeed()},
				{"limit", {boxes[i].parameter.GetLimit().x, boxes[i].parameter.GetLimit().y}},
			};
		}
	}

	file << std::setw(4) << data << endl;
	file.close();
}

void Stage::ScaleGimmick(Road& road)
{
	Vec2 limit = road.GetInitSize();
	// 軸単位で動かすかどうか
	const Vec2 isMove = {
		(road.parameter.GetLimit().x ? 1.0f : 0.0f),
		(road.parameter.GetLimit().y ? 1.0f : 0.0f)
	};

	if (road.parameter.GetFlag())
	{
		// 道幅が広くなる
		limit += road.parameter.GetLimit();
		road.size += isMove * road.parameter.GetSpeed();

		if ((isMove.x && (road.size.x >= limit.x)) || (isMove.y && (road.size.y >= limit.y)))
		{
			road.parameter.ChangeFlag();
		}
	}
	else
	{
		// 道幅が狭くなる
		limit -= road.parameter.GetLimit();
		road.size -= isMove * road.parameter.GetSpeed();

		if ((isMove.x && (road.size.x <= limit.x)) || (isMove.y && (road.size.y <= limit.y)))
		{
			road.parameter.ChangeFlag();
		}
	}
}

void Stage::DirectionScaleGimmick(Road& road)
{
	Vec2 limit = road.GetInitSize();
	// 軸単位で動かすかどうか
	Vec2 moveDir = {};
	if (road.parameter.GetLimit().x > 0) { moveDir.x = +1.0f; }
	else if (road.parameter.GetLimit().x < 0) { moveDir.x = -1.0f; }
	if (road.parameter.GetLimit().y > 0) { moveDir.y = +1.0f; }
	else if (road.parameter.GetLimit().y < 0) { moveDir.y = -1.0f; }

	Vec2 speed = moveDir * road.parameter.GetSpeed() / 2.0f;
	Vec2 scale = {
		(road.parameter.GetLimit().x ? 1.0f : 0.0f),
		(road.parameter.GetLimit().y ? 1.0f : 0.0f)
	};
	scale *= road.parameter.GetSpeed();
	bool isOver = false;

	if (road.parameter.GetFlag())
	{
		if (moveDir.x > 0)
		{
			// 大きくなる
			isOver |= (moveDir.x &&
					   IsUpOver(&road.pos.x,
								&road.size.x,
								(limit + road.parameter.GetLimit()).x,
								speed.x,
								scale.x));
		}
		else
		{
			// 小さくなる
			isOver |= (moveDir.x &&
					   IsDownOver(&road.pos.x,
								  &road.size.x,
								  (limit + road.parameter.GetLimit()).x,
								  speed.x,
								  scale.x));
		}
		if (moveDir.y > 0)
		{
			// 大きくなる
			isOver |= (moveDir.y &&
					   IsUpOver(&road.pos.y,
								&road.size.y,
								(limit + road.parameter.GetLimit()).y,
								speed.y,
								scale.y));
		}
		else
		{
			// 小さくなる
			isOver |= (moveDir.y &&
					   IsDownOver(&road.pos.y,
								  &road.size.y,
								  (limit + road.parameter.GetLimit()).y,
								  speed.y,
								  scale.y));
		}
	}
	else
	{
		if (moveDir.x > 0)
		{
			// 小さくなる
			isOver |= (moveDir.x &&
					   IsDownOver(&road.pos.x,
								  &road.size.x,
								  (limit - road.parameter.GetLimit()).x,
								  speed.x,
								  scale.x));
		}
		else
		{
			// 大きくなる
			isOver |= (moveDir.x &&
					   IsUpOver(&road.pos.x,
								&road.size.x,
								(limit - road.parameter.GetLimit()).x,
								speed.x,
								scale.x));
		}
		if (moveDir.y > 0)
		{
			// 小さくなる
			isOver |= (moveDir.y &&
					   IsDownOver(&road.pos.y,
								  &road.size.y,
								  (limit - road.parameter.GetLimit()).y,
								  speed.y,
								  scale.y));
		}
		else
		{
			// 大きくなる
			isOver |= (moveDir.y &&
					   IsUpOver(&road.pos.y,
								&road.size.y,
								(limit - road.parameter.GetLimit()).y,
								speed.y,
								scale.y));
		}
	}

	if (isOver)
	{
		road.parameter.ChangeFlag();
	}
}

void Stage::MoveGimmick(Road& road)
{
	Vec2 limit = road.GetInitPos();
	// 軸単位で動かすかどうか
	const Vec2 isMove = {
		(road.parameter.GetLimit().x ? 1.0f : 0.0f),
		(road.parameter.GetLimit().y ? 1.0f : 0.0f)
	};

	if (road.parameter.GetFlag())
	{
		// +方向に移動する
		limit += road.parameter.GetLimit();
		road.pos += isMove * road.parameter.GetSpeed();

		if ((isMove.x && (road.pos.x >= limit.x)) || (isMove.y && (road.pos.y >= limit.y)))
		{
			road.parameter.ChangeFlag();
		}
	}
	else
	{
		// -方向に移動する
		limit -= road.parameter.GetLimit();
		road.pos -= isMove * road.parameter.GetSpeed();

		if ((isMove.x && (road.pos.x <= limit.x)) || (isMove.y && (road.pos.y <= limit.y)))
		{
			road.parameter.ChangeFlag();
		}
	}
}

bool Stage::IsUpOver(float* pos, float* size, float limit, float speed, float scale)
{
	*pos += speed;
	*size += scale;

	return *size >= limit;
}

bool Stage::IsDownOver(float* pos, float* size, float limit, float speed, float scale)
{
	*pos -= speed;
	*size -= scale;

	return *size <= limit;
}
