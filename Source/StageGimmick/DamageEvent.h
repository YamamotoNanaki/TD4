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

	//デバック更新
	void DebugGUI()override;

	//受けるダメージにしたいが今その機能ないんで一応用意しているだけ
	int32_t damage_ = 1;

private:


};

