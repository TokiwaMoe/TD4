#include "Stage.h"
#include <fstream>
#include <json.hpp>
#include "Input.h"

const Vec2 Stage::ROAD_OFFSET = { 30.0f, 30.0f };
Vec2 Stage::ROAD_SIZE = Vec2();
Vec2 Stage::PLAYER_SIZE = Vec2();

void Stage::Back::Init()
{
	Road::Init();

	if (type != RoadType::BACK) return;
	switch (back)
	{
	case Stage::Back::PALM:
		sprite = Sprite::Get()->SpriteCreate(L"Resources/stage/palm.png");
		break;
	case Stage::Back::DEER:
		sprite = Sprite::Get()->SpriteCreate(L"Resources/stage/shika.png");
		break;
	case Stage::Back::HAND:
		sprite = Sprite::Get()->SpriteCreate(L"Resources/stage/hand.png");
		break;
	case Stage::Back::FUGU:
		sprite = Sprite::Get()->SpriteCreate(L"Resources/stage/fugu.png");
		break;
	case Stage::Back::CAT:
		sprite = Sprite::Get()->SpriteCreate(L"Resources/stage/cat.png");
		break;
	case Stage::Back::LAZY:
		sprite = Sprite::Get()->SpriteCreate(L"Resources/stage/namake.png");
		break;
	case Stage::Back::EYE:
		sprite = Sprite::Get()->SpriteCreate(L"Resources/stage/eye.png");
		break;
	case Stage::Back::FISH:
		sprite = Sprite::Get()->SpriteCreate(L"Resources/stage/fish.png");
		break;
	case Stage::Back::SEA:
		sprite = Sprite::Get()->SpriteCreate(L"Resources/stage/sea.png");
		break;
	case Stage::Back::SUN:
		sprite = Sprite::Get()->SpriteCreate(L"Resources/stage/sun.png");
		break;
	case Stage::Back::FISH_BIG:
		sprite = Sprite::Get()->SpriteCreate(L"Resources/stage/fish_big.png");
		break;
	case Stage::Back::DINOSAUR:
		sprite = Sprite::Get()->SpriteCreate(L"Resources/stage/dinosaur.png");
		break;
	case Stage::Back::CAPYBARA:
		sprite = Sprite::Get()->SpriteCreate(L"Resources/stage/capybara.png");
		break;
	case Stage::Back::JELLYFISH:
		sprite = Sprite::Get()->SpriteCreate(L"Resources/stage/jellyfish.png");
		break;
	case Stage::Back::LIZARD:
		sprite = Sprite::Get()->SpriteCreate(L"Resources/stage/lizard.png");
		break;
	case Stage::Back::SNAKE:
		sprite = Sprite::Get()->SpriteCreate(L"Resources/stage/snake.png");
		break;
	case Stage::Back::GRASS:
		sprite = Sprite::Get()->SpriteCreate(L"Resources/stage/grass_back.png");
		break;
	case Stage::Back::NIGHT:
		sprite = Sprite::Get()->SpriteCreate(L"Resources/stage/night_back.png");
		break;
	case Stage::Back::NOSE:
		sprite = Sprite::Get()->SpriteCreate(L"Resources/stage/nose.png");
		break;
	case Stage::Back::MUSEUM:
		sprite = Sprite::Get()->SpriteCreate(L"Resources/stage/museum_back.png");
		break;
	default:
		break;
	}
}

Stage::Stage() :
	boxes{},
	backObjects{},
	startIndex(0),
	restartIndex(startIndex),
	goalIndex(0),
	roadCount(2),
	scale(1),
	switchCount(0)
{
}

Stage::~Stage()
{
}

void Stage::Init(const Vec2& playerSize)
{
	PLAYER_SIZE = playerSize;
	switchCount = 0;
	starSprite = Sprite::Get()->SpriteCreate(L"Resources/Particle/particle3.png");
}

void Stage::Update()
{
	// 背景オブジェクトの更新
	GimmickUpdate(backObjects);
	// 道の更新
	GimmickUpdate(boxes);
}

void Stage::Draw(float offsetX, float offsetY)
{
	// 背景オブジェクトの描画
	for (auto& i : backObjects)
	{
		Sprite::Get()->Draw(i.GetSprite(), i.GetPos() + Vec2(offsetX, offsetY), i.size.x, i.size.y, Vec2(0.5f, 0.5f), Vec4(1.0f, 1.0f, 1.0f, 1.0f), i.isFlipX, i.isFlipY);
	}

	// 道の描画
	for (auto& i : boxes)
	{
		Vec4 color = Vec4();

		switch (i.type)
		{
		case Road::RoadType::START:
		case Road::RoadType::SAVE:
			color = Vec4(0.0f, 0.0f, 1.0f, 1.0f);
			break;
		case Road::RoadType::GOAL:
			color = Vec4(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		case Road::RoadType::WALL:
			color = Vec4(1.0f, 1.0f, 0.0f, 1.0f);
			break;
		case Road::RoadType::SWITCH:
			color = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
			break;
		default:
			color = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		}

		Sprite::Get()->Draw(i.GetSprite(), i.GetPos() + Vec2(offsetX, offsetY), i.size.x, i.size.y, Vec2(0.5f, 0.5f), color);
	}

	Sprite::Get()->Draw(starSprite, boxes[restartIndex].GetPos() + Vec2(offsetX, offsetY), ROAD_SIZE.x, ROAD_SIZE.x, Vec2(0.5f, 0.5f), Vec4(1.0f, 1.0f, 0.5f, 1.0f));
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

	// 正しいファイルかチェック
	assert(deserialized.is_object());

	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());
	std::string name = deserialized["name"];
	assert(name.compare("stage") == 0);

	assert(deserialized.contains("player"));
	assert(deserialized["player"].is_number());
	scale = deserialized["player"];
	assert(scale != 0);

	// レベルデータ格納用インスタンスを生成
	JsonData* levelData = new JsonData();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"])
	{
		assert(object.contains("type"));

		// 要素を追加
		levelData->objects.emplace_back();
		auto& objectData = levelData->objects.back();
		objectData.type = object["type"];
		objectData.pos = Vec2(object["pos"][0], object["pos"][1]);
		objectData.size = Vec2(object["size"][0], object["size"][1]);
		objectData.offset = Vec2(object["offset"][0], object["offset"][1]);
		objectData.pos += objectData.offset;

		objectData.Init();

		// ギミックの読み込み
		if (object.contains("gimmick"))
		{
			auto& gimmickArray = object["gimmick"];
			for (auto& i : gimmickArray)
			{
				Road::Gimmick type = i["type"];
				bool flag = false;
				float speed = 0.0f;
				Vec2 limit = Vec2();

				if (type != Road::Gimmick::NO_GIMMICK)
				{
					flag = i["flag"];
					speed = i["speed"];
					limit = { i["limit"][0], i["limit"][1] };
				}

				objectData.parameter.push_back(Road::GimmickParameter(type, flag, speed, limit));
			}
		}
		else if (objectData.type == Road::RoadType::SWITCH)
		{
			objectData.parameter.push_back(Road::GimmickParameter());
		}
	}
	ROAD_SIZE = (PLAYER_SIZE / scale) + (ROAD_OFFSET / scale);

	return levelData;
}

void Stage::ChangeStage(int stageNumber)
{
	if (stageNumber <= 0 || stageNumber > STAGE_COUNT) return;

	auto file = LoadStage("stage" + std::to_string(stageNumber));
	boxes.clear();
	for (auto& i : file->objects) boxes.push_back(i.ConvertRoad());
	SetIndex();
	delete file;
}

void Stage::ChangeStage(JsonData* file)
{
	boxes.clear();
	for (auto& i : file->objects) boxes.push_back(i.ConvertRoad());
}

Stage::JsonData* Stage::LoadBack(const std::string& jsonFile)
{
	// 連結してフルパスを得る
	const std::string fullpath = std::string("Resources/") + jsonFile + ".json";

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

	// 正しいファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());
	std::string name = deserialized["name"];
	assert(name.compare("back") == 0);

	// レベルデータ格納用インスタンスを生成
	JsonData* backData = new JsonData();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"])
	{
		assert(object.contains("back"));

		// 要素を追加
		backData->objects.emplace_back();
		auto& objectData = backData->objects.back();
		objectData.type = Road::RoadType::BACK;
		objectData.pos = Vec2(object["pos"][0], object["pos"][1]);
		objectData.size = Vec2(object["size"][0], object["size"][1]);
		objectData.offset = Vec2(object["offset"][0], object["offset"][1]);
		objectData.pos += objectData.offset;
		objectData.back = object["back"];
		objectData.isFlipX = object["flipX"];
		objectData.isFlipY = object["flipY"];

		objectData.Init();

		// ギミック用のパラメータの読み込み
		if (object.contains("gimmick"))
		{
			auto& gimmickArray = object["gimmick"];
			for (auto& i : gimmickArray)
			{
				Road::Gimmick type = i["type"];
				bool flag = false;
				float speed = 0.0f;
				Vec2 limit = Vec2();

				if (type != Road::Gimmick::NO_GIMMICK)
				{
					flag = i["flag"];
					speed = i["speed"];
					limit = { i["limit"][0], i["limit"][1] };
				}

				objectData.parameter.push_back(Road::GimmickParameter(type, flag, speed, limit));
			}
		}
	}

	return backData;
}

void Stage::ChangeBack(int backNumber)
{
	if (BACK_COUNT - 1 <= 0) return;
	if (backNumber <= 0 || backNumber > BACK_COUNT) backNumber = BACK_COUNT - 1;

	auto file = LoadBack("back" + std::to_string(backNumber));
	backObjects.clear();
	backObjects = file->objects;
	delete file;
}

void Stage::ChangeRestart(size_t num)
{
	if (num == restartIndex) return;

	if (boxes[num].type == Road::RoadType::START ||
		boxes[num].type == Road::RoadType::SAVE) {
		restartIndex = num;
	}
}

void Stage::SwitchCount(size_t num)
{
	if (boxes[num].type != Road::RoadType::SWITCH) return;
	if (boxes[num].isOldPlayer == true) return;

	boxes[num].GetGimmickParameter(switchCount).ChangeFlag();
	if (boxes[num].GetGimmickParameter(switchCount).GetFlag())
	{
		switchCount++;
	}
	else
	{
		switchCount--;
	}
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
	data["player"] = json::number_unsigned_t(scale);
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

		// ギミックタイプの書き込み
		if (boxes[i].type != Road::RoadType::START && boxes[i].type != Road::RoadType::GOAL)
		{
			objectData["gimmick"] = boxes[i].parameter[0].GetGimmick();
		}

		// ギミック用のパラメータの書き込み
		if (boxes[i].parameter[0].GetGimmick() != Road::Gimmick::NO_GIMMICK)
		{
			objectData["parameter"] = {
				{"flag", boxes[i].GetGimmickParameter(switchCount).GetFlag()},
				{"speed", boxes[i].GetGimmickParameter(switchCount).GetSpeed()},
				{"limit", {boxes[i].GetGimmickParameter(switchCount).GetLimit().x, boxes[i].GetGimmickParameter(switchCount).GetLimit().y}},
			};
		}
	}

	file << std::setw(4) << data << endl;
	file.close();
}

void Stage::SetIndex()
{
	roadCount = 0;

	for (size_t i = 0; i < boxes.size(); i++)
	{
		if (boxes[i].type != Road::RoadType::WALL)
		{
			roadCount++;

			if (boxes[i].type == Road::RoadType::START)
			{
				startIndex = i;
				restartIndex = startIndex;
			}
			else if (boxes[i].type == Road::RoadType::GOAL)
			{
				goalIndex = i;
			}
		}
	}
}

void Stage::ScaleGimmick(Road& road)
{
	Vec2 limit = road.GetInitSize();
	// 軸単位で動かすかどうか
	const Vec2 isMove = {
		(road.GetGimmickParameter(switchCount).GetLimit().x ? 1.0f : 0.0f),
		(road.GetGimmickParameter(switchCount).GetLimit().y ? 1.0f : 0.0f)
	};

	if (road.GetGimmickParameter(switchCount).GetFlag())
	{
		// 道幅が広くなる
		limit += road.GetGimmickParameter(switchCount).GetLimit();
		road.size += isMove * road.GetGimmickParameter(switchCount).GetSpeed();

		if ((isMove.x && (road.size.x >= limit.x)) || (isMove.y && (road.size.y >= limit.y)))
		{
			road.GetGimmickParameter(switchCount).ChangeFlag();
		}
	}
	else
	{
		// 道幅が狭くなる
		limit -= road.GetGimmickParameter(switchCount).GetLimit();
		road.size -= isMove * road.GetGimmickParameter(switchCount).GetSpeed();

		if ((isMove.x && (road.size.x <= limit.x)) || (isMove.y && (road.size.y <= limit.y)))
		{
			road.GetGimmickParameter(switchCount).ChangeFlag();
		}
	}
}

void Stage::DirectionScaleGimmick(Road& road)
{
	Vec2 limit = road.GetInitSize();
	// 軸単位で動かすかどうか
	Vec2 moveDir = {};
	if (road.GetGimmickParameter(switchCount).GetLimit().x > 0) { moveDir.x = +1.0f; }
	else if (road.GetGimmickParameter(switchCount).GetLimit().x < 0) { moveDir.x = -1.0f; }
	if (road.GetGimmickParameter(switchCount).GetLimit().y > 0) { moveDir.y = +1.0f; }
	else if (road.GetGimmickParameter(switchCount).GetLimit().y < 0) { moveDir.y = -1.0f; }

	Vec2 speed = moveDir * road.GetGimmickParameter(switchCount).GetSpeed() / 2.0f;
	Vec2 scale = {
		(road.GetGimmickParameter(switchCount).GetLimit().x ? 1.0f : 0.0f),
		(road.GetGimmickParameter(switchCount).GetLimit().y ? 1.0f : 0.0f)
	};
	scale *= road.GetGimmickParameter(switchCount).GetSpeed();
	bool isOver = false;

	if (road.GetGimmickParameter(switchCount).GetFlag())
	{
		if (moveDir.x > 0)
		{
			// 大きくなる
			isOver |= (moveDir.x &&
					   IsUpOver(&road.pos.x,
								&road.size.x,
								(limit + road.GetGimmickParameter(switchCount).GetLimit()).x,
								speed.x,
								scale.x));
		}
		else
		{
			// 小さくなる
			isOver |= (moveDir.x &&
					   IsDownOver(&road.pos.x,
								  &road.size.x,
								  (limit + road.GetGimmickParameter(switchCount).GetLimit()).x,
								  speed.x,
								  scale.x));
		}
		if (moveDir.y > 0)
		{
			// 大きくなる
			isOver |= (moveDir.y &&
					   IsUpOver(&road.pos.y,
								&road.size.y,
								(limit + road.GetGimmickParameter(switchCount).GetLimit()).y,
								speed.y,
								scale.y));
		}
		else
		{
			// 小さくなる
			isOver |= (moveDir.y &&
					   IsDownOver(&road.pos.y,
								  &road.size.y,
								  (limit + road.GetGimmickParameter(switchCount).GetLimit()).y,
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
								  (limit - road.GetGimmickParameter(switchCount).GetLimit()).x,
								  speed.x,
								  scale.x));
		}
		else
		{
			// 大きくなる
			isOver |= (moveDir.x &&
					   IsUpOver(&road.pos.x,
								&road.size.x,
								(limit - road.GetGimmickParameter(switchCount).GetLimit()).x,
								speed.x,
								scale.x));
		}
		if (moveDir.y > 0)
		{
			// 小さくなる
			isOver |= (moveDir.y &&
					   IsDownOver(&road.pos.y,
								  &road.size.y,
								  (limit - road.GetGimmickParameter(switchCount).GetLimit()).y,
								  speed.y,
								  scale.y));
		}
		else
		{
			// 大きくなる
			isOver |= (moveDir.y &&
					   IsUpOver(&road.pos.y,
								&road.size.y,
								(limit - road.GetGimmickParameter(switchCount).GetLimit()).y,
								speed.y,
								scale.y));
		}
	}

	if (isOver)
	{
		road.GetGimmickParameter(switchCount).ChangeFlag();
	}
}

void Stage::MoveGimmick(Road& road)
{
	Vec2 limit = road.GetInitPos();
	// 軸単位で動かすかどうか
	const Vec2 isMove = {
		(road.GetGimmickParameter(switchCount).GetLimit().x ? 1.0f : 0.0f),
		(road.GetGimmickParameter(switchCount).GetLimit().y ? 1.0f : 0.0f)
	};

	if (road.GetGimmickParameter(switchCount).GetFlag())
	{
		// +方向に移動する
		limit += road.GetGimmickParameter(switchCount).GetLimit();
		road.pos += isMove * road.GetGimmickParameter(switchCount).GetSpeed();

		if ((isMove.x && (road.pos.x >= limit.x)) || (isMove.y && (road.pos.y >= limit.y)))
		{
			road.GetGimmickParameter(switchCount).ChangeFlag();
		}
	}
	else
	{
		// -方向に移動する
		limit -= road.GetGimmickParameter(switchCount).GetLimit();
		road.pos -= isMove * road.GetGimmickParameter(switchCount).GetSpeed();

		if ((isMove.x && (road.pos.x <= limit.x)) || (isMove.y && (road.pos.y <= limit.y)))
		{
			road.GetGimmickParameter(switchCount).ChangeFlag();
		}
	}
}

void Stage::LoopMoveGimmick(Road& road)
{
	Vec2 limit = road.GetInitPos() + road.GetGimmickParameter(switchCount).GetLimit();
	// 軸単位で動かすかどうか
	Vec2 moveDir = {};
	if (road.GetGimmickParameter(switchCount).GetLimit().x > 0) { moveDir.x = +1.0f; }
	else if (road.GetGimmickParameter(switchCount).GetLimit().x < 0) { moveDir.x = -1.0f; }
	if (road.GetGimmickParameter(switchCount).GetLimit().y > 0) { moveDir.y = +1.0f; }
	else if (road.GetGimmickParameter(switchCount).GetLimit().y < 0) { moveDir.y = -1.0f; }

	road.pos += moveDir * road.GetGimmickParameter(switchCount).GetSpeed();

	if ((moveDir.x > 0 && (road.pos.x >= limit.x)) ||
		(moveDir.x < 0 && (road.pos.x <= limit.x)) ||
		(moveDir.y > 0 && (road.pos.y >= limit.y)) ||
		(moveDir.y < 0 && (road.pos.y <= limit.y)))
	{
		road.pos = road.GetInitPos() - road.GetGimmickParameter(switchCount).GetLimit();
	}
}

void Stage::OnlyMoveGimmick(Road& road)
{
	if (road.GetGimmickParameter(switchCount).GetFlag()) return;

	Vec2 limit = road.GetInitPos() + road.GetGimmickParameter(switchCount).GetLimit();
	// 軸単位で動かすかどうか
	Vec2 moveDir = {};
	if (road.GetGimmickParameter(switchCount).GetLimit().x > 0) { moveDir.x = +1.0f; }
	else if (road.GetGimmickParameter(switchCount).GetLimit().x < 0) { moveDir.x = -1.0f; }
	if (road.GetGimmickParameter(switchCount).GetLimit().y > 0) { moveDir.y = +1.0f; }
	else if (road.GetGimmickParameter(switchCount).GetLimit().y < 0) { moveDir.y = -1.0f; }

	road.pos += moveDir * road.GetGimmickParameter(switchCount).GetSpeed();

	if ((moveDir.x > 0 && (road.pos.x >= limit.x)) ||
		(moveDir.x < 0 && (road.pos.x <= limit.x)) ||
		(moveDir.y > 0 && (road.pos.y >= limit.y)) ||
		(moveDir.y < 0 && (road.pos.y <= limit.y)))
	{
		road.GetGimmickParameter(switchCount).ChangeFlag();
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

void Stage::EditerInit(const Vec2& playerSize)
{
	Init(playerSize);
	scale = 1;
	ROAD_SIZE = (PLAYER_SIZE / scale) + (ROAD_OFFSET / scale);

	boxes.clear();

	// スタートの追加
	boxes.emplace_back();
	boxes.back().type = Road::RoadType::START;
	boxes.back().pos = { ROAD_SIZE.x, ROAD_SIZE.y };
	boxes.back().size = { ROAD_SIZE.x, ROAD_SIZE.y };
	boxes.back().Init();

	// ゴールの追加
	boxes.emplace_back();
	boxes.back().type = Road::RoadType::GOAL;
	boxes.back().pos = { window_width - ROAD_SIZE.x, window_height - ROAD_SIZE.y };
	boxes.back().size = { ROAD_SIZE.x, ROAD_SIZE.y };
	boxes.back().Init();
}

void Stage::Create()
{
	boxes.emplace_back();
	boxes.back().type = Road::RoadType::ROAD;
	boxes.back().pos = Input::Get()->GetMousePos();
	boxes.back().size = { ROAD_SIZE.x, ROAD_SIZE.y };
	boxes.back().Init();
}

void Stage::Delete(size_t num)
{
	if (num < 0 || num >= boxes.size()) return;

	boxes.erase(boxes.begin() + num);
}

void Stage::EditerReset()
{
	boxes.clear();

	// スタートの追加
	boxes.emplace_back();
	boxes.back().type = Road::RoadType::START;
	boxes.back().pos = { ROAD_SIZE.x, ROAD_SIZE.y };
	boxes.back().size = { ROAD_SIZE.x, ROAD_SIZE.y };
	boxes.back().Init();

	// ゴールの追加
	boxes.emplace_back();
	boxes.back().type = Road::RoadType::GOAL;
	boxes.back().pos = { window_width - ROAD_SIZE.x, window_height - ROAD_SIZE.y };
	boxes.back().size = { ROAD_SIZE.x, ROAD_SIZE.y };
	boxes.back().Init();
}

void Stage::BringForefront(size_t num)
{
	for (size_t i = num; i < boxes.size() - 1; i++)
	{
		auto keep = boxes[i];
		boxes[i] = boxes[i + 1];
		boxes[i + 1] = keep;
	}
}

void Stage::SetScale(unsigned short scale)
{
	unsigned short nowScale = this->scale;
	this->scale = scale;

	ROAD_SIZE = (PLAYER_SIZE / static_cast<float>(scale)) + (ROAD_OFFSET / static_cast<float>(scale));
	float rate = static_cast<float>(nowScale) / static_cast<float>(scale);
	for (auto& i : boxes)
	{
		i.pos *= rate;
		i.size *= rate;
	}
}
