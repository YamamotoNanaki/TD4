#pragma once
#include"Component.h"
#include"IFEMath.h"

namespace IFE {
	class BaseEnemy :public IFE::Component
	{
		using Component::Component;
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
		/// �I��
		/// </summary>
		virtual void Finalize() = 0;

#ifdef EditorMode
		virtual void ComponentDebugGUI() = 0;
#endif
	};
}