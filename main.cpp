#include "math.h"
#include "chrono"
#include "DxLib.h"
#include "Windows.h"

#pragma comment (lib , "winmm.lib")

const int WIDTH = 500, HEIGHT = 500;

// 準線の高さ
int p = 1;
// 焦点 の X成分
int xf = WIDTH / 2;
// 焦点 の Y成分
int  yf = HEIGHT - 100;

// https://www.mathartroom.com/processing/voronoi_diagram/
// ボロノイ図を描写する

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
	// ピクセルサイズ
	int pixelSize = 5;
	pixelSize /= 2;

	// 焦点を描写
	for (int y = -pixelSize; y < pixelSize; y++)
	{
		for (int x = -pixelSize; x < pixelSize; x++)
		{
			DrawPixel(xf + x, yf + y, 0);
		}
	}

	// 放物線
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

	// ピクセルサイズ
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

// エントリーポイントを提供
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// フレームレート制限のための変数群
	// １フレーム当たりの時間[ms]
	float frameTime = 0;
	// 過去時間
	std::chrono::system_clock::time_point clockStarted;
	// 現在時間
	std::chrono::system_clock::time_point clockEnded;
	// δ秒
	float deltaTime = frameTime / 1000.0f;
	// 経過時間
	float elapsedTime = 0;

	// DXLibの初期化
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

		// 計測開始
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

