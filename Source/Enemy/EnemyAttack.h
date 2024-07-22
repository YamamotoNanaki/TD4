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
		bool isFront;

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
		void SetIsFront(bool isFront_) { isFront = isFront_; }
		bool GetIsShot() { return isShot; }
		bool GetIsFront() { return isFront; }

		void ComponentDebugGUI();
	};
}