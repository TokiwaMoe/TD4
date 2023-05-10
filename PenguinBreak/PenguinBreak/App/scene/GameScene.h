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
	Vec2 SizeChange(Vec2 startSize, Vec2 lim, bool flag, float speed);
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
	Vec2 deerPos;
	Vec2 palmSize_1;
	Vec2 palmSize_2;
	bool isChange = true;

};