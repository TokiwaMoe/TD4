#pragma once
#include "ShadowMap.h"
#include"BaseScene.h"
#include"Audio.h"
#include<Sprite.h>
extern const int window_width;
extern const int window_height;

class SceneManager :public Singleton<SceneManager>
{
private:
	friend Singleton<SceneManager>;

	enum BlockOutType
	{
		Stand_By,
		First_Half,
		Latter_Half
	};
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SceneManager();
	/// <summary>
	/// �f�X�R���X�g���N�^
	/// </summary>
	~SceneManager();
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();
	/// <summary>
	/// �폜
	/// </summary>
	void Delete();

	//���̃V�[���\��
	void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }
	//�V�[���`�F���W���̉��o
	void BlackOut();
	//�V�[���`�F���W���̉��o
	void BlackOutDraw();
private://��`
	//���̃V�[��
	BaseScene* scene_ = nullptr;
	//���̃V�[��
	BaseScene* nextScene_ = nullptr;

	ShadowMap  m_shadowMapFar;

	float time = 0;


	SpriteData blackOut;//�Ó]�p�摜
	Vec3 blackPos[2] = {};
	float blackOutTime = 0.0f;
	int blackOutFlag = Stand_By;
};