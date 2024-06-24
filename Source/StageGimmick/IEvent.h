#pragma once

namespace IFE {
	class IEvent
	{
	public:
		virtual ~IEvent() {};

		//初期化
		virtual void Initialize() {};

		//終了処理
		virtual void Finalize() {};

		//毎フレーム更新
		virtual void Update() {};

		//終了フラグ獲得
		virtual bool GetIsEnd() { return isEnd_; };

		//設定を出す
		virtual void DebugGUI() {};

	protected:

		//このイベントが終わっているか
		bool isEnd_ = false;

	};


}
