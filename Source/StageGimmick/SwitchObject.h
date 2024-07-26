#pragma once
#include "Component.h"
#include "Object3D.h"
#include "Collider.h"
#include "IEvent.h"

class SwitchObject :public IFE::Component
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

	bool GetIsHit() { return isHit_; };

	void SetIsActive(bool flag);

#ifdef EditorMode

	void ComponentDebugGUI()override;
	void OutputComponent(nlohmann::json& json)override;
#endif
	void LoadingComponent(nlohmann::json& json)override;


private:

	//�����蔻���transform
	IFE::Float3 scale_ = { 1,1,1 };


	IFE::Collider* cols_;

	float countHitTimer_ = 0;
	float hitMaxTime_ = 0;

	std::unique_ptr<IFE::IEvent> event_;

	//��������C�x���g�̎�ނ��L�^���邽�߂ɗp��
	int32_t eventType_ = EventType::damage;

	bool isEventStart = false;

	//�X�C�b�`���N��������
	bool isSwitchObject_ = false;

	//�M�~�b�N�������Ă��邩
	bool isActive_ = true;

	bool testflag_ = true;

private:

};