#include"TitleScene.h"
#include<sstream>
#include<iomanip>
#include"Camera.h"
#include"Shape.h"
#include"Input.h"
#include"Texture.h"
#include"SceneManager.h"
#include"GameScene.h"
TitleScene::TitleScene()
{}
TitleScene::~TitleScene()
{}

void TitleScene::Init()
{
	// ���C�g�O���[�v�N���X�쐬
	lightGroup = std::make_unique<LightGroup>();
	lightGroup->Initialize();
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,-1,0,0 });
	lightGroup->SetShadowDir(Vec3(0, 1, 0));

	//Shepe�Ő}�`��������x�ݒ�ł���B
	shape = Shape::CreateSquare(10.0f, 10.0f, 10.0f);
	//.obj�t�@�C���ǂݍ���
	objectData = Shape::CreateOBJ("sphere", false, "OBJ/");
	//�X�v���C�g�쐬�̎d��
	sprite = Sprite::Get()->SpriteCreate(L"Resources/gutitubo.png");

	//3D�e�N�X�`���`��
	one = Texture::Get()->LoadTexture(L"Resources/Mary.jpg");

	//�J�����ݒ�
	Camera::Get()->SetCamera(Vec3{ 0,0,-35 }, Vec3{ 0, 0, 0 }, Vec3{ 0, 1, 0 });
	FBXObject3d::SetLight(lightGroup.get());
	Object::SetLight(lightGroup.get());
}

void TitleScene::Update()
{
	//�V�[���̕ύX�̎d��
	if (Input::Get()->KeybordTrigger(DIK_SPACE))
	{
		BaseScene* scene = new GameScene();
		sceneManager_->SetNextScene(scene);
	}

	DebugText::Get()->Print(100.0f, 100.0f, 5, "Title");
	lightGroup->Update();
}

void TitleScene::Draw()
{
	Vec3 position = {};
	Vec3 scale = { 1.0f,1.0f,1.0f };
	Vec3 angle = {};

	/// <summary>
	/// 3D�`��
	/// shadowFlag true����Ȃ��ƕ`�悳��Ȃ���
	/// </summary>
	Object::Draw(objectData, psr, position, scale, angle,{},one,true);


	//2D�`��
	Vec2 position2D = { 200.0f,200.0f };
	float width = 140.0f, height = 100.0f;
	Sprite::Get()->Draw(sprite, position2D, width, height);

}

void TitleScene::ShadowDraw()
{


}

void TitleScene::Finalize()
{


}
