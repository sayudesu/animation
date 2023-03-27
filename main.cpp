#include "DxLib.h"

#include "game.h"

namespace player
{
	constexpr int imageNum = 2;
}

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//ウィンドウモード設定
	ChangeWindowMode(Game::kWindowMode);
	//ウィンドウ名設定
	SetMainWindowText(Game::kTitelText);
	//画面サイズ設定
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	//ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	int hPlayerIdle;
	int hPlayerWalk[player::imageNum];
	int playerImageLeft = 0, playerImageTop = 0;

	int playerImageFrameCount = 0;

	int player_X = 0, player_Y = 0;
	
	bool playerMovWalk = false;
	bool playerDirections = false;

	for (int i = 0; i < player::imageNum; i++)
	{
		hPlayerWalk[i] = 0;
	}

	hPlayerIdle = LoadGraph("Data/Idle1_L.png");

	hPlayerWalk[0] = LoadGraph("Data/Walk_Backpedal_L.png");
	hPlayerWalk[1] = LoadGraph("Data/Walk_Backpedal_R.png");

	while (ProcessMessage() == 0)
	{
		LONGLONG time = GetNowHiPerformanceCount();

		//画面をクリアする
		ClearDrawScreen();

	



		playerMovWalk = false;

		if (CheckHitKey(KEY_INPUT_A))
		{
			player_X -= 5;
			playerDirections = false;
			playerMovWalk = true;
		}
		else if (CheckHitKey(KEY_INPUT_D))
		{
			player_X += 5;
			playerDirections = true;
			playerMovWalk = true;
		}
		if (CheckHitKey(KEY_INPUT_W))
		{
			player_Y -= 5;
			playerMovWalk = true;
		}
		else if (CheckHitKey(KEY_INPUT_S))
		{
			player_Y += 5;
			playerMovWalk = true;
		}

		

		if (playerDirections ||!playerDirections)
		{
			playerImageFrameCount++;

			if (playerImageFrameCount == 3)
			{
				playerImageFrameCount = 0;

				if (playerImageLeft < 300 * 3)
				{
					playerImageLeft += 300;
				}
				else
				{
					playerImageLeft = 0;
					playerImageTop += 160;
					if (playerImageTop > 160 * 4)playerImageTop = 0;
				}
			}

		}

		if (playerMovWalk)
		{
			if (playerDirections)//右向き
			{
				DrawRectRotaGraph(player_X, player_Y,
					playerImageLeft, playerImageTop, 300, 160,
					1.5, DX_PI * 2, hPlayerWalk[1], true, false);
			}
			else
			{
				DrawRectRotaGraph(player_X, player_Y,
					playerImageLeft, playerImageTop, 300, 160,
					1.5, DX_PI * 2, hPlayerWalk[0], true, false);
			}

		}
		else
		{
			DrawRectRotaGraph(player_X, player_Y,
				playerImageLeft, playerImageTop, 300, 160,
				1.5, DX_PI * 2, hPlayerIdle, true, playerDirections);
		}



		//裏画面を表画面を入れ替える
		ScreenFlip();
		//escを押して終了
		if (CheckHitKey(KEY_INPUT_ESCAPE)) break;

		//60fps固定
		while (GetNowHiPerformanceCount() - time < 16667)
		{

		}
	}

	DxLib_End();			// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}