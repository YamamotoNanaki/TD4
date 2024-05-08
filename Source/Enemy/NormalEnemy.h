#pragma once
#include "BaseEnemy.h"
#include "PlayerAction.h"

namespace IFE {
	//---めっちゃ普通の敵---//
	class NormalEnemy : public IFE::BaseEnemy
	{
	private:
		const float LERP_TIME = 200.f;
		//周りを見渡す時間
		const int WAIT_TIME = 200;
		//探索時移動速度 
		const float MOVE_SPEED = 2.0f;
	//メンバ変数
	private:
		//待機タイマー
		int waitTimer;
		//経由地点
		std::vector<Float3> points;
		//経由地点
		int pointA;
		int pointB;
		//lerp用
		float moveTime;

	//メンバ関数
	public:
		/// <summary>
		/// 座標指定なし初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 座標指定あり初期化
		/// </summary>
		void Initialize(const Vector3& pos_, const std::vector<Float3>& points);

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
		/// 描画
		/// </summary>
		void Draw();

		/// <summary>
		/// ヒット時
		/// </summary>
		/// <param name="collider"></param>
		void OnColliderHit(IFE::Collider* collider)override;

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize();


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