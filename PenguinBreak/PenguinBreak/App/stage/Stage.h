#pragma once
#include "Sprite.h"
#include <vector>
#include "./../../Library/Helper/LoadJson.h"

class Stage final
{
private: //�V���O���g����
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

public: //�T�u�N���X
	// ���̃^�C�v
	enum RoadType
	{
		ROAD,  //��
		START, //�X�^�[�g
		GOAL,  //�S�[��
	};

	// �M�~�b�N�̃^�C�v
	enum Gimmick
	{
		NO_GIMMICK, //�M�~�b�N����
		SCALE,      //�������܂�M�~�b�N
	};

	// �M�~�b�N�𓮂����ׂɎg���l
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

	class Road
	{
	private:
		SpriteData sprite;
		RoadType type;
		Gimmick gimmick;
		GimmickParameter parameter;

		Vec2 initPos;
		Vec2 initSize;
	public:
		Vec2 pos;
		Vec2 size;

	public:
		Road();
		Road(const Vec2& pos, const Vec2& size);
		void BoxInit();
		void ParameterInit(const GimmickParameter& parameter) { this->parameter = parameter; }

		void SetType(const RoadType& type) { this->type = type; }
		void SetGimmick(const Gimmick& gimmick) { this->gimmick = gimmick; }

		SpriteData& GetSprite() { return sprite; }
		const RoadType GetType() const { return type; }
		const Gimmick GetGimmick() const { return gimmick; }
		GimmickParameter& GetGimmickParameter() { return parameter; }
		const Vec2 GetInitPos() { return initPos; }
		const Vec2 GetInitSize() { return initSize; }
	};

	struct JsonData
	{
		std::vector<Road> objects = {};
	};

private: //�����o�ϐ�
	std::vector<Road> boxes;
	size_t startIndex;
	size_t goalIndex;

public: //�����o�֐�
	// ������
	void Init();
	// �M�~�b�N�̍X�V
	void GimmickUpdate();
	// �`��
	void Draw(float offsetX = 0.0f, float offsetY = 0.0f);

	// �X�e�[�W�̏�������
	void WriteStage(const std::string& stageName);
	// �X�e�[�W�̓ǂݍ���
	JsonData* LoadStage(const std::string& jsonFile);

	Road GetBox(int num) { return boxes[num]; }
	size_t GetBoxSize() { return boxes.size(); }
	// ���W
	Vec2 GetPos(int num) { return boxes[num].pos; }
	//�X�v���C�g�T�C�Y
	Vec2 GetSize(int num) { return boxes[num].size; }
	Vec2 GetStartPos() { return boxes[startIndex].pos; }
	size_t GetStart() { return startIndex; }
	Vec2 GetGoalPos() { return boxes[goalIndex].pos; }
	size_t GetGoal() { return goalIndex; }
private:
	// �������܂�M�~�b�N
	void ScaleGimmick(Road& road);
};
