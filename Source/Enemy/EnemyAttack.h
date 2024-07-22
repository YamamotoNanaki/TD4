#pragma once
#include"Component.h"
class Player;

namespace IFE {
	class EnemyAttack :public IFE::Component
	{
		using Component::Component;
		//メンバ変数
	private:
		Player* player_;
		bool isShot;
		bool isFront;

		//メンバ関数
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 更新
		/// </summary>
		void Update();

		/// <summary>
		/// 
		/// </summary>
		void OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider);

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize();
		void SetIsShot(bool isShot_) { isShot = isShot_; }
		void SetIsFront(bool isFront_) { isFront = isFront_; }
		bool GetIsShot() { return isShot; }
		bool GetIsFront() { return isFront; }

		void ComponentDebugGUI();
	};
}