#include "BaseEnemy.h"
#include "Collider.h"
#include "Object3d.h"
#include "transform.h"
#include "ObjectManager.h"
#include "PlayerDrone.h"
#include "IFETime.h"
#include "Sound.h"
#include "PlayerAction.h"
#include "StringUtil.h"
#include "Blood.h"
#include "IFEEffekseerManager.h"
#include "TextureManager.h"

using namespace IFE;

const IFE::Vector2 IFE::BaseEnemy::GetPos()
{
	return { transform_->position_.x, transform_->position_.z };
}

const bool IFE::BaseEnemy::GetIsOneShot()
{
	if (state == DEAD) {
		return true;
	}
	return isOneShot;
}

void IFE::BaseEnemy::Highlighting()
{
	Vector3 dir = ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->transform_->position_ - transform_->position_;
	dir.Normalize();
	objectPtr_->GetComponent<Collider>()->GetCollider(0)->rayDir_ = dir;

	droneHit_ = false;
	droneHitDistance_ = 0;
	if (hitObject_)
	{
		if (hitObject_->GetComponent<PlayerDrone>())
		{
			droneHit_ = true;
			droneHitDistance_ = hitDistance_;
		}
	}

	hitObject_ = nullptr;
	hitDistance_ = 0;

	ani_ = objectPtr_->GetComponent<IFE::Animator>();
}

void IFE::BaseEnemy::Update()
{
	//hitcool
	if (isHit_ == true) {
		hitTime_-= IFE::IFETime::sDeltaTime_ * 3.5f;
		if (hitTime_ <= 0) {
			isHit_ = false;
		}
	}
	EnemyUpdate();
	Highlighting();
}

bool IFE::BaseEnemy::ChaseLen(Vector3 target)
{
	Vector3 ePos = transform_->position_;

	//�߂Â����牣��
	double len = sqrt(pow(ePos.x - target.x, 2) + pow(ePos.y - target.y, 2) +
		pow(ePos.z - target.z, 2));
	if (len >= 20.0f) {
		return true;
	}
	return false;
}

void IFE::BaseEnemy::ApproachTarget(float& current, float target, float step)
{
	if (std::abs(current - target) > 180)
	{
		if (target > current)
		{
			target -= 360;
		}
		else
		{
			target += 360;
		}
	}
	if (std::abs(current - target) < step) {
		current = target; // �ڕW�l�ɂقڒB�����ꍇ�A�ڕW�l�ɐݒ�
	}
	else if (current < target) {
		current += step; // ���ݒl���ڕW�l��菬�����ꍇ�A����
	}
	else {
		current -= step; // ���ݒl���ڕW�l���傫���ꍇ�A����
	}
}

float IFE::BaseEnemy::GetLen()
{
	return 0.0f;
}

void IFE::BaseEnemy::Initialize()
{
	auto texm = TextureManager::Instance();
	for (auto& mat : objectPtr_->GetComponent<Material>()->childMaterials_)
	{
		if (mat.first.name == "Enemy0")
		{
			mat.first.tex = texm->GetTexture("enemy");
		}
		if (mat.first.name == "Gun0")
		{
			mat.first.tex = texm->GetTexture("Gun");
		}
		if (mat.first.name == "Helmet0")
		{
			mat.first.tex = texm->GetTexture("helmet");
		}
		mat.first.ambient = { 0.3f,0.3f ,0.3f };
		mat.first.diffuse = { 0.7f,0.7f ,0.7f };
		mat.first.specular = { 1.0f,1.0f ,1.0f };
		mat.first.color = { 0.4f,0.4f,0.4f,1 };
	}
	EnemyInitialize();
}

void IFE::BaseEnemy::SetSound()
{
	IFE::Sound::Instance()->LoadWave("attack", Sound::SoundSettings::SE);
	IFE::Sound::Instance()->SetVolume("attack", 60);
	IFE::Sound::Instance()->LoadWave("gun", Sound::SoundSettings::SE);
	IFE::Sound::Instance()->SetVolume("gun", 60);
	//IFE::Sound::Instance()->LoadMP3("what");
	//IFE::Sound::Instance()->SetVolume("what", 60);
	//IFE::Sound::Instance()->LoadMP3("found");
	//IFE::Sound::Instance()->SetVolume("found", 60);
	/*IFE::Sound::Instance()->LoadWave("walk");
	IFE::Sound::Instance()->SetVolume("walk", 4);*/
}

void IFE::BaseEnemy::DecHp()
{
	if (isHit_ == false) {
		if (hp_ > 0) {
			ani_->SetAnimation("damage");
		}
		else if(hp_ <= 0) {
			ani_->SetAnimation("downBack");
		}
		hp_ -= 25;
		decHp_ = 25;
		hitTime_ = HIT_COOLTIME;
		isHit_ = true;
		auto e = IFE::IFEEffekseerManager::Instance()->GetEffekseer("LowHPBlood");
		e->Play(&transform_->position_, nullptr, nullptr, 0, 0, { 0.5,0.5,0.5 });
	}
}

void IFE::BaseEnemy::OneShot()
{
	if (isHit_ == false) {
		ani_->SetAnimation("downFront");
		decHp_ = hp_;
		hp_ -= hp_;
		hitTime_ = HIT_COOLTIME;
		isHit_ = true;
		auto blood = ObjectManager::Instance()->GetObjectPtr("Blood");
		if (blood)blood->GetComponent<Blood>()->Play(this);
	}
}

void IFE::BaseEnemy::Killed() {
	objectPtr_->GetComponent<Collider>()->GetCollider(1)->SetNoPushBackFlag(true);
	Vector3 pPos = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetPos();
	Vector3 addVec = (Vector3)transform_->position_ - pPos;
	addVec.Normalize();
	Vector3 rot = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetRot();
	transform_->position_ = pPos + (addVec);
	transform_->rotation_ = rot;
	status_->objectPtr_->DrawFlag_ = false;
	ani_->SetAnimation("standBy", false);
}

void IFE::BaseEnemy::OnColliderHit(ColliderCore* mycol, ColliderCore* hitcol)
{
	if (mycol->colliderNumber_ == 0)
	{
		if (mycol->rayDistance > -2)
		{
			if (hitcol->objectPtr_)
			{
				if (Contains(hitcol->objectPtr_->GetObjectName(), "wall") || Contains(hitcol->objectPtr_->GetObjectName(), "Player"))
				{
					if (!hitObject_)
					{
						hitObject_ = hitcol->objectPtr_;
						hitDistance_ = mycol->rayDistance;
					}
					else if (hitObject_->GetObjectName() != "PlayerDrone");
					else
					{
						if (hitDistance_ > mycol->rayDistance)
						{
							hitObject_ = hitcol->objectPtr_;
							hitDistance_ = mycol->rayDistance;
						}
					}
				}
			}
		}
	}
	EnemyOnColliderHit(mycol, hitcol);
}


void IFE::BaseEnemy::SetisOneShot(bool isOne)
{
	if (state != DEAD) {
		isOneShot = isOne;
	}
	else {
		isOneShot = false;
	}
}

void  IFE::BaseEnemy::SetMotion(std::string name) {
	ani_->SetAnimation(name);
}

const bool IFE::BaseEnemy::GetBack(float judge)
{
	Vector3 pFront = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetFrontVec();
	float result = pFront.Dot(-frontVec);
	if (result < judge) {
		return true;
	}
	return false;
}