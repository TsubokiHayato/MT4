#include <Novice.h>
#include<Vector3.h>
#include <Vector4.h>
#include"Matrix4x4.h"
const char kWindowTitle[] = "学籍番号";


// 任意軸回転行列を作成する関数
// 行ベクトルで計算
Matrix4x4 MakeRotateAxisAngleMatrix(Vector3& axis, float angle) {
	Matrix4x4 result;

	float cosAngle = cosf(angle);
	float sinAngle = sinf(angle);
	float oneMinusCos = 1.0f - cosAngle;

	axis = Normalize(axis);

	result.m[0][0] = cosAngle + axis.x * axis.x * oneMinusCos;
	result.m[0][1] = axis.x * axis.y * oneMinusCos + axis.z * sinAngle;
	result.m[0][2] = axis.x * axis.z * oneMinusCos - axis.y * sinAngle;
	result.m[0][3] = 0.0f;

	result.m[1][0] = axis.y * axis.x * oneMinusCos - axis.z * sinAngle;
	result.m[1][1] = cosAngle + axis.y * axis.y * oneMinusCos;
	result.m[1][2] = axis.y * axis.z * oneMinusCos + axis.x * sinAngle;
	result.m[1][3] = 0.0f;

	result.m[2][0] = axis.z * axis.x * oneMinusCos + axis.y * sinAngle;
	result.m[2][1] = axis.z * axis.y * oneMinusCos - axis.x * sinAngle;
	result.m[2][2] = cosAngle + axis.z * axis.z * oneMinusCos;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

static const int kRowHeight = 20;
static const int kRowWidth = 60;

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix) {

	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {

			Novice::ScreenPrintf(x + column * kRowWidth, y + row * kRowHeight, "%6.03f", matrix.m[row][column]);
		}
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};


	Vector3 axis = Normalize({1.0f, 1.0f, 1.0f});
	float angle = 0.44f;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		Matrix4x4 rotateAxisAngleMatrix = MakeRotateAxisAngleMatrix(axis, angle);

		MatrixScreenPrintf(10, 10, rotateAxisAngleMatrix);
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
