#pragma once
#include"Component.h"
#include"IFMath.h"

namespace IFE {
	class BaseEnemy :public IFE::Component
	{
		//�����o�ϐ�
	private:
		//���
		enum State {
			//�ҋ@
			WAIT,
			//�T��
			SEARCH,
			//�ǐ�
			Chase
		};


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
	};
}