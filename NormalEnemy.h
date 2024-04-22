#pragma once
#include "BaseEnemy.h"

namespace IFE {
	//---めっちゃ普通の敵---//
	class NormalEnemy : public IFE::BaseEnemy
	{
	//メンバ変数
	private:

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
		/// 描画
		/// </summary>
		void Finalize();
	};
}