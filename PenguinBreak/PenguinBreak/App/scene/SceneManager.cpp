#include "SceneManager.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"Shape.h"
#include"PostEffect.h"
#include"GameScene.h"
#include"TitleScene.h"
#include"ResultScene.h"
SceneManager::SceneManager()
{}
SceneManager::~SceneManager()
{
	scene_->Finalize();
	delete scene_;
}
void SceneManager::Initialize()
{
	//�ėp�@�\
	//�Q�[���E�B���h�E�̍쐬
	Window::Get()->CreateGameWindow();
	// DirectX����������
	DirectXBase::Get()->Initilize();
	//���͂̏�����
	Input::Get()->Initialize();
	//�V�F�[�_�[�̓ǂݍ���
	ShaderManager::LoadShaders();
	//���C�g�ÓI������
	LightGroup::StaticInitialize(DirectXBase::Get()->GetDevice());
	//�X�v���C�g�ÓI������
	Sprite::StaticInit(DirectXBase::Get()->GetDevice(), DirectXBase::Get()->GetCmandList());
	//�e�L�X�g�N���X������
	Texture::Get()->Init(DirectXBase::Get()->GetDevice());
	Pipeline::CreatePipeline(DirectXBase::Get()->GetDevice());
	//�f�o�b�N�e�L�X�g������
	DebugText::Get()->Initialize();
	//�X�v���C�g�N���X�쐬
	Sprite::Get()->Init();
	//FBX������
	FbxLoader::GetInstance()->Initialize(DirectXBase::Get()->GetDevice());
	FBXObject3d::SetDevice(DirectXBase::Get()->GetDevice());
	FBXObject3d::SetCmdList(DirectXBase::Get()->GetCmandList());
	FBXObject3d::CreateGraphicsPipeline();
	FBXObject3d::CreateShadowPipeline();
	//�}�`���f��������
	Shape::Init(DirectXBase::Get()->GetDevice());
	//�p�[�e�B�N��������
	ParticleManager::StaticInitialize(DirectXBase::Get()->GetDevice(), DirectXBase::Get()->GetCmandList(), window_width, window_height);
	//3D�I�u�W�F�N�g������
	Object::Init(DirectXBase::Get()->GetDevice(), DirectXBase::Get()->GetCmandList());
	PostEffect::Get()->Initialize(DirectXBase::Get()->GetDevice());
	//���쐬
	Audio::Get()->Init();
	//�V�[�����^�C�g���ɐݒ�
	BaseScene* scene = new TitleScene();
	SetNextScene(scene);
}

void SceneManager::Update()
{
	Input::Get()->Update();
	time += 0.01f;
	//���̃V�[���̗\�񂪂���Ȃ�
	if (nextScene_)
	{
		if (scene_)
		{
			scene_->Finalize();
			delete scene_;
		}
		//�V�[���؂�ւ�
		scene_ = nextScene_;
		nextScene_ = nullptr;

		scene_->SetSceneManager(this);
		//���̃V�[��������������
		scene_->Init();
	}

	//�X�V
	scene_->Update();
}

void SceneManager::Draw()
{
	PostEffect::Get()->PreDrawScene(DirectXBase::Get()->GetCmandList());
	Object::InitDraw(), Sprite::Get()->PreDraw();
	//�J�����ڐ��̕`��
	scene_->Draw();
	DebugText::Get()->DrawAll();

	PostEffect::Get()->PostDrawScene(DirectXBase::Get()->GetCmandList());
	DirectXBase::Get()->PreDraw();
	////�|�X�g�G�t�F�N�g�̕`��
	PostEffect::Get()->Draw(DirectXBase::Get()->GetCmandList(), Vec4{ 1.0f,1.0f,1.0f,1.0f }, time,scene_->GetEffect());
	//UI�`��
	DirectXBase::Get()->ResourceBarrier();
}

void SceneManager::Delete()
{
	Object::Delete();
	FbxLoader::GetInstance()->Finalize();
}