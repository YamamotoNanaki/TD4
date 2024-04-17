#include "Projection.h"
#include "WindowsAPI.h"

using namespace IFE;

Projection::Projection(float radian, float winWidth, float winHeight, float nearZ, float farZ)
	:fovAngle_(radian), winWidth_(winWidth), winHeight_(winHeight), nearZ_(nearZ), farZ_(farZ)
{
	Inisialize(radian, winWidth, winHeight, nearZ, farZ);
}

IFE::Projection::Projection(float radian, float nearZ, float farZ)
	:fovAngle_(radian), winWidth_((float)WindowsAPI::Instance()->winWidth_), winHeight_((float)WindowsAPI::Instance()->winHeight_), nearZ_(nearZ), farZ_(farZ)
{
}

void Projection::Inisialize(float radian, float ww, float wh, float nz, float fz)
{
	matProjection_ = MatrixPerspectiveFovLH(
		ConvertToRadians(radian), (float)ww / wh, nz, fz);
}

void IFE::Projection::Update()
{
	matProjection_ = MatrixPerspectiveFovLH(
		ConvertToRadians(fovAngle_), (float)winWidth_ / winHeight_, nearZ_, farZ_);
}

Matrix Projection::Get() const
{
	return matProjection_;
}

Matrix* IFE::Projection::GetAddressOf()
{
	return &matProjection_;
}
