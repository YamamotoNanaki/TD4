#pragma once
#include "BaseEnemy.h"

namespace IFE {
	//---めっちゃ普通の敵---//
	class NormalEnemy : public IFE::BaseEnemy
	{
	//メンバ変数
	private:
		//待機タイマー
		int waitTimer;
		//経由地点
		std::vector<Vector3> points;
		//経由地点数
		int nowPoints;

	//メンバ関数
	public:
		/// <summary>
		/// 座標指定なし初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 座標指定あり初期化
		/// </summary>
		void Initialize(const Vector3& pos_, const std::vector<Vector3>& points);

		/// <summary>
		/// 状態変化
		/// </summary>
		void ChangeState();

		/// <summary>
		/// 更新
		/// </summary>
		void Update();

		/// <summary>
		/// 動け
		/// </summary>
		void Move();

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