#pragma once
#include"Component.h"
#include"IFEMath.h"

namespace IFE {
	class EnemyHp :public IFE::Component
	{
		using Component::Component;
	private:
		const int32_t MAX_HP = 100;
		const int32_t HIT_COOLTIME = 25;
	private:
		//メンバ変数
		int32_t hp_;
		int32_t preHp_;
		int32_t decHp_;
		bool isDead_;
		//フラグ
		bool isHit_;
		int32_t hitTime_;

		//メンバ関数
	public:

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 更新
		/// </summary>
		void Update(Float3 pos_);

		/// <summary>
		/// 体力減少
		/// </summary>
		void DecHp();

		/// <summary>
		/// 一撃で死ぬ
		/// </summary>
		void OneShot();

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
		void SetIsDead(bool isDead) { isDead_ = isDead; }

	private:
		/// <summary>
		/// hp表示計算
		/// </summary>
		void ScaleCalc();

		/// <summary>
		/// 表示位置調整
		/// </summary>
		/// <param name="pos_"></param>
		void SetPos(Float3 pos_);
	};
}