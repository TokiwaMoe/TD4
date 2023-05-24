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
private://��`

	SpriteData backGround[2];					//�w�i

	SpriteData stageData[10];	//�X�e�[�W�i���o�[
	Vec2 stagePos[10];	//���W

	SpriteData hand_p = {};
	SpriteData hand_g = {};
	bool isDraw = false;
};

