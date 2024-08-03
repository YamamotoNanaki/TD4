#pragma once
#include "IEvent.h"
#include "IFEMath.h"

class DamageEvent:public IFE::IEvent
{
public:
	DamageEvent();
	~DamageEvent();

	//初期化
	void Initialize()override;

	//毎フレーム更新
	void Update()override;

#ifdef EditorMode
	//デバック更新
	void DebugGUI()override;
#endif

	//開始時の初期化
	void StartInitialize()override;

	//受けるダメージにしたいが今その機能ないんで一応用意しているだけ
	int32_t damage_ = 1;

private:


};

