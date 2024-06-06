#pragma once
#include"Component.h"
#include"IFEMath.h"
#include"SpriteManager.h"

namespace IFE {
	class UI :public IFE::Component
	{
		using Component::Component;
	private:
		//ƒƒ“ƒo•Ï”
		int8_t hp_;
		bool isDead;

		//ƒƒ“ƒoŠÖ”
	public:
		// I—¹
		void Finalize();

		// UIØ‘Ö
		void UIChange(bool mode);

		bool GetIsDead() { return isDead; }

	private:
	};
}