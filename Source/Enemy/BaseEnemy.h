#pragma once
#include"Component.h"
#include"IFEMath.h"
#include "Animator.h"
#include "EnemyHp.h"

namespace IFE {
	class BaseEnemy :public IFE::Component
	{
		using Component::Component;
	protected:
		//hp
		const int8_t MAX_HP = 100;
		const int8_t HIT_COOLTIME = 1;
		const float MAX_DEADTIME = 10.0f;
		//�����o�ϐ�
	protected:
		//���
		enum State {
			//�ҋ@
			WAIT,
			//�T��
			SEARCH,
			//�x��
			WARNING,
			//�ǐ�
			CHASE,
			//�U��
			ATTACK,
			//���S
			DEAD,
			TUTO
		};
		State state;
		Vector3 frontVec;
		//hp
		EnemyHp* hpUI = nullptr;
		EnemyHp* status_ = nullptr;
		int8_t hp_;
		int8_t decHp_;
		float deadTime= 0;
		bool isHit_;
		bool isOneShot;
		bool isDead = false;
		float hitTime_;
		//�A�j���[�V����
		IFE::Animator* ani_ = nullptr;


		//Highlighting�֘A�̕ϐ�
	private:
		Object3D* hitObject_;
		float hitDistance_;
		bool droneHit_ = false;
		float droneHitDistance_ = 0;


		//�����o�֐�
	public:
		//�C���X�^���X
		BaseEnemy() = default;
		virtual ~BaseEnemy() = default;

		void Initialize()override final;

		/// <summary>
		/// �X�V����
		/// �����ŌĂ΂�܂�
		/// </summary>
		virtual void EnemyUpdate() = 0;
		virtual void EnemyInitialize() = 0;

		/// <summary>
		/// �I��
		/// </summary>
		virtual void Finalize() = 0;

		void SetSound();

		/// <summary>
		/// �̗͌���
		/// </summary>
		void DecHp();

		/// <summary>
		/// �ꌂ�Ŏ���
		/// </summary>
		void OneShot();

		const int8_t GetHP() { return hp_; }

		/// <summary>
		/// �X�V����
		/// BaseEnemy�N���X�̌y�����s���Ă���ꍇ�Ăяo���܂���
		/// EnemyUpdate���g�p���Ă�������
		/// </summary>
		void Update()final override;

		bool ChaseLen(Vector3 target,float len_);

		/// <summary>
		/// �R���C�_�[�̏���
		/// BaseEnemy�N���X�̌y�����s���Ă���ꍇ�Ăяo���܂���
		/// EnemyOnColliderHit���g�p���Ă�������
		/// </summary>
		/// <param name="mycol">�����̃R���C�_�[</param>
		/// <param name="hitcol">����̃R���C�_�[</param>
		void OnColliderHit(ColliderCore* mycol, ColliderCore* hitcol)final override;

		void Killed();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="current"></param>
		/// <param name="target"></param>
		/// <param name="step"></param>
		void ApproachTarget(float& current, float target, float step);

		float GetLen();

		/// <summary>
		/// �R���C�_�[�̏���
		/// �����ŌĂ΂�܂�
		/// </summary>
		/// <param name="mycol">�����̃R���C�_�[</param>
		/// <param name="hitcol">����̃R���C�_�[</param>
		virtual void EnemyOnColliderHit(ColliderCore* mycol, ColliderCore* hitcol) = 0;

		inline bool GetDroneHitRay() { return droneHit_; }
		inline float GetDroneHitDistance() { return droneHitDistance_; }
		const IFE::Vector3 GetPos();
		const bool GetIsOneShot();
		const bool GetIsDead() { return isDead; }
		void SetisOneShot(bool isOne);
		void SetMotion(std::string name);
		const bool GetBack(float judge);
	private:
		/// <summary>
		/// �h���[���Ɍ��点�邽�߂̏���
		/// </summary>
		void Highlighting();

	};
}