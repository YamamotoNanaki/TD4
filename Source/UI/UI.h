#pragma once
#include"Component.h"
#include"IFEMath.h"
#include"SpriteManager.h"

namespace IFE {
	class UI :public IFE::Component
	{
		using Component::Component;
	private:
		//メンバ変数
		int8_t hp_;
		bool isDead;

		//メンバ関数
	public:
		// 終了
		void Finalize();

		// UI切替
		void UIChange(bool mode);

		// ダメージ
		void Damage(int8_t hp);

		bool GetIsDead() { return isDead; }

	private:
	};
}