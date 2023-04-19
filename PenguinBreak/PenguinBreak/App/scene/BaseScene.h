#pragma once

class SceneManager;
/// <summary>
/// �V�[���`�F���W�x�[�X
/// </summary>
class BaseScene
{
public:
	virtual ~BaseScene() = default;
	/// <summary>
	/// ������
	/// </summary>
	virtual void Init() = 0;
	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw() = 0;
	/// <summary>
	/// �e�`��
	/// </summary>
	virtual void ShadowDraw() = 0;

	virtual void Finalize() = 0;

	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }
protected:
	SceneManager* sceneManager_ = nullptr;
};

