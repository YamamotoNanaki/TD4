#pragma once
#include "IEvent.h"

class DamageEvent:public IFE::IEvent
{
public:
	DamageEvent();
	~DamageEvent();

	//初期化
	void Initialize()override;

	//毎フレーム更新
	void Update()override;

private:

};

