#pragma once
#include"Component.h"
#include"IFEMath.h"

namespace IFE {
	class BaseEnemy :public IFE::Component
	{
		using Component::Component;
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
			DEAD
		};
		State state;
		State preState;


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

		/// <summary>
		/// 初期化
		/// </summary>
		virtual void Initialize() = 0;

		/// <summary>
		/// 更新処理
		/// 自動で呼ばれます
		/// </summary>
		virtual void EnemyUpdate() = 0;

		/// <summary>
		/// 描画
		/// </summary>
		virtual void Draw() = 0;

		/// <summary>
		/// 終了
		/// </summary>
		virtual void Finalize() = 0;

		/// <summary>
		/// 更新処理
		/// BaseEnemyクラスの軽傷を行っている場合呼び出せません
		/// EnemyUpdateを使用してください
		/// </summary>
		void Update()final override;

		/// <summary>
		/// コライダーの処理
		/// BaseEnemyクラスの軽傷を行っている場合呼び出せません
		/// EnemyOnColliderHitを使用してください
		/// </summary>
		/// <param name="mycol">自分のコライダー</param>
		/// <param name="hitcol">相手のコライダー</param>
		void OnColliderHit(ColliderCore* mycol, ColliderCore* hitcol)final override;

		/// <summary>
		/// コライダーの処理
		/// 自動で呼ばれます
		/// </summary>
		/// <param name="mycol">自分のコライダー</param>
		/// <param name="hitcol">相手のコライダー</param>
		virtual void EnemyOnColliderHit(ColliderCore* mycol, ColliderCore* hitcol) = 0;

		inline bool GetDroneHitRay() { return droneHit_; }
		inline float GetDroneHitDistance() { return droneHitDistance_; }

	private:
		/// <summary>
		/// ドローンに光らせるための処理
		/// </summary>
		void Highlighting();

#ifdef EditorMode
	public:
		virtual void ComponentDebugGUI() = 0;
#endif
	};
}