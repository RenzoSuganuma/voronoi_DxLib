#include "math.h"
#include "chrono"
#include "DxLib.h"
#include "Windows.h"

#pragma comment (lib , "winmm.lib")

const int WIDTH = 500, HEIGHT = 500;

// �����̍���
int p = 1;
// �œ_ �� X����
int xf = WIDTH / 2;
// �œ_ �� Y����
int  yf = HEIGHT - 100;

// https://www.mathartroom.com/processing/voronoi_diagram/
// �{���m�C�}��`�ʂ���

void draw_background() {
	for (size_t i = 0; i < HEIGHT; i++)
	{
		for (size_t k = 0; k < WIDTH; k++)
		{
			DrawPixel(k, i, GetColor(128, 128, 128));
		}
	}
}

void draw_function() {
	// �s�N�Z���T�C�Y
	int pixelSize = 5;
	pixelSize /= 2;

	// �œ_��`��
	for (int y = -pixelSize; y < pixelSize; y++)
	{
		for (int x = -pixelSize; x < pixelSize; x++)
		{
			DrawPixel(xf + x, yf + y, 0);
		}
	}

	// ������
	for (int x = -WIDTH; x < WIDTH; x++)
	{
		int y = -(pow(x - xf, 2) / (2 * (p - yf)))
			+ (p + yf) / 2;

		for (int i = -pixelSize; i < pixelSize; i++)
		{
			for (int j = -pixelSize; j < pixelSize; j++)
			{
				DrawPixel(x + j, y + i, GetColor(255, 0, 0));
			}
		}
	}
}

void draw_standard_line() {
	p++;
	if (p > HEIGHT) p = 0;

	// �s�N�Z���T�C�Y
	int pixelSize = 5;
	pixelSize /= 2;

	for (int y = -pixelSize; y < pixelSize; y++)
	{
		for (size_t x = 0; x < WIDTH; x++)
		{
			DrawPixel(x, p + y, -1);
		}
	}

}

// �G���g���[�|�C���g���
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �t���[�����[�g�����̂��߂̕ϐ��Q
	// �P�t���[��������̎���[ms]
	float frameTime = 0;
	// �ߋ�����
	std::chrono::system_clock::time_point clockStarted;
	// ���ݎ���
	std::chrono::system_clock::time_point clockEnded;
	// �b
	float deltaTime = frameTime / 1000.0f;
	// �o�ߎ���
	float elapsedTime = 0;

	// DXLib�̏�����
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	ClearDrawScreen();

	ChangeWindowMode(true);
	SetGraphMode(WIDTH, HEIGHT, 32);

	while (1)
	{
		if (ProcessMessage() == -1)
		{
			break;
		}

		// �v���J�n
		clockStarted = std::chrono::system_clock::now();

		ClearDrawScreen();

		elapsedTime += deltaTime;

		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			DxLib_End();

			break;
		}

		draw_background();
		draw_function();
		draw_standard_line();

		ScreenFlip();

		clockEnded = std::chrono::system_clock::now();
		double deltaMilSec = (static_cast<double>
			(std::chrono::duration_cast<std::chrono::microseconds>
				(clockEnded - clockStarted).count()));
		deltaTime = deltaMilSec / 1000.0 / 1000.0;
		if (frameTime > deltaMilSec)
		{
			timeBeginPeriod(1);
			Sleep((DWORD)(frameTime - deltaMilSec));
			timeEndPeriod(1);
		}
	}

	return 0;

}

