#pragma once
#include"Component.h"
#include"IFEMath.h"

namespace IFE {
	class BaseEnemy :public IFE::Component
	{
		//�����o�ϐ�
	protected:
		//���
		enum State {
			//�ҋ@
			WAIT,
			//�T��
			SEARCH,
			//�ǐ�
			CHASE
		};
		State state;


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
		/// �X�V
		/// </summary>
		virtual void Update() = 0;

		/// <summary>
		/// �`��
		/// </summary>
		virtual void Draw() = 0;

		/// <summary>
		/// �����蔻��
		/// </summary>
		/// <param name="collider"></param>
		//virtual void OnColliderHit(IFE::Collider collider) = 0;

		/// <summary>
		/// �I��
		/// </summary>
		virtual void Finalize() = 0;

		virtual void ComponentDebugGUI() = 0;
	};
}