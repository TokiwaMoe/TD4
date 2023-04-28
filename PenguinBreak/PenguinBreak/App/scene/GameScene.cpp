#include "GameScene.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"FbxLoader.h"
#include"Shape.h"
#include"SceneManager.h"
#include"ResultScene.h"


GameScene::GameScene()
{}
GameScene::~GameScene()
{
}

void GameScene::Init()
{
	//���C�g�O���[�v�N���X�쐬
	lightGroup = std::make_unique<LightGroup>();
	lightGroup->Initialize();
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,-1,0,0 });
	lightGroup->SetShadowDir(Vec3(0, 1, 0));
	//���f�[�^�ǂݍ���
	//�J�����ʒu���Z�b�g
	Camera::Get()->SetCamera(Vec3{ 0,0,-200 }, Vec3{ 0, 0, 0 }, Vec3{ 0, 1, 0 });
	FBXObject3d::SetLight(lightGroup.get());
	Object::SetLight(lightGroup.get());

	//FBX�֘A
	Model* model1 = FbxLoader::GetInstance()->LoadModelFromFile("pengin", "FBX/");
	m_model = std::make_unique<Model>();
	m_model = std::unique_ptr<Model>(model1);
	m_fbx = std::make_unique<FBXObject3d>();
	m_fbx->Initialize();
	m_fbx->SetModel(m_model.get());
	m_fbx->SetScale(Vec3(0.025f, 0.025f, 0.025f));

	//�X�v���C�g�쐬�̎d��
	background = Sprite::Get()->SpriteCreate(L"Resources/background.png");

	//�v���C���[
	player = new Player();
	player->Initialize();
	player->Init();
	stage = Stage::GetInstance();
	stage->Init();
}

void GameScene::Update()
{
	//�V�[���̕ύX�̎d��
	if (Input::Get()->KeybordTrigger(DIK_SPACE))
	{
		BaseScene* scene = new ResultScene();
		sceneManager_->SetNextScene(scene);
	}

	//FBX�֘A
	m_fbx->SetRotation({30.0f,180.0f,0.0f});
	m_fbx->Update();
	if (Input::Get()->KeybordTrigger(DIK_UP))
	{
		m_fbx->PlayAnimation(true);
	}

	// �X�e�[�W�o�́i�f�o�b�O���s�p�j
	if (Input::Get()->KeybordTrigger(DIK_1))
	{
		stage->WriteStage("write_test");
	}

	player->Update();

	DebugText::Get()->Print(100.0f, 100.0f, 10, "Game");

	lightGroup->Update();
}

void GameScene::Draw()
{
	float width = 1280, height = 720;
	Sprite::Get()->Draw(background, { 0,0 }, width, height);
	m_fbx->Draw(true);
	stage->Draw();
	player->Draw();
}

void GameScene::ShadowDraw()
{
	
}


void GameScene::Finalize()
{

}

