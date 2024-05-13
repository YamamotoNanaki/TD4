#pragma once
#include "BaseEnemy.h"
#include "PlayerAction.h"
#include "IFEMath.h"
#include "EnemyHp.h"

namespace IFE {
	//---めっちゃ普通の敵---//
	class NormalEnemy : public IFE::BaseEnemy
	{
	private:
		//周りを見渡す時間
		const int WAIT_TIME = 200;
		//探索時移動速度 
		const float MOVE_VELO = 0.08f;
	//メンバ変数
	private:
		//待機タイマー
		int waitTimer;
		//経由地点
		std::vector<Float3> points;
		//次の場所
		size_t nextPoint;
		//攻撃したか
		bool isAttack;
		//hp
		EnemyHp* hp_ = nullptr;

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
		void Update();

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
		/// 描画
		/// </summary>
		void Draw();

		/// <summary>
		/// ヒット時
		/// </summary>
		/// <param name="collider"></param>
		void OnColliderHit(ColliderCore* myCollider, ColliderCore* hitCollider)override;

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize();


		///-----Getter-----///
		Vector3 GetPos();


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