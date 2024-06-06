#pragma once
#include "BaseEnemy.h"
#include "PlayerAction.h"
#include "IFEMath.h"
#include "EnemyHp.h"
#include "enemyAttack.h"
#include "IFETime.h"

namespace IFE {
	//---めっちゃ普通の敵---//
	class NormalEnemy : public IFE::BaseEnemy
	{
		using BaseEnemy::BaseEnemy;
	private:
		//周りを見渡す時間
		const int32_t WAIT_TIME = 200;
		//探索時移動速度
		const float SEARCH_VELO = 2.5f;
		//追跡時速度
		const float CHASE_VELO = 5.0f;
		//メンバ変数
	private:
		//待機タイマー
		int32_t waitTimer;
		//経由地点
		std::vector<Float3> points;
		//次の場所
		size_t nextPoint;
		Vector3 frontVec;
		//攻撃時間
		int32_t attackTime;
		//警戒時間
		int32_t warningTime;
		//発見
		bool isFound;
		Vector3 lookfor;
		//hp
		EnemyHp* hpUI = nullptr;
		EnemyHp* status_ = nullptr;
		//攻撃判定クラス
		EnemyAttack* enemyAttack = nullptr;
		bool isAttack;
		//レイヒット記憶変数
		float rayDist;
		float preRayDist;
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
		/// プレイヤーの方を向かせる
		/// </summary>
		void LookAt();

		/// <summary>
		/// 視線判定
		/// </summary>
		bool RaySight();

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
		Vector3 GetPos();
		bool GetBack();
		bool GetIsAttack() { return isAttack; }
		bool GetIsHit() { return isHit_; }
		bool GetIsFound() { return isFound; }

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