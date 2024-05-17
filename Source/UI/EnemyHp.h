#pragma once
#include"Component.h"
#include"IFEMath.h"

namespace IFE {
	class EnemyHp :public IFE::Component
	{
		using Component::Component;
	private:
		const int MAX_HP = 100;
	private:
		//メンバ変数
		int hp_;
		bool isDead;

		//メンバ関数
	public:

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 更新
		/// </summary>
		void Update();

		/// <summary>
		/// 描画
		/// </summary>
		void Draw();

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize();

		//getter
		int GetHp() { return hp_; }
		//setter
		void SetIsDead(bool isDead_) { isDead = isDead_; }
	};
}