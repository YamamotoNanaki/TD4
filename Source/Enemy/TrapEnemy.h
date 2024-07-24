#pragma once
#include "BaseEnemy.h"
#include "IFEMath.h"
#include "EnemyHp.h"
#include "enemyAttack.h"
#include "IFETime.h"

namespace IFE {
	//---めっちゃ普通の敵---//
	class TrapEnemy : public IFE::BaseEnemy
	{
		using BaseEnemy::BaseEnemy;
	private:
		//追跡時速度
		const float CHASE_VELO = 5.0f;
	private:
		//経由地点
		std::vector<Float3> points;
		//次の場所
		Vector3 trapPos;
		//攻撃時間
		float attackTime;
		//警戒時間
		float warningTime;
		//発見
		bool isFound;
		bool isChaseDrone;
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
		void Initialize();

		/// <summary>
		/// 状態変化
		/// </summary>
		void ChangeState();

		/// <summary>
		/// 更新
		/// </summary>
		void EnemyUpdate()override;

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
		/// 
		/// </summary>
		void Killed();

		/// <summary>
		/// プレイヤーの方を向かせる
		/// </summary>
		void LookAt();

		/// <summary>
		/// 視線判定
		/// </summary>
		bool RaySight(Vector3 pos);

		/// <summary>
		/// 描画
		/// </summary>
		void Draw();

		/// <summary>
		/// ヒット時
		/// </summary>
		/// <param name="collider"></param>
		void EnemyOnColliderHit(ColliderCore* myCollider, ColliderCore* hitCollider)override;

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize();


		///-----Getter-----///
		const Vector3 GetPos();
		const bool GetIsAttack() { return isAttack; }
		const bool GetIsHit() { return isHit_; }
		const bool GetIsFound() { return isFound; }

		void SetTrapPos(Vector3 tp) { trapPos = tp; }

#ifdef EditorMode
		/// <summary>
		/// デバック中に調整できるらしい
		/// </summary>
		void ComponentDebugGUI()override;
		void OutputComponent(nlohmann::json& json)override;
#endif
		void LoadingComponent(nlohmann::json& json)override;
	};
}