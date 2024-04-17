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
		/// �t���[�g�^�̃����_���֐�
		/// </summary>
		/// <param name="min">�ŏ��l</param>
		/// <param name="max">�ő�l</param>
		/// <returns>�����_1����float�^����</returns>
		static float GetRandF(float min, float max, int32_t resolution = 10000);
	};
}