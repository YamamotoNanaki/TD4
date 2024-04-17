#pragma once
#include "IFMath.h"

namespace IFE
{
	class View
	{
	private:
		Matrix matView_;

	public:
		static Matrix sMatBillBoard_;
		static Matrix sMatBillBoardY_;
		Float3 eye_;
		Float3 target_;
		Float3 up_;
	public:
		View(const Float3& eye = { 0,10,-50 }, const Float3& target = { 0,10,1 }, const Float3& up = { 0,1,0 });
		void Initialze(const Float3& eye, const Float3& target, const Float3& up);
		void Update();
		Matrix Get()const;
		Matrix* GetAddressOf();
		void SetMatrixView(Matrix view);
	};
}