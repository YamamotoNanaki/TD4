#pragma once
#include <cstdint>

namespace IFE
{
	struct IFERand
	{
		static void Initialize();
		static int32_t GetRand();
		static int32_t GetRand(int32_t min, int32_t max);
		/// <summary>
		/// フロート型のランダム関数
		/// </summary>
		/// <param name="min">最小値</param>
		/// <param name="max">最大値</param>
		/// <returns>小数点1桁のfloat型乱数</returns>
		static float GetRandF(float min, float max, int32_t resolution = 10000);
	};
}