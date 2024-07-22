#pragma once
#include "BaseEnemy.h"
#include "IFEMath.h"
#include "EnemyHp.h"
#include "enemyAttack.h"
#include "IFETime.h"

namespace IFE {
	//---�߂����ᕁ�ʂ̓G---//
	class TrapEnemy : public IFE::BaseEnemy
	{
		using BaseEnemy::BaseEnemy;
	private:
		//�ǐՎ����x
		const float CHASE_VELO = 5.0f;
	private:
		//�o�R�n�_
		std::vector<Float3> points;
		//���̏ꏊ
		Vector3 trapPos;
		//�U������
		float attackTime;
		//�x������
		float warningTime;
		//����
		bool isFound;
		bool isChaseDrone;
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
		void Initialize();

		/// <summary>
		/// ��ԕω�
		/// </summary>
		void ChangeState();

		/// <summary>
		/// �X�V
		/// </summary>
		void EnemyUpdate()override;

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
		/// 
		/// </summary>
		void Killed();

		/// <summary>
		/// �v���C���[�̕�����������
		/// </summary>
		void LookAt();

		/// <summary>
		/// ��������
		/// </summary>
		bool RaySight(Vector3 pos);

		/// <summary>
		/// �`��
		/// </summary>
		void Draw();

		/// <summary>
		/// �q�b�g��
		/// </summary>
		/// <param name="collider"></param>
		void EnemyOnColliderHit(ColliderCore* myCollider, ColliderCore* hitCollider)override;

		/// <summary>
		/// �I��
		/// </summary>
		void Finalize();


		///-----Getter-----///
		const Vector3 GetPos();
		const bool GetIsAttack() { return isAttack; }
		const bool GetIsHit() { return isHit_; }
		const bool GetIsFound() { return isFound; }

		void SetTrapPos(Vector3 tp) { trapPos = tp; }

#ifdef EditorMode
		/// <summary>
		/// �f�o�b�N���ɒ����ł���炵��
		/// </summary>
		void ComponentDebugGUI()override;
		void OutputComponent(nlohmann::json& json)override;
#endif
		void LoadingComponent(nlohmann::json& json)override;
	};
}