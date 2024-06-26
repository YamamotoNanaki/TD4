#pragma once

#include "IEvent.h"
#include "IFEMath.h"


class DoorEvent :public IFE::IEvent
{
public:
	DoorEvent();
	~DoorEvent();

	//������
	void Initialize()override;

	//���t���[���X�V
	void Update()override;

	//�f�o�b�N�X�V
	void DebugGUI()override;

	//�ݒ���o��
	void DebugGUI()override;

	//�g���M�~�b�N���Ńf�[�^���o�������Ƃ�
	void OutputData(nlohmann::json& json)override;

	//�g���M�~�b�N���Ńf�[�^���󂯎�肽���Ƃ�
	void InputData(nlohmann::json& json)override;

private:

	//�J�n�ʒu
	IFE::Float3 doorStartPos_;

	//�I���ʒu
	IFE::Float3 doorEndPos_;

	//�J���Ă��邩
	bool isOpen_ = false;

	//�J������
	float moveOpenTime_ = 1;

	//���鎞��
	float moveCloseTime_ = 1;


};