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

	//�f�o�b�N�X�V
	void DebugGUI()override;

	//�󂯂�_���[�W�ɂ������������̋@�\�Ȃ���ňꉞ�p�ӂ��Ă��邾��
	int32_t damage_ = 1;

private:


};

