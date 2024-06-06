#include "EnemyHighlighting.h"
#include "EnemyManager.h"
#include "ObjectManager.h"
#include "GraphicsPipelineManager.h"
#include "LightManager.h"
#include "Transform.h"
#include "Collision.h"
#include "CameraManager.h"

using namespace IFE;
using namespace std;

void EnemyHighlighting::Initialize()
{
	name_ = "EnemyHighlighting";
	gp_ = GraphicsPipelineManager::Instance()->CreatePostEffectPipeLine("EnemyHighlightingVS", "EnemyHighlightingPS", "EnemyHighlighting", 2, 1);
	enemyJuge = GraphicsPipelineManager::Instance()->CreateObjectGraphicsPipeLine("EnemyJudgeVS", "EnemyJudgeGS", "EnemyJudgePS", "EnemyJudge", 1, 1, 2);
	drawFlag_ = false;
	auto obj = ObjectManager::Instance()->GetObjectPtr("PlayerDrone");
	if (obj)dronePosition = &obj->transform_->position_;
	droneCamera_ = CameraManager::Instance()->GetCamera("DroneCamera");
	cb = make_unique<ConstBuffer<Judge>>();
	cbFalse = make_unique<ConstBuffer<Judge>>();
	playerAction = ObjectManager::Instance()->GetObjectPtr("PlayerAction");
	playerDrone = ObjectManager::Instance()->GetObjectPtr("PlayerDrone");
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
		if (!obj)continue;
		if (!obj->isActive_)continue;
		if (!obj->DrawFlag_)continue;
		//��Q���̔���
		if (!itr->GetDroneHitRay())continue;
		if (itr->GetDroneHitDistance() > droneHighlightingDistance_)continue;
		//������J�����O
		Sphere sphere(itr->transform_->position_, Vector3Max(itr->transform_->scale_));
		if (!droneCamera_->IsFrustumCulling(sphere))continue;

		objList.push_back(obj);
	}
	if (objList.size() == 0)return;
	objList.sort([](const Object3D* objA, const Object3D* objB) {return objA->gp_->pipelineNum_ > objB->gp_->pipelineNum_; });
	uint8_t num = 255;
	enemyJuge->SetDrawBlendMode();
	num = enemyJuge->pipelineNum_;
	LightManager::Instance()->Draw(3);
	CameraManager::Instance()->Draw();
	cb->GetCBMapObject()->enemy = true;
	cb->SetConstBuffView(6);
	for (auto& itr : objList)
	{
		itr->Draw();
	}
	cbFalse->GetCBMapObject()->enemy = false;
	cbFalse->SetConstBuffView(6);
	playerAction->Draw();
	playerDrone->Draw();
	PostEffectDrawAfter();
}

void EnemyHighlighting::Draw()
{
	tex_[0]->SetTexture(1);
	tex_[1]->SetTexture(2);
}
