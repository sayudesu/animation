#include "DxLib.h"

#include "game.h"

namespace player
{
	constexpr int imageNum = 2;
}

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//�E�B���h�E���[�h�ݒ�
	ChangeWindowMode(Game::kWindowMode);
	//�E�B���h�E���ݒ�
	SetMainWindowText(Game::kTitelText);
	//��ʃT�C�Y�ݒ�
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	//�_�u���o�b�t�@���[�h
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

		//��ʂ��N���A����
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
			if (playerDirections)//�E����
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



		//����ʂ�\��ʂ����ւ���
		ScreenFlip();
		//esc�������ďI��
		if (CheckHitKey(KEY_INPUT_ESCAPE)) break;

		//60fps�Œ�
		while (GetNowHiPerformanceCount() - time < 16667)
		{

		}
	}

	DxLib_End();			// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}