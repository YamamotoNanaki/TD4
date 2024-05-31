#pragma once
#include"Component.h"
#include"IFEMath.h"

namespace IFE {
	class EnemyHp :public IFE::Component
	{
		using Component::Component;
	private:
		//メンバ変数
		int8_t hp_;
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
		void Update(Float3 pos_,int8_t hp, int8_t& decHp_);

		/// <summary>
		/// アイコン用
		/// </summary>
		void IconUpdate(Float3 pos_);

		/// <summary>
		/// 描画
		/// </summary>
		void Draw();

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize();

		bool GetIsDead() { return isDead; }

	private:
		/// <summary>
		/// hp表示計算
		/// </summary>
		void ScaleCalc(int8_t& decHp_);

		/// <summary>
		/// 表示位置調整
		/// </summary>
		/// <param name="pos_"></param>
		void SetPos(Float3 pos_);
	};
}