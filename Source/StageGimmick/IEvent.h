#pragma once
#include "JsonManager.h"
#include "Transform.h"

namespace IFE {
	class IEvent
	{
	public:
		virtual ~IEvent() {};

		//������
		virtual void Initialize() {};

		//�I������
		virtual void Finalize() {};

		//���t���[���X�V
		virtual void Update() {};

		//�I���t���O�l��
		virtual bool GetIsEnd() { return isEnd_; };

		//�ݒ���o��
		virtual void DebugGUI() {};

		//�g���M�~�b�N���Ńf�[�^���o�������Ƃ�
		virtual void OutputData(nlohmann::json& json) { json; };

		//�g���M�~�b�N���Ńf�[�^���󂯎�肽���Ƃ�
		virtual void InputData(nlohmann::json& json) { json; };

		//�M�~�b�N���̈ʒu���~�����Ƃ�
		virtual void InputTransform(const IFE::Transform& transform) { transform; };

		//�J�n���̏�����
		virtual void StartInitialize() {};
		

	protected:

		//���̃C�x���g���I����Ă��邩
		bool isEnd_ = false;

	};


}
