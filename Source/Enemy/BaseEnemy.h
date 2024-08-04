#pragma once
#include"Component.h"
#include"IFEMath.h"
#include "Animator.h"
#include "EnemyHp.h"

namespace IFE {
	class BaseEnemy :public IFE::Component
	{
		using Component::Component;
	protected:
		//hp
		const int8_t MAX_HP = 100;
		const int8_t HIT_COOLTIME = 1;
		const float MAX_DEADTIME = 10.0f;
		//メンバ変数
	protected:
		//状態
		enum State {
			//待機
			WAIT,
			//探索
			SEARCH,
			//警戒
			WARNING,
			//追跡
			CHASE,
			//攻撃
			ATTACK,
			//死亡
			DEAD,
			TUTO
		};
		State state;
		Vector3 frontVec;
		//hp
		EnemyHp* hpUI = nullptr;
		EnemyHp* status_ = nullptr;
		int8_t hp_;
		int8_t decHp_;
		float deadTime= 0;
		bool isHit_;
		bool isOneShot;
		bool isDead = false;
		float hitTime_;
		//アニメーション
		IFE::Animator* ani_ = nullptr;


		//Highlighting関連の変数
	private:
		Object3D* hitObject_;
		float hitDistance_;
		bool droneHit_ = false;
		float droneHitDistance_ = 0;


		//メンバ関数
	public:
		//インスタンス
		BaseEnemy() = default;
		virtual ~BaseEnemy() = default;

		void Initialize()override final;

		/// <summary>
		/// 更新処理
		/// 自動で呼ばれます
		/// </summary>
		virtual void EnemyUpdate() = 0;
		virtual void EnemyInitialize() = 0;

		/// <summary>
		/// 終了
		/// </summary>
		virtual void Finalize() = 0;

		void SetSound();

		/// <summary>
		/// 体力減少
		/// </summary>
		void DecHp();

		/// <summary>
		/// 一撃で死ぬ
		/// </summary>
		void OneShot();

		const int8_t GetHP() { return hp_; }

		/// <summary>
		/// 更新処理
		/// BaseEnemyクラスの軽傷を行っている場合呼び出せません
		/// EnemyUpdateを使用してください
		/// </summary>
		void Update()final override;

		bool ChaseLen(Vector3 target,float len_);

		/// <summary>
		/// コライダーの処理
		/// BaseEnemyクラスの軽傷を行っている場合呼び出せません
		/// EnemyOnColliderHitを使用してください
		/// </summary>
		/// <param name="mycol">自分のコライダー</param>
		/// <param name="hitcol">相手のコライダー</param>
		void OnColliderHit(ColliderCore* mycol, ColliderCore* hitcol)final override;

		void Killed();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="current"></param>
		/// <param name="target"></param>
		/// <param name="step"></param>
		void ApproachTarget(float& current, float target, float step);

		float GetLen();

		/// <summary>
		/// コライダーの処理
		/// 自動で呼ばれます
		/// </summary>
		/// <param name="mycol">自分のコライダー</param>
		/// <param name="hitcol">相手のコライダー</param>
		virtual void EnemyOnColliderHit(ColliderCore* mycol, ColliderCore* hitcol) = 0;

		inline bool GetDroneHitRay() { return droneHit_; }
		inline float GetDroneHitDistance() { return droneHitDistance_; }
		const IFE::Vector3 GetPos();
		const bool GetIsOneShot();
		const bool GetIsDead() { return isDead; }
		void SetisOneShot(bool isOne);
		void SetMotion(std::string name);
		const bool GetBack(float judge);
	private:
		/// <summary>
		/// ドローンに光らせるための処理
		/// </summary>
		void Highlighting();

	};
}