#pragma once
#include "NormalEnemy.h"

namespace IFE {
	class EnemyManager
	{
	//メンバ変数
	private:
		//敵リスト
		std::list<std::unique_ptr<NormalEnemy>> enemyList_;


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
		/// リスト取得
		/// </summary>
		/// <returns></returns>
		std::list<std::unique_ptr<NormalEnemy>>* GetEnemyList() { return &enemyList_; };

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize();

		//
		void ComponentDebugGUI();
	};
}