#pragma once
#include <DirectXMath.h>
#include "DirectXBase.h"
#include"DebugText.h"
#include"Collision.h"
#include"Audio.h"
#include"FBXObject3d.h"
#include"Object.h"
#include"Sprite.h"
#include"Stage.h"
#include<memory>

class Player {
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	void Initialize();

	void Init();

	void Update(Stage *stage);

	void Move();//�}�E�X

	void collide2Stage(Stage *stage);

	bool OutStage(Vec2 position, Stage *stage, int num);

	void Draw();

	//getter
	//���W
	const Vec2& GetPosition() { return position; }
	//��]
	const Vec2& GetRotation() { return rotation; }
	//setter

public:
	enum Direction
	{
		Left,
		Right
	};
	//2d
	SpriteData player;
	// X,Y,Z�����̃��[�J����]�p
	Vec2 rotation = { 0,0 };
	// �v���C���[���W
	Vec2 position = { 50,50 };
	int  direction = 0;
	//�X�v���C�g�T�C�Y
	float width = 64.0f, height = 128.0f;
	//���a
	Vec2 radius = { 32, 64 };
	//�}�E�X�̔���p
	Sphere circle;
	Ray ray;
};