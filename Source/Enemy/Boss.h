#pragma once
#pragma once
#include "BaseEnemy.h"
#include "EnemyHp.h"
#include "IFEMath.h"
#include "EnemyAttack.h"

namespace IFE {
	class Boss : public IFE::BaseEnemy
	{
		using BaseEnemy::BaseEnemy;
	private:
		//周りを見渡す時間
		const float WAIT_TIME = 1.0f;
		//探索時移動速度
		const float SEARCH_VELO = 3.0f;
		//追跡時速度
		const float CHASE_VELO = 7.0f;
		//メンバ変数
	private:
		//待機タイマー
		float waitTimer;
		//経由地点
		std::vector<Float3> points;
		//次の場所
		size_t nextPoint;
		//攻撃時間
		float attackTime;
		//警戒時間
		float warningTime;
		//発見
		bool isFound;
		bool isChaseDrone;
		uint8_t attackNum;
		Vector3 lookfor;
		Vector3 shotVec;
		//hp
		EnemyHp* hpUI = nullptr;
		EnemyHp* status_ = nullptr;
		//攻撃判定クラス
		IFE::EnemyAttack* enemyAttack = nullptr;
		bool isAttack;
		//レイヒット記憶変数
		float rayDist;
		ColliderCore* hitColl_ = nullptr;
		//メンバ関数
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize()override;

		/// <summary>
		/// 状態変化
		/// </summary>
		void ChangeState();

		/// <summary>
		/// 更新
		/// </summary>
		void EnemyUpdate()override;

		/// <summary>
		/// 待て
		/// </summary>
		void Wait();

		/// <summary>
		/// 警戒
		/// </summary>
		void Warning();

		/// <summary>
		/// 探せ
		/// </summary>
		void Search();

		/// <summary>
		/// 追え
		/// </summary>
		void Chase();

		/// <summary>
		/// 殴れ
		/// </summary>
		void Attack();

		/// <summary>
		/// 撃て
		/// </summary>
		void Shot();

		/// <summary>
		/// プレイヤーの方を向かせる
		/// </summary>
		void LookAt();

		/// <summary>
	/// 視線判定
	/// </summary>
		bool RaySight(Vector3 pos);

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

		bool GetIsHit() { return isHit_; }

#ifdef EditorMode
		/// <summary>
		/// デバック中に調整できるらしい
		/// </summary>
		void ComponentDebugGUI()override;
#endif
	};
}

