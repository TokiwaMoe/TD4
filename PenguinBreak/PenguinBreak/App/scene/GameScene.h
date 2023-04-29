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

	//�S�[�����̉��o�n���h�T�C��
	std::unique_ptr<Model> m_model;		  //�y���M�����f��
	std::unique_ptr<FBXObject3d>  m_fbx;//�y���M��FBX

	//2d
	SpriteData background;
	SpriteData palm_1;
	SpriteData palm_2;

	Player* player = nullptr;
	Stage* stage = nullptr;
};