#pragma once
#include "NormalEnemy.h"

namespace IFE {
	class EnemyManager : public IFE::Component
	{
	//�����o�ϐ�
	private:
		//�G���X�g
		std::list<BaseEnemy*> enemyList_;

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
		/// ���X�g�擾
		/// </summary>
		/// <returns></returns>
		std::list<BaseEnemy*>& GetEnemyList() { return enemyList_; };

		/// <summary>
		/// �I��
		/// </summary>
		//void Finalize();

#ifdef EditorMode
	public:
		//
		void ComponentDebugGUI();
		void OutputComponent(nlohmann::json& json);
#endif
		void LoadingComponent(nlohmann::json& json);
	};
}
