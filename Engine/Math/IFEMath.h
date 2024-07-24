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


	// ��x�N�g����Ԃ�
	const Vector3 Vector3Zero();
	// 2�x�N�g������v���Ă��邩���ׂ�
	bool Vector3Equal(const Vector3& v1, const Vector3& v2);
	// ���������߂�
	float Vector3Length(const Vector3& v);
	// ���K������
	//Vector3& Vector3Normalize(Vector3& v);
	Vector3 Vector3Normalize(const Vector3& v);
	// ���ς����߂�
	float Vector3Dot(const Vector3& v1, const Vector3& v2);
	// �O�ς����߂�
	Vector3 Vector3Cross(const Vector3& v1, const Vector3& v2);

	// �r���[�s��̍쐬
	Matrix MatrixLookAtLH(const Vector3& eye, const Vector3& target, const Vector3& up);
	// ���s���e�s��̍쐬
	Matrix MatrixOrthographic(
		float viewLeft, float viewRight, float viewBottom, float viewTop, float nearZ, float farZ);
	// �������e�s��̍쐬
	Matrix MatrixPerspective(float fovAngleY, float aspectRatio, float nearZ, float farZ);

	// ���W�ϊ��iw���Z�Ȃ��j
	Vector3 Vector3Transform(const Vector3& v, const Matrix& m);
	// ���W�ϊ��iw���Z����j
	Vector3 Vector3TransformCoord(const Vector3& v, const Matrix& m);
	// �x�N�g���ϊ�
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
	/// �⊮
	/// </summary>
	/// <param name="x1">�ړ��������ϐ�</param>
	/// <param name="x2">�ړ�����������</param>
	/// <param name="flame">����</param>
	/// <returns>�⊮��̒l</returns>
	float Complement(float& x1, float& x2, float flame);

	/// <summary>
	/// �P�U���������l��Ԃ�
	/// </summary>
	/// <param name="time">�o�ߎ���</param>
	/// <param name="amplitude">�U��</param>
	/// <param name="period">����</param>
	/// <returns></returns>
	float SimpleHarmonicMotion(float time, float amplitude = 10.0f, float period = 60.0f);

	/// <summary>
	/// �~�^���������l��Ԃ�(���f����)
	/// </summary>
	/// <param name="CenterPoint">���S�_</param>
	/// <param name="radius">���S�_����̔��a</param>
	/// <param name="angle">�p�x</param>
	/// <returns></returns>
	Vector2 CircularMotion(Vector2 CenterPoint, float radius, float angle);

	float Vector3Max(const Vector3& vec);
	float Vector3Min(const Vector3& vec);

	//���̍s���timer0�̂Ƃ�0�{�Atimer1�̂Ƃ�1�{�̒l�ɂ���֐�
	Matrix Lerp(const Matrix& mat, float timer,float maxTime);
	//timer0�̂Ƃ�mat0�Atimer1�̂Ƃ�mat1�̒l�ɂ���֐�
	Matrix Lerp(const Matrix& mat0,const Matrix& mat1, float timer,float maxTime);
};