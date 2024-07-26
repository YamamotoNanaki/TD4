#pragma once
#include "JsonManager.h"
#include "Transform.h"

//�C�x���g�̎�ޗp�ӂ͂������ǒu���ꏊ�ɍ����Ă�
enum EventType
{
	damage,
	door,
	DroneKeepoutZone,
	EnemySpawn,
	GimmickSwitch,
	EventCount//�ő�l�A�Ō�ɂ����Ă�������
};

struct EventName
{
	static std::string EventString(int32_t eventNum)
	{
		if (eventNum == EventType::damage)
		{
			return "DamageEvent";
		}
		else if (eventNum == EventType::door)
		{
			return "DoorEvent";
		}
		else if (eventNum == EventType::DroneKeepoutZone)
		{
			return "DroneKeepoutZoneEvent";
		}
		else if (eventNum == EventType::EnemySpawn)
		{
			return "EnemySpawnEvent";
		}
		else if (eventNum == EventType::GimmickSwitch)
		{
			return "GimmickSwitchEvent";
		}

		return "";
	}
};

//enum����string���擾���邽�߂̊֐�
std::string EventTypeToString(EventType eventType);

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
