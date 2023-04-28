#include "Stage.h"
#include <fstream>
#include <json.hpp>

Stage::Road::Road() :
	sprite{},
	pos{},
	size{},
	type{}
{
}

Stage::Road::Road(const Vec2& pos, const Vec2& size) :
	sprite{},
	pos(pos),
	size(size),
	type(RoadType::ROAD)
{
}

void Stage::Road::BoxInit()
{
	this->sprite = Sprite::Get()->SpriteCreate(L"Resources/white1x1.png");
}

Stage::Stage() :
	boxes{}
{
}

Stage::~Stage()
{
}

void Stage::Init()
{
	const float ROAD_OFFSET = 30.0f; //道の余白
	const float ROAD_WIDTH = 128.0f + ROAD_OFFSET; //プレイヤーのサイズのでかい方の値を採用（今回は横64:縦128なので縦の値）

	auto file = LoadStage("stage1");
	boxes.clear();
	boxes = file->objects;
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

		Sprite::Get()->Draw(i.sprite, i.pos + Vec2(offsetX, offsetY), i.size.x, i.size.y, Vec2(), color);
	}
}

Stage::JsonData* Stage::LoadStage(const std::string& jsonFile)
{
	//連結してフルパスを得る
	const std::string fullpath = std::string("Resources/levels/") + jsonFile + ".json";

	//ファイルストリーム
	std::ifstream file;

	//ファイルを開く
	file.open(fullpath);
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	//JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	//解凍
	file >> deserialized;

	//正しいレベルファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//レベルデータ格納用インスタンスを生成
	JsonData* levelData = new JsonData();

	//"objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"])
	{
		assert(object.contains("type"));

		//要素を追加
		levelData->objects.emplace_back(Road{});
		auto& objectData = levelData->objects.back();

		objectData.BoxInit();
		objectData.type = object["type"];
		objectData.pos.x = object["pos"][0];
		objectData.pos.y = object["pos"][1];
		objectData.size.x = object["size"][0];
		objectData.size.y = object["size"][1];
	}

	return levelData;
}

void Stage::WriteStage(const std::string& stageName)
{
	using namespace std;
	using json = nlohmann::json;

	//連結してフルパスを得る
	const string fullpath = string("Resources/levels/") + stageName + ".json";

	//ファイルストリーム
	ofstream file;

	//ファイルを開く
	file.open(fullpath);
	//ファイルオープン失敗をチェック
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
			{"type", boxes[i].type},
			{"pos", {boxes[i].pos.x, boxes[i].pos.y}},
			{"size", {boxes[i].size.x, boxes[i].size.y}}};
	}

	file << std::setw(4) << data << endl;
	file.close();
}
