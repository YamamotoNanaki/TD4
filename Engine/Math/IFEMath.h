#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include "Float2.h"
#include "Float3.h"
#include "Float4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix.h"
#include "Quaternion.h"
#include <stdint.h>

namespace IFE
{
	constexpr double PI = 3.14159265358979;

	struct Int2
	{
		int32_t x;
		int32_t y;
		int32_t operator=(const int32_t& i);
	};

	Float4 SetFloat4ToFloat3(const Float3& f, float z = 0);
	Float4 SetFloat4ToFoat2(const Float2& f, float z = 0, float w = 0);
	Float3 SetFloat3ToFloat2(const Float2& f, float z = 0);
	Float4 SetFloat4ToVector3(const Vector3& v, float w = 0);
	Float3 SetFloat3(const Vector3& v);
	Float2 SetFloat3ToVector2(const Vector2& v);
	Vector3 SetVector3(const Float3& f);
	Vector3 SetVector3ToFloat2(const Float2& f, float z = 0);
	Vector2 SetVector2(const Float2& f);


	// 零ベクトルを返す
	const Vector3 Vector3Zero();
	// 2ベクトルが一致しているか調べる
	bool Vector3Equal(const Vector3& v1, const Vector3& v2);
	// 長さを求める
	float Vector3Length(const Vector3& v);
	// 正規化する
	//Vector3& Vector3Normalize(Vector3& v);
	Vector3 Vector3Normalize(const Vector3& v);
	// 内積を求める
	float Vector3Dot(const Vector3& v1, const Vector3& v2);
	// 外積を求める
	Vector3 Vector3Cross(const Vector3& v1, const Vector3& v2);

	// ビュー行列の作成
	Matrix MatrixLookAtLH(const Vector3& eye, const Vector3& target, const Vector3& up);
	// 並行投影行列の作成
	Matrix MatrixOrthographic(
		float viewLeft, float viewRight, float viewBottom, float viewTop, float nearZ, float farZ);
	// 透視投影行列の作成
	Matrix MatrixPerspective(float fovAngleY, float aspectRatio, float nearZ, float farZ);

	// 座標変換（w除算なし）
	Vector3 Vector3Transform(const Vector3& v, const Matrix& m);
	// 座標変換（w除算あり）
	Vector3 Vector3TransformCoord(const Vector3& v, const Matrix& m);
	// ベクトル変換
	Vector3 Vector3TransformNormal(const Vector3& v, const Matrix& m);

	Vector3 VectorNegate(const Vector3& m);

	Vector3 VectorLerp(const Vector3& v0, const Vector3& v1, float t);

	Matrix MatrixPerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ);
	bool NearEqual(float S1, float S2, float epsilon);
	bool NearEqual(const Float2& f1, const Float2& f2, float epsilon);
	bool NearEqual(const Float3& f1, const Float3& f2, float epsilon);
	void ScalarSinCos(float* pSin, float* pCos, float  Value);
	float ConvertToRadians(float fDegrees);
	float ConvertToDegrees(float fRadians);
	Float3 ConvertToRadians(const Float3& fDegrees);
	Float3 ConvertToDegrees(const Float3& fRadians);
	Float3 operator+(const Float3& f, const Vector2& v);

	float Average(Vector2 v);
	float Average(Vector3 v);

	/// <summary>
	/// 補完
	/// </summary>
	/// <param name="x1">移動したい変数</param>
	/// <param name="x2">移動したい距離</param>
	/// <param name="flame">時間</param>
	/// <returns>補完後の値</returns>
	float Complement(float& x1, float& x2, float flame);

	/// <summary>
	/// 単振動をした値を返す
	/// </summary>
	/// <param name="time">経過時間</param>
	/// <param name="amplitude">振幅</param>
	/// <param name="period">周期</param>
	/// <returns></returns>
	float SimpleHarmonicMotion(float time, float amplitude = 10.0f, float period = 60.0f);

	/// <summary>
	/// 円運動をした値を返す(複素数版)
	/// </summary>
	/// <param name="CenterPoint">中心点</param>
	/// <param name="radius">中心点からの半径</param>
	/// <param name="angle">角度</param>
	/// <returns></returns>
	Vector2 CircularMotion(Vector2 CenterPoint, float radius, float angle);

	float Vector3Max(const Vector3& vec);
	float Vector3Min(const Vector3& vec);

	//元の行列をtimer0のとき0倍、timer1のとき1倍の値にする関数
	Matrix Lerp(const Matrix& mat, float timer,float maxTime);
	//timer0のときmat0、timer1のときmat1の値にする関数
	Matrix Lerp(const Matrix& mat0,const Matrix& mat1, float timer,float maxTime);
};