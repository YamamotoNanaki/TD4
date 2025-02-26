#pragma once
#include "Vector3.h"
#include "Float4.h"

namespace IFE
{
	struct Matrix
	{
		float m[4][4];

	public:
		Matrix();
		Matrix(float _1_1, float _1_2, float _1_3, float _1_4,
			float _2_1, float _2_2, float _2_3, float _2_4,
			float _3_1, float _3_2, float _3_3, float _3_4,
			float _4_1, float _4_2, float _4_3, float _4_4);
		Matrix operator+()const;
		Matrix operator-()const;
		Matrix operator*(float s)const;
		Matrix operator/(float s)const;
		Matrix& operator+=(const Matrix& m);
		Matrix& operator-=(const Matrix& m);
		Matrix& operator*=(const Matrix& m);
		Matrix& operator*=(float s);
		Matrix& operator/=(float s);
		Float4 operator[](size_t row);
		const Float4 operator[](size_t row) const;

		void SetX(float x, float y, float z, float w);
		void SetY(float x, float y, float z, float w);
		void SetZ(float x, float y, float z, float w);
		void SetW(float x, float y, float z, float w);
		void SetX(Vector3 v, float w = 0);
		void SetY(Vector3 v, float w = 0);
		void SetZ(Vector3 v, float w = 0);
		void SetW(Vector3 v, float w = 1);

		Matrix Inverse();
		//座標変換（ベクトルと行列の掛け算をする）
		static Vector3 Transform(const Vector3 v, const Matrix& m);
	};

	void GetArrayMatrix(const Matrix& matrix, float arrayPtr[]);
	void SetArrayMatrix(Matrix& matrix, float arrayPtr[]);

	// 単位行列を求める
	Matrix MatrixIdentity();
	// 逆行列を求める
	Matrix MatrixInverse(const Matrix& m);
	// 転置行列を求める
	Matrix MatrixTranspose(const Matrix& m);

	// 拡大縮小行列の作成
	Matrix MatrixScaling(float sx, float sy, float sz);

	// 回転行列の作成
	Matrix MatrixRotationX(float angle);
	Matrix MatrixRotationY(float angle);
	Matrix MatrixRotationZ(float angle);
	Matrix MatrixRotation(float x, float y, float z, float angle);

	// 平行移動行列の作成
	Matrix MatrixTranslation(float tx, float ty, float tz);

	Matrix MatrixOrthoGraphicProjection(float left, float right, float top, float bottom, float near, float far);

	Matrix operator+(const Matrix& m1, const Matrix& m2);
	Matrix operator-(const Matrix& m1, const Matrix& m2);
	Matrix operator*(const Matrix& m1, const Matrix& m2);

	Float3 GetScale(const Matrix& m);
};
