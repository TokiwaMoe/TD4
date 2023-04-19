#pragma once

class SceneManager;
/// <summary>
/// シーンチェンジベース
/// </summary>
class BaseScene
{
public:
	virtual ~BaseScene() = default;
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;
	/// <summary>
	/// 影描画
	/// </summary>
	virtual void ShadowDraw() = 0;

	virtual void Finalize() = 0;

	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }
protected:
	SceneManager* sceneManager_ = nullptr;
};

