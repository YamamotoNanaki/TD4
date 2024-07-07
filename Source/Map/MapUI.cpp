#include "MapUI.h"
#include "ObjectManager.h"
#include "SpriteManager.h"
#include "CameraManager.h"

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
	
	for (auto& itr : list)
	{
		if (itr->GetObjectName().find("player") != std::string::npos)
		{
			playerObj_ = itr.get();
			break;
		}
	}
}

void MapUI::Update()
{

}

void MapUI::Draw()
{
	uint8_t count = 0;
	//�J������angleY�̎擾(���ʃx�N�g������擾)
	//float angle = -(atan2(IFE::CameraManager::Instance()->GetCamera("ActionCamera")-> , cameraFrontVec_.x) + myMath::AX_PIF / 2);

	//for (auto& enemy : enemys->GetEnemyList())
	//{
	//	//���@�ƓG�Ƃ̈ʒu�̍�
	//	myMath::Vector2 difference = { player->GetTransform().parentToTranslation.x - enemy->GetTrans().translation.x,
	//		enemy->GetTrans().translation.z - player->GetTransform().parentToTranslation.z };
	//	//���@�ƓG�Ƃ̋����̎Z�o
	//	float length = sqrt(difference.x * difference.x) + sqrt(difference.y * difference.y);

	//	//�G���o�����Ă��邩���[�_�[�̓����͈͓̔��ɂ���Ȃ�
	//	if ((enemy->GetSpawnFlag() == true && radarSize >= length) && enemy->GetDeathAnimationFlag() == false)
	//	{
	//		radarEnemys_[count]->DrawSprite2D({ center_.x + difference.x * cosf(angle) + difference.y * sinf(angle),
	//			center_.y + difference.y * cosf(angle) - difference.x * sinf(angle) }, { 1.0f,1.0f ,1.0f ,1.0f }, { 1.0f,1.0f }, enemy->GetTrans().rotation.y - myMath::AX_PIF / 2 - angle);
	//	}
	//	count++;
	//}
	//player_->DrawSprite2D(center_, { 1.0f,1.0f,1.0f,1.0f }, { 1.0f ,1.0f }, 0.0f);
	//radar_->DrawSprite2D(center_);
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