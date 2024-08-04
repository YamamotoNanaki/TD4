#pragma once
#include "IEvent.h"
#include "IFEMath.h"
#include "Object3D.h"


class GimmickSwitchEvent :public IFE::IEvent
{
public:
	GimmickSwitchEvent();
	~GimmickSwitchEvent();

	//������
	void Initialize()override;

	//���t���[���X�V
	void Update()override;

#ifdef EditorMode
	//�f�o�b�N�X�V
	void DebugGUI()override;

	//�g���M�~�b�N���Ńf�[�^���o�������Ƃ�
	void OutputData(nlohmann::json& json)override;
#endif

	//�g���M�~�b�N���Ńf�[�^���󂯎�肽���Ƃ�
	void InputData(nlohmann::json& json)override;

	//�J�n���̏�����
	void StartInitialize()override;

private:

	std::string selectGimmickName_;

	IFE::Object3D* selectGimmickObject_ = nullptr;

	//�؂�ւ�
	bool isSwitch_ = false;

	int32_t selectNum = 0;

};