#include "EnemyHighlighting.h"
#include "EnemyManager.h"
#include "ObjectManager.h"
#include "GraphicsPipelineManager.h"
#include "LightManager.h"
#include "Transform.h"
#include "Collision.h"

using namespace IFE;
using namespace std;

void EnemyHighlighting::Initialize()
{
	name_ = "EnemyHighlighting";
	gp_ = GraphicsPipelineManager::Instance()->CreatePostEffectPipeLine("EnemyHighlightingVS", "EnemyHighlightingPS", "EnemyHighlighting", 2, 1);
	drawFlag_ = false;
	auto obj = ObjectManager::Instance()->GetObjectPtr("PlayerDrone");
	if (obj)dronePosition = &obj->transform_->position_;
}

void EnemyHighlighting::Update()
{
	auto em = ObjectManager::Instance()->GetObjectPtr("EnemyManager")->GetComponent<EnemyManager>();
	if (em->GetEnemyList().size() == 0)return;
	PostEffectDrawBefore();
	list<Object3D*>objList;
	for (auto& itr : em->GetEnemyList())
	{
		auto obj = itr->objectPtr_;
		if (!obj->isActive_)continue;
		if (!obj->DrawFlag_)continue;
		if (!itr->GetDroneHitRay())continue;
		//if (itr->GetDroneHitDistance() > droneHighlightingDistance_)continue;

		objList.push_back(obj);
	}

	objList.sort([](const Object3D* objA, const Object3D* objB) {return objA->gp_->pipelineNum_ > objB->gp_->pipelineNum_; });
	uint8_t num = 255;
	for (auto& itr : objList)
	{
		if (num != itr->gp_->pipelineNum_)
		{
			itr->gp_->SetDrawBlendMode();
			num = itr->gp_->pipelineNum_;
			LightManager::Instance()->Draw(3);
			CameraManager::Instance()->Draw();
		}
		itr->Draw();
	}
	PostEffectDrawAfter();
}

void EnemyHighlighting::Draw()
{
	tex_[0]->SetTexture(1);
	tex_[1]->SetTexture(2);
}
