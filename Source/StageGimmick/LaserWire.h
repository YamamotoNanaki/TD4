#pragma once
#include "Component.h"
#include "Object3D.h"
#include "Collider.h"
#include "IEvent.h"

//�C�x���g�̎�ޗp�ӂ͂������ǒu���ꏊ�ɍ����Ă�
enum EventType
{
	damage,
	door,
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

		return "";
	}
};

class LaserWire:public IFE::Component
{
public:

	//���ݓ������Ă��邩
	bool isHit_ = false;

public:

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider);

#ifdef EditorMode

	void ComponentDebugGUI()override;
	void OutputComponent(nlohmann::json& json)override;
#endif
	void LoadingComponent(nlohmann::json& json)override;


private:

	IFE::Float3 pos_;

	std::vector<IFE::Float3> poss_;
	std::vector<IFE::Float3> rots_;
	std::vector<IFE::Float3> scales_;

	std::vector<IFE::Object3D*> objects_;

	//�e�X�g�Œu���`��p�̃I�u�W�F�N�g�Ȃ񂩂��������ɒǉ�������@����Ȃ璼��
	IFE::Object3D testobj_[10];

	int32_t oldposSize = 0;
	int32_t oldscaleSize = 0;

	IFE::Collider* cols_;

	float countHitTimer_ = 0;
	float hitMaxTime_ = 0;

	std::unique_ptr<IFE::IEvent> event_;

	//��������C�x���g�̎�ނ��L�^���邽�߂ɗp��
	int32_t eventType_ = EventType::damage;

	bool isEventStart = false;

private:
	//enum����string���擾���邽�߂̊֐�
	std::string EventTypeToString(EventType eventType);
};

