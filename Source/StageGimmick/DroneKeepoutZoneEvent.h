#pragma once
#include "IEvent.h"
#include "IFEMath.h"
#include "Object3D.h"

class DroneKeepoutZoneEvent :public IFE::IEvent
{
public:
	DroneKeepoutZoneEvent();
	~DroneKeepoutZoneEvent();

	//������
	void Initialize()override;

	//���t���[���X�V
	void Update()override;

	//�f�o�b�N�X�V
	void DebugGUI()override;

	//�g���M�~�b�N���Ńf�[�^���o�������Ƃ�
	void OutputData(nlohmann::json& json)override;

	//�g���M�~�b�N���Ńf�[�^���󂯎�肽���Ƃ�
	void InputData(nlohmann::json& json)override;

	//�J�n���̏�����
	void StartInitialize()override;

private:

	//�J�n�ʒu
	IFE::Float3 pos_;

	//�I���ʒu
	IFE::Float3 scele_ = {1,1,1};

	//�֎~�]�[���̖{��
	IFE::Object3D* zoneObject_ = nullptr;


};