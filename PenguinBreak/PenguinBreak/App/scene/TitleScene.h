#pragma once
#include "DirectXBase.h"
#include"DebugText.h"
#include"Collision.h"
#include"Audio.h"
#include<PostEffect.h>
#include<memory>
#include"BaseScene.h"
extern const int window_width;
extern const int window_height;
/// <summary>
/// �^�C�g���V�[��
/// </summary>
class TitleScene :public BaseScene
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	TitleScene();
	/// <summary>
	/// �f�X�R���X�g���N�^
	/// </summary>
	~TitleScene();

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
	//����
	void StageDecision();
	//�I�����傫�����ς�鏈��
	void DecisionScale();
private://��`
	SpriteData background = {};//�w�i
	SpriteData titleGraph = {};
	SpriteData button = {};
	SpriteData explanation = {};
	const Vec2 bPos = { 630.0f, 580.0f };
	const Vec2 bSize = { 400.0f, 160.0f };
	float scale = 1.0f;
	bool isScale = false;
	bool decScaleFlag = false;          //���������邩�傫�����邩

	bool isDraw = false;
	SpriteData hand_p = {};
	SpriteData hand_g = {};
	Vec2 hPos = {};
};


