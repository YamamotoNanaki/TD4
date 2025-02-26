#include "TrapEnemy.h"
#include "Transform.h"
#include "Collider.h"
#include "IFETime.h"
#include "ObjectManager.h"
#include "ModelManager.h"
#include "StageCollideManageer.h"
#include "TextureManager.h"
#include "PlayerAction.h"
#include"PlayerDrone.h"
#include "Sound.h"

void IFE::TrapEnemy::EnemyInitialize()
{
	trapPos = { 0,0,0 };
	attackTime = 0;
	rayDist = 0.0f;
	isFound = false;
	isAttack = false;
	isOneShot = false;
	warningTime = 0.5f;
	hp_ = 100;
	decHp_ = 0;
	isHit_ = false;
	hitTime_ = 0;
	frontVec = { 0,0,0 };
	lookfor = { 0,0,0 };
	shotVec = { 0,0,0 };
	//HPUI
	if (!hpUI)
	{
		auto ptr = IFE::ObjectManager::Instance()->AddInitialize("EnemyHp", ModelManager::Instance()->GetModel("hppanel"));
		ptr->AddComponent<EnemyHp>();
		hpUI = ptr->GetComponent<EnemyHp>();
		ptr = IFE::ObjectManager::Instance()->AddInitialize("EnemyHp", ModelManager::Instance()->GetModel("hppanel"));
		ptr->AddComponent<EnemyHp>();
		status_ = ptr->GetComponent<EnemyHp>();
		status_->objectPtr_->DrawFlag_ = false;
	}
	//攻撃
	auto ptr = IFE::ObjectManager::Instance()->AddInitialize("EnemyAttack", ModelManager::Instance()->GetModel("dice"));
	ptr->AddComponent<EnemyAttack>();
	enemyAttack = ptr->GetComponent<EnemyAttack>();
	SetSound();
}

void IFE::TrapEnemy::ChangeState()
{
	if (hpUI->GetIsDead() == true && state != DEAD) {
		ani_->loop_ = false;
		status_->objectPtr_->DrawFlag_ = false;
		if (isOneShot) {
			isOneShot = false;
			ani_->SetAnimation("downFront", false);
		}
		else {
			ani_->SetAnimation("downBack", false);
		}
		state = DEAD;
	}
	//攻撃は最優先
	switch (state)
	{
	case IFE::BaseEnemy::WAIT:
		state = SEARCH;
		break;
	case IFE::BaseEnemy::SEARCH:
		if (!isOneShot) {
			/*if (!IFE::Sound::Instance()->GetPlayStatus("walk")) {
			IFE::Sound::Instance()->SoundPlay("walk", false, true);
			}*/
			status_->objectPtr_->DrawFlag_ = false;
			Search();
		}
		break;
	case IFE::BaseEnemy::WARNING:
		if (!isOneShot) {
			//警戒状態のUIに変更
			status_->objectPtr_->GetComponent<Material>()->SetTexture(TextureManager::Instance()->GetTexture("eye"));
			status_->objectPtr_->DrawFlag_ = true;
			Warning();
		}
		break;
	case IFE::BaseEnemy::CHASE:
		if (!isOneShot) {
			/*if (!IFE::Sound::Instance()->GetPlayStatus("walk")) {
			 IFE::Sound::Instance()->SoundPlay("walk", false, true);
			}*/
			//追跡状態のUIに変更
			status_->objectPtr_->GetComponent<Material>()->SetTexture(TextureManager::Instance()->GetTexture("exclamation"));
			status_->objectPtr_->DrawFlag_ = true;
			Chase();
		}
		break;
	case IFE::BaseEnemy::ATTACK:
		if (!isOneShot) {
			if (isChaseDrone == true) {
				Shot();
			}
			else {
				Attack();
			}
		}
		break;
	case IFE::BaseEnemy::DEAD:
		deadTime += IFE::IFETime::sDeltaTime_;
		if (deadTime >= 2.0f) {
			if (!isDead) {
				hpUI->objectPtr_->Destroy();
				status_->objectPtr_->Destroy();
				enemyAttack->objectPtr_->Destroy();
				isDead = true;
				objectPtr_->GetComponent<Collider>()->GetCollider(1)->active_ = false;
			}
		}
		break;
	default:
		break;
	}
}

void IFE::TrapEnemy::EnemyUpdate()
{
	if (state != DEAD) {
		if (state != WAIT) {
			LookAt();
		}
		if (state != CHASE) {
			isFound = !ChaseLen(ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetPos(),20);
			if (isFound == false && IFE::ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->GetComponent<PlayerDrone>()->GetIsDroneSurvival() == true) {
				isFound = RaySight(IFE::ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->GetComponent<PlayerDrone>()->GetPos());
				isChaseDrone = isFound;
			}
			else if (!IFE::ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->GetComponent<PlayerDrone>()->GetIsDroneSurvival()) {
				isChaseDrone = false;
			}
		}
		//hp表示
		hpUI->Update(transform_->position_, hp_, decHp_);
		status_->IconUpdate(transform_->position_);
		//重力
		if (!objectPtr_->GetComponent<Collider>()->GetCollider(1)->onGround_)
		{
			transform_->position_.y -= 4.9f * IFETime::sDeltaTime_;
		}
	}
	ChangeState();
	if (state != DEAD) {
		if (ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetHP() == 0) {
			state = SEARCH;
			ani_->SetAnimation("walk", false);
		}
		rayDist = 0;
	}
}

void IFE::TrapEnemy::Warning()
{
	//異変の状態が続いたら追跡へ移行
	if (isFound == true) {
		warningTime += IFE::IFETime::sDeltaTime_;
	}
	else {
		warningTime -=IFE::IFETime::sDeltaTime_;
	}

	if (warningTime >= 0.9f) {
		warningTime = 0.5f;
		state = CHASE;
		ani_->SetAnimation("walk");
	}
	if (warningTime <= 0) {
		color.w -= 0.05f;
		objectPtr_->SetColor(color);
		if (color.w == 0) {
			hpUI->objectPtr_->Destroy();
			status_->objectPtr_->Destroy();
			enemyAttack->objectPtr_->Destroy();
			objectPtr_->Destroy();
		}
	}
}

void IFE::TrapEnemy::Search()
{
	Vector3 addVec = trapPos - (Vector3)transform_->position_;
	addVec.Normalize();
	transform_->position_ += (addVec * CHASE_VELO * IFE::IFETime::sDeltaTime_);


	//前フレームに敵を見つけていたなら警戒体制に
	if (isFound == true) {
		state = WARNING;
	}
}

void IFE::TrapEnemy::Chase()
{
	//とりあえず追いかける
	Vector3 ePos = transform_->position_;
	Vector3 target;
	if (isChaseDrone == false) {
		target = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetPos();
	}
	else {
		target = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->GetComponent<PlayerDrone>()->GetPos();
	}
	//近くにいるか判定
	if (ChaseLen(target,20)) {
		warningTime += IFE::IFETime::sDeltaTime_;
	}
	//playerの方を向く
	lookfor = target;
	Vector3 addVec = target - ePos;
	addVec.Normalize();
	transform_->position_ += (addVec * CHASE_VELO * IFE::IFETime::sDeltaTime_);

	//近づいたら殴る
	double len = sqrt(pow(ePos.x - target.x, 2) + pow(ePos.y - target.y, 2) +
		pow(ePos.z - target.z, 2));
	if (isChaseDrone == false) {
		if (len <= 3.0) {
			state = ATTACK;
			enemyAttack->objectPtr_->transform_->position_ = ePos + (addVec * 2);
			enemyAttack->objectPtr_->transform_->scale_ = { 1,1,1 };
			ani_->loop_ = false;
			ani_->SetAnimation("knifeAttack");
			frontVec = target - ePos;
			frontVec = frontVec.Normalize();
			enemyAttack->SetIsBack(GetBack(0.9f));
			float radY = std::atan2(frontVec.x, frontVec.z);
			float targetAngle = ((radY * 180.0f) / (float)PI);
			transform_->rotation_.y = targetAngle;
		}
	}
	else {
		if (len <= 18.0) {
			enemyAttack->objectPtr_->DrawFlag_ = true;
			state = ATTACK;
			enemyAttack->objectPtr_->transform_->position_ = ePos + (addVec * 2);
			isAttack = true;
			enemyAttack->SetIsShot(true);
			enemyAttack->objectPtr_->transform_->scale_ = { 0.1f,0.1f,0.1f };
			IFE::Sound::Instance()->SoundPlay("gun", false, true);
			ani_->SetAnimation("gunAttack");
			float radY = std::atan2(frontVec.x, frontVec.z);
			float targetAngle = ((radY * 180.0f) / (float)PI);
			transform_->rotation_.y = targetAngle;
		}
	}
	if (warningTime >= 2.0f) {
		warningTime = 0.5f;
		state = WARNING;
		/*IFE::Sound::Instance()->SoundPlay("what", false, true);*/
		ani_->SetAnimation("search");
	}
}

void IFE::TrapEnemy::Attack()
{
	attackTime += IFE::IFETime::sDeltaTime_;
	if (attackTime > 0.6 && attackTime < 0.8f) {
		IFE::Sound::Instance()->SoundPlay("attack", false, true);
	}
	if (attackTime > 0.8f - IFE::IFETime::sDeltaTime_) {
		isAttack = true;
	}

	if (attackTime > 2.0f) {
		attackTime = 0;
		isAttack = false;
		state = SEARCH;
		ani_->SetAnimation("walk");
	}
	enemyAttack->objectPtr_->GetComponent<IFE::Collider>()->GetCollider(0)->active_ = isAttack;
}

void IFE::TrapEnemy::Shot()
{
	Vector3 worldPos = enemyAttack->transform_->GetWorldPosition();
	Vector3 dPos = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->GetComponent<PlayerDrone>()->GetPos();
	attackTime += 100 * IFE::IFETime::sDeltaTime_;
	if (enemyAttack->GetIsShot()) {
		if (attackTime < 3) {
			shotVec = dPos - worldPos;
			shotVec.Normalize();
		}
		enemyAttack->transform_->position_ += (shotVec * 0.3f);
	}
	if (enemyAttack->GetIsShot() == false) {
		state = CHASE;
		isAttack = false;
		ani_->SetAnimation("walk");
	}
	else if (attackTime > 100) {
		enemyAttack->objectPtr_->DrawFlag_ = false;
		enemyAttack->SetIsShot(false);
		state = CHASE;
		isAttack = false;
		ani_->SetAnimation("walk");
		attackTime = 0;
	}
	enemyAttack->objectPtr_->GetComponent<IFE::Collider>()->GetCollider(0)->active_ = isAttack;
}

void IFE::TrapEnemy::LookAt()
{
	Vector3 ePos = transform_->position_;
	frontVec = lookfor - ePos;
	frontVec = frontVec.Normalize();
	frontVec *= Vector3(1, 0, 1);
	if (state == CHASE || state == SEARCH || state == WARNING || isOneShot == false) {
		//カメラ方向に合わせてY軸の回転
		float radY = std::atan2(frontVec.x, frontVec.z);
		float targetAngle = ((radY * 180.0f) / (float)PI);
		ApproachTarget(transform_->rotation_.y, targetAngle, 2.0f);
	}
}

bool IFE::TrapEnemy::RaySight(Vector3 pos) {
	//視界の距離
	float maxDistance = 20;
	//視野角
	float sightAngle = 45;
	// 自身の位置
	Vector3 ePos = transform_->position_;
	// ターゲットの位置
	Vector3 targetPos = pos;
	objectPtr_->GetComponent<Collider>()->GetCollider(2)->rayDir_ = Vector3(pos - ePos).Normalize();

	//俺の正面ベクトル
	Vector3 selfDir = frontVec;

	// ターゲットまでの向きと距離計算
	Vector3 targetDir = targetPos - ePos;
	float targetDistance = targetDir.Length();
	targetDir.Normalize();

	// cos(θ/2)を計算
	float cosHalf = cos(ConvertToRadians(sightAngle / 2.0f * (float)PI / 180.0f));
	cosHalf *= 10;
	cosHalf = std::floor(cosHalf);

	// 自身とターゲットへの向きの内積計算
	// ターゲットへの向きベクトルを正規化する必要があることに注意
	float innerProduct = selfDir.Dot(targetDir) / targetDir.Length();
	innerProduct *= 10;
	innerProduct = std::floor(innerProduct);

	// 視界判定
	bool inSight = cosHalf <= innerProduct && targetDistance < maxDistance;

	//// 障害物がないかどうかを判定
	if (rayDist < targetDistance && rayDist > 0) {
		// ターゲットよりも障害物が近い場合は視界が遮られている
		inSight = false;
	}

	return inSight;
}

void IFE::TrapEnemy::Draw()
{

}

void IFE::TrapEnemy::EnemyOnColliderHit(ColliderCore* myCollider, ColliderCore* hitCollider)
{
	//壁があった場合
	if (myCollider->GetColliderType() == ColliderType::RAY) {
		if (hitCollider->objectPtr_->GetObjectName().find("wall") != std::string::npos || hitCollider->objectPtr_->GetObjectName().find("box") != std::string::npos) {
			if (rayDist == 0) {
				rayDist = myCollider->rayDistance;
			}
			else if (rayDist > myCollider->rayDistance) {
				rayDist = myCollider->rayDistance;
			}
		}
	}
}

const IFE::Vector3 IFE::TrapEnemy::GetPos() {
	Vector3 temp = transform_->position_;
	return temp;
}

void IFE::TrapEnemy::Finalize()
{
	delete hpUI;
	delete enemyAttack;
}

#ifdef EditorMode
#include "ImguiManager.h"
void IFE::TrapEnemy::ComponentDebugGUI()
{
	ImguiManager* gui = ImguiManager::Instance();
	gui->DragVectorFloat3GUI(points, "points", transform_->position_);
	std::vector<std::string> str = {
		"WAIT",
		"SEARCH",
		"WARNING",
		"CHASE",
		"ATTACK",
		"DEAD",
		"TUTO"
	};
	int32_t num = static_cast<int32_t>(state);
	gui->Combo("state", num, str);
	state = static_cast<State>(num);
}

void IFE::TrapEnemy::OutputComponent(nlohmann::json& json)
{
	JsonManager::Instance()->OutputVectorFloat3(json["points"], points);
	json["state"] = state;
}
#endif

void IFE::TrapEnemy::LoadingComponent(nlohmann::json& json)
{
	JsonManager::Instance()->InputVectorFloat3(json["points"], points);
	int32_t num = 0;
	JsonManager::Instance()->GetData(json, "state", num);
	state = static_cast<State>(num);
}
