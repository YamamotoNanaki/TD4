#pragma once
#include"Component.h"
#include"IFEMath.h"

namespace IFE {
	class EnemyHp :public IFE::Component
	{
		using Component::Component;
	private:
		//�����o�ϐ�
		int8_t hp_;
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
		void Update(Float3 pos_,int8_t hp, int8_t& decHp_);

		/// <summary>
		/// �A�C�R���p
		/// </summary>
		void IconUpdate(Float3 pos_);

		/// <summary>
		/// �`��
		/// </summary>
		void Draw();

		/// <summary>
		/// �I��
		/// </summary>
		void Finalize();

		bool GetIsDead() { return isDead; }

	private:
		/// <summary>
		/// hp�\���v�Z
		/// </summary>
		void ScaleCalc(int8_t& decHp_);

		/// <summary>
		/// �\���ʒu����
		/// </summary>
		/// <param name="pos_"></param>
		void SetPos(Float3 pos_);
	};
}