#include "MapUI.h"
#include "ObjectManager.h"
#include "SpriteManager.h"
#include "CameraManager.h"
#include "Transform.h"
#include "SpriteManager.h"

MapUI::MapUI()
{
}

MapUI::~MapUI()
{
}

void MapUI::Initialize()
{
	IFE::ObjectManager* objm = IFE::ObjectManager::Instance();
	auto& list = objm->GetObjList();
	uint8_t count = 0;
	
	for (auto& itr : list)
	{
		if (itr->GetObjectName().find("player") != std::string::npos)
		{
			playerObj_ = itr.get();
		}
		else if (itr->GetObjectName().find("ground") != std::string::npos)
		{
			objectPos_.push_back({ 0,0 });
			objectRot_.push_back(0.0f);

			mapSprite_.push_back(IFE::SpriteManager::Instance()->AddInitialize(std::string("map"+count),""));
			count++;
		}
		
	}
}

void MapUI::Update()
{
	uint8_t count = 0;

	IFE::Float3 frontVec = IFE::CameraManager::Instance()->GetCamera("ActionCamera")->transform_->target_ - IFE::CameraManager::Instance()->GetCamera("ActionCamera")->transform_->eye_;

	//�J������angleY�̎擾(���ʃx�N�g������擾)
	float angle = -(atan2(frontVec.z, frontVec.x) + (float)IFE::PI / 2);

	IFE::ObjectManager* objm = IFE::ObjectManager::Instance();
	auto& list = objm->GetObjList();

	for (auto& itr : list)
	{
		if (itr->GetObjectName().find("ground") != std::string::npos)
		{
			//���@�ƓG�Ƃ̈ʒu�̍�
			IFE::Float2 difference = { playerObj_->transform_->position_.x - itr->objectPtr_->transform_->position_.x,
				itr->objectPtr_->transform_->position_.z - playerObj_->transform_->position_.z };
			//���@�ƓG�Ƃ̋����̎Z�o
			float length = sqrt(difference.x * difference.x) + sqrt(difference.y * difference.y);

			mapSprite_[count]->drawFlag_ = false;

			mapSprite_[count]->transform_->position2D_ = { transform2D_->position2D_.x + difference.x * cosf(angle) + difference.y * sinf(angle),transform2D_->position2D_.y + difference.y * cosf(angle) - difference.x * sinf(angle) };
			mapSprite_[count]->transform_->rotation2D_ = itr->objectPtr_->transform_->rotation_.y - (float)IFE::PI / 2 - angle;

			//�G���o�����Ă��邩���[�_�[�̓����͈͓̔��ɂ���Ȃ�
			if (transform2D_->scale2D_.x >= length && transform2D_->scale2D_.y >= length)
			{
				mapSprite_[count]->drawFlag_ = true;
			}
			count++;
		}
	}
	
}

void MapUI::Draw()
{
	
}

void MapUI::Finalize()
{

}

void MapUI::OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider)
{
	
	myCollider;
	hitCollider;
}

#ifdef EditorMode
#include "ImguiManager.h"
void MapUI::ComponentDebugGUI()
{

}

void MapUI::OutputComponent(nlohmann::json& json)
{
	json;
}

#endif

void MapUI::LoadingComponent(nlohmann::json& json)
{
	json;
}