#pragma once
#pragma once
#include "BaseEnemy.h"
#include "EnemyHp.h"
#include "IFEMath.h"
#include "EnemyAttack.h"

namespace IFE {
	class Boss : public IFE::BaseEnemy
	{
		using BaseEnemy::BaseEnemy;
	private:
		//��������n������
		const float WAIT_TIME = 1.0f;
		//�T�����ړ����x
		const float SEARCH_VELO = 3.0f;
		//�ǐՎ����x
		const float CHASE_VELO = 7.0f;
		//�����o�ϐ�
	private:
		//�ҋ@�^�C�}�[
		float waitTimer;
		//�o�R�n�_
		std::vector<Float3> points;
		//���̏ꏊ
		size_t nextPoint;
		//�U������
		float attackTime;
		//�x������
		float warningTime;
		//����
		bool isFound;
		bool isChaseDrone;
		uint8_t attackNum;
		Vector3 lookfor;
		Vector3 shotVec;
		//hp
		EnemyHp* hpUI = nullptr;
		EnemyHp* status_ = nullptr;
		//�U������N���X
		IFE::EnemyAttack* enemyAttack = nullptr;
		bool isAttack;
		//���C�q�b�g�L���ϐ�
		float rayDist;
		ColliderCore* hitColl_ = nullptr;
		//�����o�֐�
	public:
		/// <summary>
		/// ������
		/// </summary>
		void Initialize()override;

		/// <summary>
		/// ��ԕω�
		/// </summary>
		void ChangeState();

		/// <summary>
		/// �X�V
		/// </summary>
		void EnemyUpdate()override;

		/// <summary>
		/// �҂�
		/// </summary>
		void Wait();

		/// <summary>
		/// �x��
		/// </summary>
		void Warning();

		/// <summary>
		/// �T��
		/// </summary>
		void Search();

		/// <summary>
		/// �ǂ�
		/// </summary>
		void Chase();

		/// <summary>
		/// ����
		/// </summary>
		void Attack();

		/// <summary>
		/// ����
		/// </summary>
		void Shot();

		/// <summary>
		/// �v���C���[�̕�����������
		/// </summary>
		void LookAt();

		/// <summary>
	/// ��������
	/// </summary>
		bool RaySight(Vector3 pos);

		/// <summary>
		/// �q�b�g��
		/// </summary>
		/// <param name="collider"></param>
		void EnemyOnColliderHit(ColliderCore* myCollider, ColliderCore* hitCollider)override;

		void Killed();

		/// <summary>
		/// �`��
		/// </summary>
		void Draw();

		/// <summary>
		/// �I��
		/// </summary>
		void Finalize();

		bool GetIsHit() { return isHit_; }

#ifdef EditorMode
		/// <summary>
		/// �f�o�b�N���ɒ����ł���炵��
		/// </summary>
		void ComponentDebugGUI()override;
#endif
	};
}

