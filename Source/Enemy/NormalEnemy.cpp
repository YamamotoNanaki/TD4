#include "NormalEnemy.h"
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

void IFE::NormalEnemy::EnemyInitialize()
{
	waitTimer = 0;
	nextPoint = 0;
	attackTime = 0;
	rayDist = 0.0f;
	isFound = false;
	isOneShot = false;
	isAttack = false;
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

void IFE::NormalEnemy::ChangeState()
{
	if ((!hpUI || hpUI->GetIsDead() == true) && state != DEAD) {
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
		if (!isOneShot) {
			status_->objectPtr_->DrawFlag_ = false;
			Wait();
		}
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
				hpUI = nullptr;
				status_ = nullptr;
				enemyAttack = nullptr;
				isDead = true;
				objectPtr_->GetComponent<Collider>()->GetCollider(1)->active_ = false;
			}
		}
		break;
	case IFE::BaseEnemy::TUTO:
		frontVec = { -1,0,0 };
		frontVec.Normalize();
		break;
	default:
		break;
	}
}

void IFE::NormalEnemy::EnemyUpdate()
{
	if (state != DEAD) {
		if (state != WAIT) {
			LookAt();
		}
		if (state != CHASE && state != TUTO) {
			isFound = RaySight(ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetPos());
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

void IFE::NormalEnemy::Wait()
{
	///
	waitTimer += IFE::IFETime::sDeltaTime_;
	if (waitTimer >= WAIT_TIME) {
		waitTimer = 0;
		state = SEARCH;
		ani_->SetAnimation("walk");
	}

	//前フレームに敵を見つけていたなら警戒体制に
	if (isFound == true) {
		state = WARNING;
		/*IFE::Sound::Instance()->SoundPlay("what", false, true);*/
		ani_->SetAnimation("search");
	}
}

void IFE::NormalEnemy::Warning()
{
	//異変の状態が続いたら追跡へ移行
	if (isFound == true) {
		warningTime += IFE::IFETime::sDeltaTime_;
	}
	else {
		warningTime -= IFE::IFETime::sDeltaTime_;
	}

	if (warningTime >= 0.7f) {
		warningTime = 0.5f;
		state = CHASE;
		/*IFE::Sound::Instance()->SoundPlay("found", false, true);*/
		ani_->SetAnimation("Walk");
	}
	if (warningTime <= 0) {
		warningTime = 0.5f;
		state = SEARCH;
		ani_->SetAnimation("walk");
	}
}

void IFE::NormalEnemy::Search()
{
	//ポイント1以上
	if (points.size() > 0) {
		lookfor = points[nextPoint];
		//経由地点を補間(現状ループするだけ)
		Vector3 dirVec = points[nextPoint] - transform_->position_;
		dirVec.Normalize();
		transform_->position_ += (dirVec * SEARCH_VELO * IFE::IFETime::sDeltaTime_);

		//次の地点へ
		double len = sqrt(pow(transform_->position_.x - points[nextPoint].x, 2) + pow(transform_->position_.y - points[nextPoint].y, 2) +
			pow(transform_->position_.z - points[nextPoint].z, 2));

		//これで行けるのか2024
		if (len <= 0.1) {
			if (nextPoint == points.size() - 1) {
				nextPoint = 0;
				ani_->SetAnimation("search");
				state = WAIT;
			}
			else {
				nextPoint++;
				ani_->SetAnimation("search");
				state = WAIT;
			}
		}
	}

	//前フレームに敵を見つけていたなら警戒体制に
	if (isFound == true) {
		state = WARNING;
		/*IFE::Sound::Instance()->SoundPlay("what", false, true);*/
		ani_->SetAnimation("walk");
	}
}

void IFE::NormalEnemy::Chase()
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
			ShotSet();
		}
	}
	if (warningTime >= 2.0f) {
		warningTime = 0.5f;
		state = WARNING;
		/*IFE::Sound::Instance()->SoundPlay("what", false, true);*/
		ani_->SetAnimation("search");
	}
}

void IFE::NormalEnemy::Attack()
{
	attackTime += IFE::IFETime::sDeltaTime_;
	if (attackTime > 0.8f - IFE::IFETime::sDeltaTime_ && attackTime < 0.9f) {
		if (isHit_ == false) {
			isAttack = true;
			IFE::Sound::Instance()->SoundPlay("attack", false, true);
		}
	}
	else {
		isAttack = false;
	}

	if (attackTime > 2.0f) {
		attackTime = 0;
		if (!ChaseLen(IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetPos(),20)) {
			state = CHASE;
		}
		else {
			state = SEARCH;
		}
		ani_->loop_ = true;
		ani_->SetAnimation("walk");
	}
	enemyAttack->objectPtr_->GetComponent<IFE::Collider>()->GetCollider(0)->active_ = isAttack;
}

void IFE::NormalEnemy::Shot()
{
	Vector3 worldPos = enemyAttack->transform_->GetWorldPosition();
	Vector3 dPos = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->GetComponent<PlayerDrone>()->GetPos();
	attackTime += 100 * IFE::IFETime::sDeltaTime_;
	if (enemyAttack->GetIsShot()) {
		if (attackTime < 3) {
			shotVec = dPos - worldPos;
			shotVec.Normalize();
		}
		enemyAttack->transform_->position_ += (shotVec * 0.5f);
	}
	if (enemyAttack->GetIsShot() == false) {
		isAttack = false;
		enemyAttack->objectPtr_->DrawFlag_ = false;
		if (!ChaseLen(IFE::ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->GetComponent<PlayerDrone>()->GetPos(),20)) {
			ShotSet();
		}
		else {
			state = SEARCH;
			enemyAttack->objectPtr_->DrawFlag_ = false;
			ani_->SetAnimation("walk");
		}
	}
	else if (attackTime > 100) {
		enemyAttack->objectPtr_->DrawFlag_ = false;
		enemyAttack->SetIsShot(false);
		isAttack = false;
		attackTime = 0;
		if (!ChaseLen(IFE::ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->GetComponent<PlayerDrone>()->GetPos(),20)) {
			ShotSet();
		}
		else {
			state = SEARCH;
			ani_->SetAnimation("walk");
		}
	}
	enemyAttack->objectPtr_->GetComponent<IFE::Collider>()->GetCollider(0)->active_ = isAttack;
}

void IFE::NormalEnemy::ShotSet()
{
	Vector3 ePos = transform_->position_;
	Vector3 target = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->GetComponent<PlayerDrone>()->GetPos();
	Vector3 addVec = target - ePos;
	addVec.Normalize();
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

void IFE::NormalEnemy::LookAt()
{
	Vector3 ePos = transform_->position_;
	frontVec = lookfor - ePos;
	frontVec = frontVec.Normalize();
	frontVec *= Vector3(1, 0, 1);
	if (state == CHASE || state == SEARCH || state == WARNING) {
		if (!isOneShot) {
			//カメラ方向に合わせてY軸の回転
			float radY = std::atan2(frontVec.x, frontVec.z);
			float targetAngle = ((radY * 180.0f) / (float)PI);
			ApproachTarget(transform_->rotation_.y, targetAngle, 2.0f);
		}
	}
}

bool IFE::NormalEnemy::RaySight(Vector3 pos) {
	//視界の距離
	float maxDistance = 60;
	//視野角
	float sightAngle = 90;
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

void IFE::NormalEnemy::Draw()
{

}

void IFE::NormalEnemy::EnemyOnColliderHit(ColliderCore* myCollider, ColliderCore* hitCollider)
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

const IFE::Vector3 IFE::NormalEnemy::GetPos() {
	Vector3 temp = transform_->position_;
	return temp;
}

void IFE::NormalEnemy::Finalize()
{
	delete hpUI;
	delete enemyAttack;
}

#ifdef EditorMode
#include "ImguiManager.h"
void IFE::NormalEnemy::ComponentDebugGUI()
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

void IFE::NormalEnemy::OutputComponent(nlohmann::json& json)
{
	JsonManager::Instance()->OutputVectorFloat3(json["points"], points);
	json["state"] = state;
}
#endif

void IFE::NormalEnemy::LoadingComponent(nlohmann::json& json)
{
	//if (JsonManager::Instance()->ErrorCheck(json, "points"))
	//{
	//	return;
	//}
	JsonManager::Instance()->InputVectorFloat3(json["points"], points);
	int32_t num = 0;
	JsonManager::Instance()->GetData(json, "state", num);
	state = static_cast<State>(num);
}
