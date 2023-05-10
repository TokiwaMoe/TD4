#include "Stage.h"
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
	initPos(pos),
	initSize(size)
{
}

Stage::Road::Road(const Vec2& pos, const Vec2& size) :
	sprite{},
	pos(pos),
	size(size),
	type(RoadType::ROAD),
	gimmick(Gimmick::NO_GIMMICK),
	parameter{},
	initPos(pos),
	initSize(size)
{
}

void Stage::Road::BoxInit()
{
	this->sprite = Sprite::Get()->SpriteCreate(L"Resources/white1x1.png");
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

void Stage::Init(const Vec2& playerSize)
{
	const float ROAD_OFFSET = 30.0f; //道の余白
	const float ROAD_WIDTH = (playerSize.x > playerSize.y) ? playerSize.x : playerSize.y;

	auto file = LoadStage("stage1");
	boxes.clear();
	boxes = file->objects;
}

void Stage::GimmickUpdate()
{
	for (auto& i : boxes)
	{
		switch (i.GetGimmick())
		{
		case Gimmick::SCALE:
			ScaleGimmick(i);
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
		switch (i.GetType())
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

		Sprite::Get()->Draw(i.GetSprite(), i.pos + Vec2(offsetX, offsetY), i.size.x, i.size.y, Vec2(0.5f, 0.5f), color);
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
		Vec2 pos = { object["pos"][0], object["pos"][1] };
		Vec2 size = { object["size"][0], object["size"][1] };
		levelData->objects.emplace_back(Road(pos, size));
		auto& objectData = levelData->objects.back();

		objectData.BoxInit();
		objectData.SetType(object["type"]);

		// ギミックタイプの読み込み
		if (objectData.GetType() == RoadType::ROAD)
		{
			objectData.SetGimmick(object["gimmick"]);
		}
		else
		{
			objectData.SetGimmick(Gimmick::NO_GIMMICK);
		}

		// ギミック用のパラメータの読み込み
		if (objectData.GetGimmick() != Gimmick::NO_GIMMICK)
		{
			bool flag = object["parameter"]["flag"];
			float speed = object["parameter"]["speed"];
			Vec2 limit = { object["parameter"]["limit"][0], object["parameter"]["limit"][1]};

			objectData.ParameterInit(GimmickParameter(flag, speed, limit));
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
		data["objects"][i] = {
			{"type", boxes[i].GetType()},
			{"pos", {boxes[i].pos.x, boxes[i].pos.y}},
			{"size", {boxes[i].size.x, boxes[i].size.y}},
		};

		// ギミックタイプの書き込み
		if (boxes[i].GetType() == RoadType::ROAD)
		{
			data["objects"][i]["gimmick"] = boxes[i].GetGimmick();
		}

		// ギミック用のパラメータの書き込み
		if (boxes[i].GetGimmick() != Gimmick::NO_GIMMICK)
		{
			data["objects"][i]["parameter"] = {
				{"flag", boxes[i].GetGimmickParameter().GetFlag()},
				{"speed", boxes[i].GetGimmickParameter().GetSpeed()},
				{"limit", {boxes[i].GetGimmickParameter().GetLimit().x, boxes[i].GetGimmickParameter().GetLimit().y}},
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
		(road.GetGimmickParameter().GetLimit().x ? 1.0f : 0.0f),
		(road.GetGimmickParameter().GetLimit().y ? 1.0f : 0.0f)
	};

	if (road.GetGimmickParameter().GetFlag())
	{
		// 道幅が広くなる
		limit += road.GetGimmickParameter().GetLimit();
		road.size += isMove * road.GetGimmickParameter().GetSpeed();

		if ((isMove.x && (road.size.x >= limit.x)) || (isMove.y && (road.size.y >= limit.y)))
		{
			road.GetGimmickParameter().ChangeFlag();
		}
	}
	else
	{
		// 道幅が狭くなる
		limit -= road.GetGimmickParameter().GetLimit();
		road.size -= isMove * road.GetGimmickParameter().GetSpeed();

		if ((isMove.x && (road.size.x <= limit.x)) || (isMove.y && (road.size.y <= limit.y)))
		{
			road.GetGimmickParameter().ChangeFlag();
		}
	}
}

void Stage::MoveGimmick(Road& road)
{
	Vec2 limit = road.GetInitPos();
	// 軸単位で動かすかどうか
	const Vec2 isMove = {
		(road.GetGimmickParameter().GetLimit().x ? 1.0f : 0.0f),
		(road.GetGimmickParameter().GetLimit().y ? 1.0f : 0.0f)
	};

	if (road.GetGimmickParameter().GetFlag())
	{
		// +方向に移動する
		limit += road.GetGimmickParameter().GetLimit();
		road.pos += isMove * road.GetGimmickParameter().GetSpeed();

		if ((isMove.x && (road.pos.x >= limit.x)) || (isMove.y && (road.pos.y >= limit.y)))
		{
			road.GetGimmickParameter().ChangeFlag();
		}
	}
	else
	{
		// -方向に移動する
		limit -= road.GetGimmickParameter().GetLimit();
		road.pos -= isMove * road.GetGimmickParameter().GetSpeed();

		if ((isMove.x && (road.pos.x <= limit.x)) || (isMove.y && (road.pos.y <= limit.y)))
		{
			road.GetGimmickParameter().ChangeFlag();
		}
	}
}
