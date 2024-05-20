#pragma once
#include"Component.h"
#include"IFEMath.h"

namespace IFE {
	class EnemyHp :public IFE::Component
	{
		using Component::Component;
	private:
		const int MAX_HP = 100;
		const int HIT_COOLTIME = 25;
	private:
		//�����o�ϐ�
		int hp_;
		int preHp_;
		int decHp_;
		bool isDead_;
		//�t���O
		bool isHit_;
		int hitTime_;

		//�����o�֐�
	public:

		/// <summary>
		/// ������
		/// </summary>
		void Initialize();

		/// <summary>
		/// �X�V
		/// </summary>
		void Update(Float3 pos_);

		/// <summary>
		/// hp�\���v�Z
		/// </summary>
		void ScaleCalc();

		/// <summary>
		/// �̗͌���
		/// </summary>
		void DecHp();

		/// <summary>
		/// �ꌂ�Ŏ���
		/// </summary>
		void OneShot();

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
		void SetIsDead(bool isDead) { isDead_ = isDead; }
		void SetPos(Float3 pos_);
	};
}