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
	//���_���W�A�����_���W�A�����
	Vector3 eyePosition = SetVector3(eye_);
	Vector3 targetPosition = SetVector3(target_);
	Vector3 upVector = SetVector3(up_);

	//�J����Z��
	Vector3 cameraAxisZ = VectorSubtract(targetPosition, eyePosition);
	//�[���x�N�g�������O
	assert(!Vector3Equal(cameraAxisZ, { 0,0,0 }));
	assert(!Vector3Equal(upVector, { 0,0,0 }));
	//�x�N�g�����K��
	cameraAxisZ = Vector3Normalize(cameraAxisZ);

	//�J����X��Y��
	Vector3 cameraAxisX;
	cameraAxisX = Vector3Cross(upVector, cameraAxisZ);
	cameraAxisX = Vector3Normalize(cameraAxisX);
	Vector3 cameraAxisY;
	cameraAxisY = Vector3Cross(cameraAxisZ, cameraAxisX);
	cameraAxisY = Vector3Normalize(cameraAxisY);

	//��]�s��̍쐬
	Matrix matCameraRot;
	matCameraRot.SetX(cameraAxisX);
	matCameraRot.SetY(cameraAxisY);
	matCameraRot.SetZ(cameraAxisZ);
	matCameraRot.SetW(0, 0, 0, 1);

	//�t�s�����
	matView_ = MatrixTranspose(matCameraRot);

	//�J�����̈ʒu���猴�_�ւ̃x�N�g���𐶐�
	Vector3 reverseEyePosition = VectorNegate(eyePosition);
	matView_.m[3][0] = Vector3Dot(cameraAxisX, reverseEyePosition);
	matView_.m[3][1] = Vector3Dot(cameraAxisY, reverseEyePosition);
	matView_.m[3][2] = Vector3Dot(cameraAxisZ, reverseEyePosition);
	matView_.m[3][3] = 1.0f;

	sMatBillBoard_.SetX(cameraAxisX);
	sMatBillBoard_.SetY(cameraAxisY);
	sMatBillBoard_.SetZ(cameraAxisZ);
	sMatBillBoard_.SetW(0, 0, 0, 1);

	//Y���r���{�[�h
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
