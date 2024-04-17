#include "View.h"
#include <cassert>

using namespace IFE;

Matrix View::sMatBillBoard_ = MatrixIdentity();
Matrix View::sMatBillBoardY_ = MatrixIdentity();

View::View(const Float3& eye, const Float3& target, const Float3& up)
{
	Initialze(eye, target, up);

}

void View::Initialze(const Float3& e, const Float3& t, const Float3& u)
{
	eye_ = e;
	target_ = t;
	up_ = u;
	Update();
}

void View::Update()
{
	//視点座標、注視点座標、上方向
	Vector3 eyePosition = SetVector3(eye_);
	Vector3 targetPosition = SetVector3(target_);
	Vector3 upVector = SetVector3(up_);

	//カメラZ軸
	Vector3 cameraAxisZ = VectorSubtract(targetPosition, eyePosition);
	//ゼロベクトルを除外
	assert(!Vector3Equal(cameraAxisZ, { 0,0,0 }));
	assert(!Vector3Equal(upVector, { 0,0,0 }));
	//ベクトル正規化
	cameraAxisZ = Vector3Normalize(cameraAxisZ);

	//カメラX軸Y軸
	Vector3 cameraAxisX;
	cameraAxisX = Vector3Cross(upVector, cameraAxisZ);
	cameraAxisX = Vector3Normalize(cameraAxisX);
	Vector3 cameraAxisY;
	cameraAxisY = Vector3Cross(cameraAxisZ, cameraAxisX);
	cameraAxisY = Vector3Normalize(cameraAxisY);

	//回転行列の作成
	Matrix matCameraRot;
	matCameraRot.SetX(cameraAxisX);
	matCameraRot.SetY(cameraAxisY);
	matCameraRot.SetZ(cameraAxisZ);
	matCameraRot.SetW(0, 0, 0, 1);

	//逆行列を代入
	matView_ = MatrixTranspose(matCameraRot);

	//カメラの位置から原点へのベクトルを生成
	Vector3 reverseEyePosition = VectorNegate(eyePosition);
	matView_.m[3][0] = Vector3Dot(cameraAxisX, reverseEyePosition);
	matView_.m[3][1] = Vector3Dot(cameraAxisY, reverseEyePosition);
	matView_.m[3][2] = Vector3Dot(cameraAxisZ, reverseEyePosition);
	matView_.m[3][3] = 1.0f;

	sMatBillBoard_.SetX(cameraAxisX);
	sMatBillBoard_.SetY(cameraAxisY);
	sMatBillBoard_.SetZ(cameraAxisZ);
	sMatBillBoard_.SetW(0, 0, 0, 1);

	//Y軸ビルボード
	Vector3 yBillAxisX, yBillAxisY, yBillAxisZ;

	yBillAxisX = cameraAxisX;
	yBillAxisY = Vector3Normalize(upVector);
	yBillAxisZ = Vector3Cross(yBillAxisX, yBillAxisY);

	sMatBillBoardY_.SetX(yBillAxisX);
	sMatBillBoardY_.SetY(yBillAxisY);
	sMatBillBoardY_.SetZ(yBillAxisZ);
	sMatBillBoardY_.SetW(0, 0, 0, 1);
}

Matrix View::Get() const
{
	return matView_;
}

Matrix* IFE::View::GetAddressOf()
{
	return &matView_;
}

void IFE::View::SetMatrixView(Matrix view)
{
	matView_ = MatrixInverse(view);
	sMatBillBoard_ = view;
	sMatBillBoard_.SetW(0, 0, 0, 1);
}
