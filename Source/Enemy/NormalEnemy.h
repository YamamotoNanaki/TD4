#pragma once
#include "BaseEnemy.h"
#include "PlayerAction.h"

namespace IFE {
	//---めっちゃ普通の敵---//
	class NormalEnemy : public IFE::BaseEnemy
	{
	//メンバ変数
	private:
		//待機タイマー
		int waitTimer;
		//経由地点
		std::vector<Float3> points;
		//経由地点数
		int nowPoints;
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
		void OnColliderHit(IFE::Collider collider);

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize();

		/// <summary>
		/// デバック中に調整できるらしい
		/// </summary>
		void ComponentDebugGUI();
	};
}