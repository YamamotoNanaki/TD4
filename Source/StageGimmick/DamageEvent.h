#pragma once
#include "IEvent.h"

class DamageEvent:public IFE::IEvent
{
public:
	DamageEvent();
	~DamageEvent();

	//������
	void Initialize()override;

	//���t���[���X�V
	void Update()override;

private:

};

