#pragma once
#include "NormalEnemy.h"

namespace IFE {
	class EnemyManager
	{
	//�����o�ϐ�
	private:
		//�G���X�g
		std::list<std::unique_ptr<NormalEnemy>> enemyList_;


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
		/// �`��
		/// </summary>
		void Draw();

		/// <summary>
		/// ���X�g�擾
		/// </summary>
		/// <returns></returns>
		std::list<std::unique_ptr<NormalEnemy>>* GetEnemyList() { return &enemyList_; };

		/// <summary>
		/// �I��
		/// </summary>
		void Finalize();

		//
		void ComponentDebugGUI();
	};
}