#pragma once
#include "NormalEnemy.h"

namespace IFE {
	class EnemyManager : public IFE::Component
	{
	//メンバ変数
	private:
		//敵リスト
		std::list<BaseEnemy*> enemyList_;

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
		/// リスト取得
		/// </summary>
		/// <returns></returns>
		std::list<BaseEnemy*>& GetEnemyList() { return enemyList_; };

		/// <summary>
		/// 終了
		/// </summary>
		//void Finalize();

#ifdef EditorMode
	public:
		//
		void ComponentDebugGUI();
		void OutputComponent(nlohmann::json& json);
#endif
		void LoadingComponent(nlohmann::json& json);
	};
}
