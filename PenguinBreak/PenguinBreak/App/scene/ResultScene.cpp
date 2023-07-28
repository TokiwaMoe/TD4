#include "ResultScene.h"
#include"DebugText.h"
#include<Input.h>
#include"SceneManager.h"
#include <PostEffect.h>
#include"SelectScene.h"
#include"GameScene.h"
ResultScene::ResultScene()
{}
ResultScene::~ResultScene()
{
	audio->SoundUnload(&sound);
}
void ResultScene::Init()
{	
	PostEffect::Get()->SetPipeline(static_cast<int>(PostEffectType::NORMAL));
	resultStating.Init();
	sound = Audio::SoundLoadWave("Resources/Sound/retrogamecenter.wav");
	audio->SoundBGMPlayLoopWave(sound, audio->BGM);
}

void ResultScene::Update()
{
	
	resultStating.Update(nextStage);

	//�X�e�[�W�Z���N�g��
	if (resultStating.NextSceneStatus() == ResultStating::BACK) {
		BaseScene* scene = new SelectScene();
		sceneManager_->SetNextScene(scene);
		audio->SoundStop(sound, audio->BGM);
	}
	//���̃X�e�[�W��
	if (resultStating.NextSceneStatus() == ResultStating::NEXT) {
		BaseScene* scene = new GameScene();
		scene->nextStage = nextStage;
		sceneManager_->SetNextScene(scene);
		audio->SoundStop(sound, audio->BGM);
	}

	//�J�[�\���\��
	Input::Get()->SetCursor(false);
}

void ResultScene::Draw()
{
	resultStating.Draw(nextStage);
}

void ResultScene::ShadowDraw()
{}

void ResultScene::Finalize()
{
	Texture::Get()->Delete();
	Sprite::Get()->Delete();
}

bool ResultScene::GetEffect()
{
	return false;
}
