#pragma once
#include "BaseEnemy.h"
#include "PlayerAction.h"
#include "IFEMath.h"
#include "EnemyHp.h"
#include "enemyAttack.h"
#include "IFETime.h"

namespace IFE {
	//---�߂����ᕁ�ʂ̓G---//
	class NormalEnemy : public IFE::BaseEnemy
	{
	private:
		//��������n������
		const int32_t WAIT_TIME = 200;
		//�T�����ړ����x 
		const float SEARCH_VELO = 2.5f;
		//�ǐՎ����x
		const float CHASE_VELO = 5.0f;
	//�����o�ϐ�
	private:
		//�ҋ@�^�C�}�[
		int32_t waitTimer;
		//�o�R�n�_
		std::vector<Float3> points;
		//���̏ꏊ
		size_t nextPoint;
		//�U������
		int32_t attackTime;
		//�x������
		int32_t warningTime;
		//hp
		EnemyHp* hp_ = nullptr;
		//�U������N���X
		EnemyAttack* enemyAttack = nullptr;

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
		void Update();

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
		/// �v���C���[�̕�����������
		/// </summary>
		void LookAt();

		/// <summary>
		/// �`��
		/// </summary>
		void Draw();

		/// <summary>
		/// �q�b�g��
		/// </summary>
		/// <param name="collider"></param>
		void OnColliderHit(ColliderCore* myCollider, ColliderCore* hitCollider)override;

		/// <summary>
		/// �I��
		/// </summary>
		void Finalize();


		///-----Getter-----///
		Vector3 GetPos();


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