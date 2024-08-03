#pragma once

#include "IEvent.h"
#include "IFEMath.h"
#include "Object3D.h"


class DoorEvent :public IFE::IEvent
{
public:
	DoorEvent();
	~DoorEvent();

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

	//�J�n�ʒu
	IFE::Float3 doorStartPos_;

	//�I���ʒu
	IFE::Float3 doorEndPos_;

	//�h�A�̃T�C�Y
	IFE::Float3 doorScele_;

	IFE::Object3D* doorObject_=nullptr;

	//�J���Ă��邩
	bool isOpen_ = false;

	//�J�E���g
	float moveTimer_ = 0;

	//�J������
	float moveOpenTime_ = 1;

	//���鎞��
	float moveCloseTime_ = 1;


};