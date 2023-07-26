#pragma once
#include <Sprite.h>
/// <summary>
/// リザルト演出
/// </summary>
class ResultStating
{
public:
	enum SELECT
	{
		ZERO,
		BACK,
		NEXT,
	};
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ResultStating();
	/// <summary>
    /// デスコンストラクタ
    /// </summary>
	~ResultStating();
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update(const int nextStage);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const int nextStage);

	int NextSceneStatus() { return nextSceneStatus; }
private:
	//ステージ決定
	void StageDecision(const int nextStage);
	//選択時大きさが変わる処理
	void DecisionScale();
	//クリア文字動かす
	void ClearMove();
	//馬人間の動き
	void DoubleHMove();
	//ての回転
	void HandRotation();
private:
	SpriteData backGround;                      //背景
	SpriteData selectGraph;                     //もどる画像
	const Vec2 selectPos = { 461.0f,440.0f };   //位置
	SpriteData nextStageGraph;                  //つぎへ画像
	const Vec2 nextStagePos = { 800.0f,440.0f };//位置
	const Vec2 selectSize = { 223.0f, 114.0f }; //大きさ
	//セレクト
	int selectStatus = NEXT;

	SpriteData hand_p = {};
	SpriteData hand_g = {};
	bool isDraw = false;
	float scale = 0.8f;             //選択時の大きさ演出用
	int scaleNumber = -1;           //大きさを変える数字
	bool decScaleFlag = false;      //小さくするか大きくするか

	//クリア文字関連
	SpriteData clearGraph[4];       //クリアー文字画像
	float clearMovePos[4];          //クリア移動量
	bool clearMoveFlag[4];          //上下どっちか

	SpriteData houseHumanGraph;     //馬の画像
	Vec2 houseHumanPos = { 0.0f,317.0f };//馬の画像の位置

	int nextSceneStatus = ZERO;

	//手
	static const int HAND_MAX_X = 4;
	static const int HAND_MAX_Y = 2;
	SpriteData hand[HAND_MAX_X][HAND_MAX_Y];

	//額
	SpriteData frame;
};

