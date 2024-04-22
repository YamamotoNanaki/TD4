#pragma once
#include"Component.h"
#include"IFEMath.h"

namespace IFE {
	class BaseEnemy :public IFE::Component
	{
		//メンバ変数
	private:
		//状態
		enum State {
			//待機
			WAIT,
			//探索
			SEARCH,
			//追跡
			Chase
		};


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
	};
}