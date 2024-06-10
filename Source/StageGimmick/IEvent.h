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

	};


}
