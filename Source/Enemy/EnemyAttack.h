#pragma once
#include"Component.h"

namespace IFE {
	class EnemyAttack :public IFE::Component
	{
		using Component::Component;
		//�����o�ϐ�
	private:


		//�����o�֐�
	public:
		/// <summary>
		/// ������
		/// </summary>
		void Initialize();

		/// <summary>
		/// �X�V
		/// </summary>
		void Update();

		/// <summary>
		/// �I��
		/// </summary>
		void Finalize();

		void ComponentDebugGUI();
	};
}