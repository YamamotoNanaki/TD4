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

void IFE::NormalEnemy::Initialize()
{
	state = SEARCH;
	preState = state;
	waitTimer = 0;
	nextPoint = 0;
	attackTime = 0;
	rayDist = 0.0f;
	isFound = false;
	isAttack = false;
	warningTime = 50;
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
	//�U��
	auto ptr = IFE::ObjectManager::Instance()->AddInitialize("EnemyAttack", ModelManager::Instance()->GetModel("dice"));
	ptr->AddComponent<EnemyAttack>();
	enemyAttack = ptr->GetComponent<EnemyAttack>();
}

void IFE::NormalEnemy::ChangeState()
{
	if (hp_ == 0) {
		state = DEAD;
		ani_->SetAnimation("downFront");
	}
	else if (hp_ > 0) {
		//�U���͍ŗD��
		switch (state)
		{
		case IFE::BaseEnemy::WAIT:
			status_->objectPtr_->DrawFlag_ = false;
			Wait();
			break;
		case IFE::BaseEnemy::SEARCH:
			status_->objectPtr_->DrawFlag_ = false;
			Search();
			break;
		case IFE::BaseEnemy::WARNING:
			//�x����Ԃ�UI�ɕύX
			status_->objectPtr_->GetComponent<Material>()->SetTexture(TextureManager::Instance()->GetTexture("eye"));
			status_->objectPtr_->DrawFlag_ = true;
			Warning();
			break;
		case IFE::BaseEnemy::CHASE:
			//�ǐՏ�Ԃ�UI�ɕύX
			status_->objectPtr_->GetComponent<Material>()->SetTexture(TextureManager::Instance()->GetTexture("exclamation"));
			status_->objectPtr_->DrawFlag_ = true;
			Chase();
			break;
		case IFE::BaseEnemy::ATTACK:
			if (isChaseDrone == true) {
				Shot();
			}
			else {
				Attack();
			}
			break;
		case IFE::BaseEnemy::DEAD:
			break;
		default:
			break;
		}
	}
}

void IFE::NormalEnemy::EnemyUpdate()
{
	if (state != WAIT) {
		LookAt();
	}
	isFound = RaySight(ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetPos());
	if (isFound == false && IFE::ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->GetComponent<PlayerDrone>()->GetIsDroneSurvival() == true) {
		isFound = RaySight(IFE::ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->GetComponent<PlayerDrone>()->GetPos());
		isChaseDrone = isFound;
	}
	//��Ԃ��擾
	preState = state;
	ChangeState();
	//hp�\��
	hpUI->Update(transform_->position_, hp_, decHp_);
	status_->IconUpdate(transform_->position_);
	//���S
	if (hpUI->GetIsDead() == true) {
		hpUI->objectPtr_->Destroy();
		status_->objectPtr_->Destroy();
		enemyAttack->objectPtr_->Destroy();
		objectPtr_->Destroy();
	}
	rayDist = 0;
	isChaseDrone = false;
	//�d��
	if (!objectPtr_->GetComponent<Collider>()->GetCollider(1)->onGround_)
	{
		transform_->position_.y -= 4.9f * IFETime::sDeltaTime_;
	}
}

void IFE::NormalEnemy::Wait()
{
	///��������n������
	if (waitTimer < 50) {
		transform_->rotation_ += (Float3(0, -15, 0) * IFE::IFETime::sDeltaTime_);
	}
	else if (waitTimer < 150) {
		transform_->rotation_ += (Float3(0, 15, 0) * IFE::IFETime::sDeltaTime_);
	}
	else if (waitTimer < WAIT_TIME) {
		transform_->rotation_ += (Float3(0, -15, 0) * IFE::IFETime::sDeltaTime_);
	}
	///
	waitTimer += 50 * IFE::IFETime::sDeltaTime_;
	if (waitTimer >= WAIT_TIME) {
		waitTimer = 0;
		transform_->rotation_ = { 0,0,0 };
		state = SEARCH;
		ani_->SetAnimation("walk");
	}

	//�O�t���[���ɓG�������Ă����Ȃ�x���̐���
	if (isFound == true) {
		state = WARNING;
		ani_->SetAnimation("search");
	}
}

void IFE::NormalEnemy::Warning()
{
	//�ٕς̏�Ԃ���������ǐՂֈڍs
	if (isFound == true) {
		warningTime += 100 * IFE::IFETime::sDeltaTime_;
	}
	else {
		warningTime -= 100 * IFE::IFETime::sDeltaTime_;
	}

	if (warningTime >= 125) {
		warningTime = 50;
		state = CHASE;
		ani_->SetAnimation("Walk");
	}
	if (warningTime <= 0) {
		warningTime = 50;
		state = SEARCH;
		ani_->SetAnimation("walk");
	}
}

void IFE::NormalEnemy::Search()
{
	//�|�C���g1�ȏ�
	if (points.size() > 0) {
		lookfor = points[nextPoint];
		//�o�R�n�_����(���󃋁[�v���邾��)
		Vector3 dirVec = points[nextPoint] - transform_->position_;
		dirVec.Normalize();
		transform_->position_ += (dirVec * SEARCH_VELO * IFE::IFETime::sDeltaTime_);

		//���̒n�_��
		double len = sqrt(pow(transform_->position_.x - points[nextPoint].x, 2) + pow(transform_->position_.y - points[nextPoint].y, 2) +
			pow(transform_->position_.z - points[nextPoint].z, 2));

		//����ōs����̂�2024
		if (len <= 0.1) {
			if (nextPoint == points.size() - 1) {
				nextPoint = 0;
				state = WAIT;
			}
			else {
				nextPoint++;
				state = WAIT;
			}
			if (ani_ != nullptr) {
				ani_->SetAnimation("search");
			}
		}
	}

	//�O�t���[���ɓG�������Ă����Ȃ�x���̐���
	if (isFound == true) {
		state = WARNING;
		ani_->SetAnimation("search");
	}
}

void IFE::NormalEnemy::Chase()
{
	//�Ƃ肠�����ǂ�������
	Vector3 ePos = transform_->position_;
	Vector3 target;
	if (isChaseDrone == false) {
		target = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetPos();
	}
	else {
		target = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->GetComponent<PlayerDrone>()->GetPos();
	}
	//player�̕�������
	lookfor = target;
	Vector3 addVec = target - ePos;
	addVec.Normalize();
	transform_->position_ += (addVec * CHASE_VELO * IFE::IFETime::sDeltaTime_);

	//�߂Â����牣��
	double len = sqrt(pow(ePos.x - target.x, 2) + pow(ePos.y - target.y, 2) +
		pow(ePos.z - target.z, 2));
	if (isChaseDrone == false) {
		if (len <= 3.0) {
			enemyAttack->objectPtr_->DrawFlag_ = true;
			state = ATTACK;
			enemyAttack->objectPtr_->transform_->position_ = ePos + (addVec * 2);
			isAttack = true;
			enemyAttack->objectPtr_->transform_->scale_ = { 1,1,1 };
			IFE::Sound::Instance()->SoundPlay("attack", false, true);
			ani_->SetAnimation("gunAttack");
		}
		if (RaySight(IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetPos()) == false) {
			warningTime += 100 * IFE::IFETime::sDeltaTime_;
		}
	}
	else {
		if (len <= 18.0) {
			enemyAttack->objectPtr_->DrawFlag_ = true;
			state = ATTACK;
			enemyAttack->objectPtr_->transform_->position_ = ePos + (addVec * 2);
			isAttack = true;
			enemyAttack->SetIsShot(true);
			enemyAttack->objectPtr_->transform_->scale_ = { 0.4f,0.4f,0.4f };
			IFE::Sound::Instance()->SoundPlay("gun", false, true);
			ani_->SetAnimation("gunAttack");
		}
		if (RaySight(IFE::ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->GetComponent<PlayerDrone>()->GetPos()) == false) {
			warningTime += 100 * IFE::IFETime::sDeltaTime_;
		}
	}
	if (warningTime >= 60) {
		warningTime = 50;
		state = WARNING;
		ani_->SetAnimation("search");
	}
}

void IFE::NormalEnemy::Attack()
{
	attackTime += 100 * IFE::IFETime::sDeltaTime_;
	if (enemyAttack->objectPtr_->DrawFlag_ == false) {
		isAttack = false;
	}
	if (attackTime >= 50 && attackTime < 150) {
		enemyAttack->objectPtr_->DrawFlag_ = false;
		enemyAttack->objectPtr_->transform_->position_ = { 0, -10, 0 };
	}
	else if (attackTime >= 150) {
		attackTime = 0;
		state = CHASE;
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
		enemyAttack->transform_->position_ += (shotVec * 0.3f);
	}
	if(enemyAttack->GetIsShot() == false) {
		state = CHASE;
		isAttack = false;
		ani_->SetAnimation("walk");
	}
	else if (attackTime > 100) {
		state = CHASE;
		isAttack = false;
		ani_->SetAnimation("walk");
		enemyAttack->objectPtr_->DrawFlag_ = false;
		enemyAttack->SetIsShot(false);
		attackTime = 0;
	}
	enemyAttack->objectPtr_->GetComponent<IFE::Collider>()->GetCollider(0)->active_ = isAttack;
}

void IFE::NormalEnemy::LookAt()
{
	Vector3 ePos = transform_->position_;
	frontVec = lookfor - ePos;
	frontVec = frontVec.Normalize();
	frontVec *= Vector3(1, 0, 1);
	//�J���������ɍ��킹��Y���̉�]
	float radY = std::atan2(frontVec.x, frontVec.z);
	transform_->rotation_.y = ((radY * 180.0f) / (float)PI ) + 180.0f;
}

bool IFE::NormalEnemy::RaySight(Vector3 pos) {
	//���E�̋���
	float maxDistance = 20;
	//����p
	float sightAngle = 45;
	// ���g�̈ʒu
	Vector3 ePos = transform_->position_;
	// �^�[�Q�b�g�̈ʒu
	Vector3 targetPos = pos;
	objectPtr_->GetComponent<Collider>()->GetCollider(2)->rayDir_ = Vector3(pos - ePos).Normalize();

	//���̐��ʃx�N�g��
	Vector3 selfDir = frontVec;

	// �^�[�Q�b�g�܂ł̌����Ƌ����v�Z
	Vector3 targetDir = targetPos - ePos;
	float targetDistance = targetDir.Length();
	targetDir.Normalize();

	// cos(��/2)���v�Z
	float cosHalf = cos(ConvertToRadians(sightAngle / 2.0f * (float)PI / 180.0f));
	cosHalf = std::floor(cosHalf * 0.1f);

	// ���g�ƃ^�[�Q�b�g�ւ̌����̓��όv�Z
	// �^�[�Q�b�g�ւ̌����x�N�g���𐳋K������K�v�����邱�Ƃɒ���
	float innerProduct = selfDir.Dot(targetDir) / targetDir.Length();
	innerProduct = std::floor(innerProduct * 0.1f);

	// ���E����
	bool inSight = cosHalf <= innerProduct && targetDistance < maxDistance;

	//// ��Q�����Ȃ����ǂ����𔻒�
	if (rayDist < targetDistance && rayDist > 0) {
		// �^�[�Q�b�g������Q�����߂��ꍇ�͎��E���Ղ��Ă���
		inSight = false;
	}

	return inSight;
}

void IFE::NormalEnemy::Draw()
{

}

void IFE::NormalEnemy::EnemyOnColliderHit(ColliderCore* myCollider, ColliderCore* hitCollider)
{
	//�ǂ��������ꍇ
	if (myCollider->GetColliderType() == ColliderType::RAY) {
		if (hitCollider->objectPtr_->GetObjectName().find("wall") != std::string::npos|| hitCollider->objectPtr_->GetObjectName().find("box") != std::string::npos) {
			if (rayDist == 0) {
				rayDist = myCollider->rayDistance;
			}
			else if (rayDist > myCollider->rayDistance) {
				rayDist = myCollider->rayDistance;
			}
		}
	}
}

IFE::Vector3 IFE::NormalEnemy::GetPos() {
	Vector3 temp = transform_->position_;
	return temp;
}

bool IFE::NormalEnemy::GetBack()
{
	Vector3 pFront = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetFrontVec();
	float result = pFront.Dot(frontVec);
	//+�Ȃ���
	if (result > 0) {
		return true;
	}
	return false;
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
}

void IFE::NormalEnemy::OutputComponent(nlohmann::json& json)
{
	JsonManager::Instance()->OutputVectorFloat3(json["points"], points);
}
#endif

void IFE::NormalEnemy::LoadingComponent(nlohmann::json& json)
{
	//if (JsonManager::Instance()->ErrorCheck(json, "points"))
	//{
	//	return;
	//}
	JsonManager::Instance()->InputVectorFloat3(json["points"], points);
}
