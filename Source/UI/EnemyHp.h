#pragma once
#include"Component.h"
#include"IFEMath.h"

namespace IFE {
	class EnemyHp :public IFE::Component
	{
		using Component::Component;
	private:
		const int32_t MAX_HP = 100;
		const int32_t HIT_COOLTIME = 25;
	private:
		//�����o�ϐ�
		int32_t hp_;
		int32_t preHp_;
		int32_t decHp_;
		bool isDead_;
		//�t���O
		bool isHit_;
		int32_t hitTime_;

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

	private:
		/// <summary>
		/// hp�\���v�Z
		/// </summary>
		void ScaleCalc();

		/// <summary>
		/// �\���ʒu����
		/// </summary>
		/// <param name="pos_"></param>
		void SetPos(Float3 pos_);
	};
}