#include<Windows.h>
#include"../PenguinBreak/App/scene/SceneManager.h"
const int window_width = 1280;//横幅
const int window_height = 720;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	SceneManager::Get()->Initialize();
	while (true)  // ゲームループ
	{
		//メッセージ処理
		if (Window::Get()->ProcessMessage()) { break; }

		SceneManager::Get()->Update();

		SceneManager::Get()->Draw();

	}
#pragma region	//データ解放
	SceneManager::Get()->Delete();

	//ゲームウィンドウの破棄
	Window::Get()->TerminateGameWindow();

	return 0;
}
