#pragma once
#include"Component.h"
#include"IFEMath.h"

namespace IFE {
	class EnemyHp :public IFE::Component
	{
		using Component::Component;
	private:
		const int MAX_HP = 100;
	private:
		//�����o�ϐ�
		int hp_;
		bool isDead;

		//�����o�֐�
	public:

		/// <summary>
		/// ������
		/// </summary>
		void Initialize();

		/// <summary>
		/// �X�V
		/// </summary>
		void Update();

		/// <summary>
		/// �`��
		/// </summary>
		void Draw();

		/// <summary>
		/// �I��
		/// </summary>
		void Finalize();

		//getter
		int GetHp() { return hp_; }
		//setter
		void SetIsDead(bool isDead_) { isDead = isDead_; }
	};
}