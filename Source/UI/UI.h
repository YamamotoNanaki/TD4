#pragma once
#include"Component.h"
#include"IFEMath.h"
#include"SpriteManager.h"

namespace IFE {
	class UI :public IFE::Component
	{
		using Component::Component;
	private:
		//�����o�ϐ�
		int8_t hp_;
		bool isDead;

		//�����o�֐�
	public:
		// �I��
		void Finalize();

		// UI�ؑ�
		void UIChange(bool mode);

		bool GetIsDead() { return isDead; }

	private:
	};
}