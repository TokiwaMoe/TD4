#include <Object.h>
#include<memory>
#include"FBXObject3d.h"
#include<Audio.h>
#include"BaseScene.h"
#include<Sprite.h>
#include"../ResultStating/ResultStating.h"
/// <summary>
/// リザルトシーン
/// </summary>
class ResultScene:public BaseScene
{
private:

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ResultScene();
	/// <summary>
	/// デスコンストラクタ
	/// </summary>
	~ResultScene();
	/// <summary>
	/// ループ初期化
	/// </summary>
	void Init()override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="stageNum">ステージ</param>
	void Draw()override;
	/// <summary>
	/// 影描画
	/// </summary>
	void ShadowDraw()override;
	void Finalize()override;
	bool GetEffect()override;
private:
	Audio* audio = Audio::Get();
	SoundData sound = {};
	//リザルト演出
	ResultStating resultStating;
};