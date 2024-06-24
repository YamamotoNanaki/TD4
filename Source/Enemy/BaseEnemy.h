#pragma once
#include"Component.h"
#include"IFEMath.h"

namespace IFE {
	class BaseEnemy :public IFE::Component
	{
		using Component::Component;
	private:
		//hp
		const int8_t MAX_HP = 100;
		const int8_t HIT_COOLTIME = 15;
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
			DEAD
		};
		State state;
		State preState;
		//hp
		int8_t hp_;
		int8_t decHp_;
		bool isHit_;
		int8_t hitTime_;


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

		/// <summary>
		/// ������
		/// </summary>
		virtual void Initialize() = 0;

		/// <summary>
		/// �X�V����
		/// �����ŌĂ΂�܂�
		/// </summary>
		virtual void EnemyUpdate() = 0;

		/// <summary>
		/// �`��
		/// </summary>
		virtual void Draw() = 0;

		/// <summary>
		/// �I��
		/// </summary>
		virtual void Finalize() = 0;

		/// <summary>
		/// �̗͌���
		/// </summary>
		void DecHp();

		/// <summary>
		/// �ꌂ�Ŏ���
		/// </summary>
		void OneShot();

		/// <summary>
		/// �X�V����
		/// BaseEnemy�N���X�̌y�����s���Ă���ꍇ�Ăяo���܂���
		/// EnemyUpdate���g�p���Ă�������
		/// </summary>
		void Update()final override;

		/// <summary>
		/// �R���C�_�[�̏���
		/// BaseEnemy�N���X�̌y�����s���Ă���ꍇ�Ăяo���܂���
		/// EnemyOnColliderHit���g�p���Ă�������
		/// </summary>
		/// <param name="mycol">�����̃R���C�_�[</param>
		/// <param name="hitcol">����̃R���C�_�[</param>
		void OnColliderHit(ColliderCore* mycol, ColliderCore* hitcol)final override;

		/// <summary>
		/// �R���C�_�[�̏���
		/// �����ŌĂ΂�܂�
		/// </summary>
		/// <param name="mycol">�����̃R���C�_�[</param>
		/// <param name="hitcol">����̃R���C�_�[</param>
		virtual void EnemyOnColliderHit(ColliderCore* mycol, ColliderCore* hitcol) = 0;

		inline bool GetDroneHitRay() { return droneHit_; }
		inline float GetDroneHitDistance() { return droneHitDistance_; }
		IFE::Vector2 GetPos();

	private:
		/// <summary>
		/// �h���[���Ɍ��点�邽�߂̏���
		/// </summary>
		void Highlighting();

#ifdef EditorMode
	public:
		virtual void ComponentDebugGUI() = 0;
#endif
	};
}