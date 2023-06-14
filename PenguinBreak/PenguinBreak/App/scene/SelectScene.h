#pragma once
#include"BaseScene.h"
#include<Sprite.h>
#include<Stage.h>
class SelectScene :public BaseScene
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SelectScene();
	/// <summary>
	/// �f�X�R���X�g���N�^
	/// </summary>
	~SelectScene();

	void Init()override;
	/// <summary>
	/// �X�V
	/// </summary>
	void Update()override;
	/// <summary>
	/// �`��
	/// </summary>
	void Draw()override;
	/// <summary>
	/// �e�`��
	/// </summary>
	void ShadowDraw()override;

	void Finalize()override;
	bool GetEffect()override;
private:
	//�X�e�[�W����
	void StageDecision();
	//�I�����傫�����ς�鏈��
	void DecisionScale();
	//�������X�v���C�g
	void Move();
private://��`

	SpriteData backGround[2];            //�w�i
	SpriteData frame;            //�w�i
	SpriteData stageData[10];            //�X�e�[�W�i���o�[
	Vec2 stagePos[10];                   //���W


	SpriteData backButton;               //�߂�{�^��
	Vec2 backButtonPos = { 60.0f,60.0f };//�߂�{�^���̈ʒu
	float scale = 0.8f;                  //�I�����̑傫�����o�p
	int scaleNumber = -1;               //�傫����ς��鐔��
	bool decScaleFlag = false;          //���������邩�傫�����邩

	SpriteData hand_p = {};
	SpriteData hand_g = {};
	bool isDraw = false;

	Vec2 rulePos = { 670,410 };
	SpriteData rule = {};
	bool isChange = false;
};

