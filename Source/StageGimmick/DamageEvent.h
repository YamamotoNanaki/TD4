#pragma once
#include "IEvent.h"
#include "IFEMath.h"

class DamageEvent:public IFE::IEvent
{
public:
	DamageEvent();
	~DamageEvent();

	//������
	void Initialize()override;

	//���t���[���X�V
	void Update()override;

	int32_t damage_ = 1;

private:


};

