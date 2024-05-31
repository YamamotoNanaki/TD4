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
		/// 更新
		/// </summary>
		virtual void Update() = 0;

		/// <summary>
		/// 描画
		/// </summary>
		virtual void Draw() = 0;

		/// <summary>
		/// 終了
		/// </summary>
		virtual void Finalize() = 0;

#ifdef EditorMode
		virtual void ComponentDebugGUI() = 0;
#endif
	};
}