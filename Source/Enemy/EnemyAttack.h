#pragma once
#include"Component.h"
class Player;

namespace IFE {
	class EnemyAttack :public IFE::Component
	{
		using Component::Component;
		//�����o�ϐ�
	private:
		Player* player_;
		bool isShot;

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
		/// 
		/// </summary>
		void OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider);

		/// <summary>
		/// �I��
		/// </summary>
		void Finalize();
		void SetIsShot(bool isShot_) { isShot = isShot_; }

		void ComponentDebugGUI();
	};
}