#pragma once
#include"Component.h"
#include"IFEMath.h"
#include"SpriteManager.h"
#include"Transform.h"

namespace IFE {
	class PlayerHp :public IFE::Component
	{
		using Component::Component;
	private:
		//メンバ変数
		int8_t hp_;
		bool isDead;

		//メンバ関数
	public:

		void Update();

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize();

		bool GetIsDead() { return isDead; }

		/// <summary>
		/// hp表示計算
		/// </summary>
		void ScaleCalc(int8_t decHp_);
	};
}