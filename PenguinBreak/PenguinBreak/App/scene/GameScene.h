#pragma once
#include "DirectXBase.h"
#include"DebugText.h"
#include"ParticleManager.h"
#include<memory>
#include"BaseScene.h"
#include<LightGroup.h>
#include"FBXObject3d.h"
#include"Player.h"
#include"../stage/Stage.h"
extern const int window_width;
extern const int window_height;

/// <summary>
/// �Q�[���V�[��
/// </summary>
class GameScene:public BaseScene
{
private:
	//�T�C�Y����W�ύX�p
	struct Change
	{
		Vec2 initPos;
		Vec2 pos;
		Vec2 lim;
		bool flag;
		float speed;
	};
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	GameScene();
	/// <summary>
	/// �f�X�R���X�g���N�^
	/// </summary>
	~GameScene();
	/// <summary>
	/// ������
	/// </summary>
	void Init()override;
	/// <summary>
	/// �X�V
	/// </summary>
	void Update()override;
	/// <summary>
	/// �X�V
	/// </summary>
	void SizeChange(Change change);
	/// <summary>
	/// �`��
	/// </summary>
	void Draw()override;
	/// <summary>
	/// �e�`��
	/// </summary>
	void ShadowDraw()override;

	void Finalize()override;
private:
	std::unique_ptr<LightGroup>lightGroup;
private://��`
	//2d
	SpriteData background;
	SpriteData palm_1;
	SpriteData palm_2;
	SpriteData deer;

	Player* player = nullptr;
	Stage* stage = nullptr;
	int stageNumber = 1;

	//
	Vec2 size = { 305,437 };
	Change palmSize_1;
	Change palmSize_2;
	Change deerPos;
	bool isChange = true;

};