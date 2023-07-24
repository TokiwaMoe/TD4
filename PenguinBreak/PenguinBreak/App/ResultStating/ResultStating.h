#pragma once
#include <Sprite.h>
/// <summary>
/// ���U���g���o
/// </summary>
class ResultStating
{
public:
	enum SELECT
	{
		ZERO,
		BACK,
		NEXT,
	};
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ResultStating();
	/// <summary>
    /// �f�X�R���X�g���N�^
    /// </summary>
	~ResultStating();
	/// <summary>
	/// ������
	/// </summary>
	void Init();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update(const int nextStage);
	/// <summary>
	/// �`��
	/// </summary>
	void Draw(const int nextStage);

	int NextSceneStatus() { return nextSceneStatus; }
private:
	//�X�e�[�W����
	void StageDecision(const int nextStage);
	//�I�����傫�����ς�鏈��
	void DecisionScale();
	//�N���A����������
	void ClearMove();
	//�n�l�Ԃ̓���
	void DoubleHMove();
	//�Ẳ�]
	void HandRotation();
private:
	SpriteData backGround;                      //�w�i
	SpriteData selectGraph;                     //���ǂ�摜
	const Vec2 selectPos = { 461.0f,440.0f };   //�ʒu
	SpriteData nextStageGraph;                  //���։摜
	const Vec2 nextStagePos = { 800.0f,440.0f };//�ʒu
	const Vec2 selectSize = { 223.0f, 114.0f }; //�傫��
	//�Z���N�g
	int selectStatus = NEXT;

	SpriteData hand_p = {};
	SpriteData hand_g = {};
	bool isDraw = false;
	float scale = 0.8f;             //�I�����̑傫�����o�p
	int scaleNumber = -1;           //�傫����ς��鐔��
	bool decScaleFlag = false;      //���������邩�傫�����邩

	//�N���A�����֘A
	SpriteData clearGraph[4];       //�N���A�[�����摜
	float clearMovePos[4];          //�N���A�ړ���
	bool clearMoveFlag[4];          //�㉺�ǂ�����

	SpriteData houseHumanGraph;     //�n�̉摜
	Vec2 houseHumanPos = { 0.0f,317.0f };//�n�̉摜�̈ʒu

	int nextSceneStatus = ZERO;

	//��
	static const int HAND_MAX_X = 4;
	static const int HAND_MAX_Y = 2;
	SpriteData hand[HAND_MAX_X][HAND_MAX_Y];

	//�z
	SpriteData frame;
};

