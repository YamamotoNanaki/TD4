#pragma once
#include"Component.h"

namespace IFE {
	class EnemyAttack :public IFE::Component
	{
		using Component::Component;
		//メンバ変数
	private:

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

		void ComponentDebugGUI();
	};
}