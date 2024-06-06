#pragma once
#include"Component.h"
#include"IFEMath.h"
#include"SpriteManager.h"
#include"Transform.h"
#include"Ease.h"

namespace IFE {
	class PlayerHp :public IFE::Component
	{
		using Component::Component;
	private:
		//�����o�ϐ�
		int8_t hp_;
		bool isDead;

		//�����o�֐�
	public:

		void Initialize();

		void Update();

		/// <summary>
		/// �I��
		/// </summary>
		void Finalize();

		bool GetIsDead() { return isDead; }

		/// <summary>
		/// hp�\���v�Z
		/// </summary>
		void ScaleCalc(uint8_t hp,uint8_t decHp,float hitTime,float maxTime);

		void SetHp(uint8_t hp);
	};
}