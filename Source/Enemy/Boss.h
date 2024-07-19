#pragma once
#pragma once
#include "BaseEnemy.h"
#include "EnemyHp.h"
#include "IFEMath.h"

namespace IFE {
	class Boss : public IFE::BaseEnemy
	{
		using BaseEnemy::BaseEnemy;
	private:
		//hp
		EnemyHp* hpUI = nullptr;
		//メンバ関数
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 更新
		/// </summary>
		void EnemyUpdate()override;

		/// <summary>
		/// ヒット時
		/// </summary>
		/// <param name="collider"></param>
		void EnemyOnColliderHit(ColliderCore* myCollider, ColliderCore* hitCollider)override;

		void Killed();

		/// <summary>
		/// 描画
		/// </summary>
		void Draw();

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize();

		bool GetBack();
		bool GetIsHit() { return isHit_; }

#ifdef EditorMode
		/// <summary>
		/// デバック中に調整できるらしい
		/// </summary>
		void ComponentDebugGUI()override;
#endif
	};
}

