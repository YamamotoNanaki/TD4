#include "Boss.h"
#include "Transform.h"
#include "Collider.h"
#include "ObjectManager.h"
#include "ModelManager.h"
#include "StageCollideManageer.h"
#include "TextureManager.h"
#include "PlayerAction.h"
#include "Scene.h"
#include "PlayerDrone.h"
#include <random>

void IFE::Boss::Initialize()
{
	state = WAIT;
	preState = state;
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
	//�U��
	auto ptr = IFE::ObjectManager::Instance()->AddInitialize("EnemyAttack", ModelManager::Instance()->GetModel("dice"));
	ptr->AddComponent<EnemyAttack>();
	enemyAttack = ptr->GetComponent<EnemyAttack>();
	SetSound();
}

void IFE::Boss::ChangeState()
{
	if (hp_ <= 0 && state != DEAD) {
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
	if (!isOneShot) {
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
				Shot(IFE::ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->GetComponent<PlayerDrone>()->GetPos());
			}
			else {
				if (attackNum == 0) {
					Attack();
				}
				else {
					Shot(ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetPos());
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
					Scene::Instance()->SetNextScene("clear");
				}
			}
			break;
		default:
			break;
		}
	}
}

void IFE::Boss::EnemyUpdate()
{
	if (state != DEAD) {
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
		//hp�\��
		hpUI->Update(transform_->position_, hp_, decHp_);
		status_->IconUpdate(transform_->position_);
		//�d��
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
			rayDist = 0;
			isChaseDrone = false;
		}
	}
}

void IFE::Boss::Wait()
{
	///
	waitTimer += IFE::IFETime::sDeltaTime_;
	if (waitTimer >= WAIT_TIME) {
		waitTimer = 0;
		state = SEARCH;
		ani_->SetAnimation("walk");
	}

	//�O�t���[���ɓG�������Ă����Ȃ�x���̐���
	if (isFound == true) {
		state = WARNING;
		/*IFE::Sound::Instance()->SoundPlay("what", false, true);*/
		ani_->SetAnimation("search");
	}
}

void IFE::Boss::Warning()
{
	//�ٕς̏�Ԃ���������ǐՂֈڍs
	if (isFound == true) {
		warningTime += IFE::IFETime::sDeltaTime_;
	}
	else {
		warningTime -= IFE::IFETime::sDeltaTime_;
	}

	if (warningTime >= 1.0f) {
		std::random_device seed_gen;
		std::mt19937_64 engine(seed_gen());
		std::uniform_real_distribution<float>dist(-1, 1);
		if (dist(engine) > 0) {
			attackNum = 0;
		}
		else {
			attackNum = 1;
		}
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

void IFE::Boss::Search()
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
		}
	}

	//�O�t���[���ɓG�������Ă����Ȃ�x���̐���
	if (isFound == true) {
		state = WARNING;
		/*IFE::Sound::Instance()->SoundPlay("what", false, true);*/
		ani_->SetAnimation("walk");
	}
}

void IFE::Boss::Chase()
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
	if (!isChaseDrone && attackNum == 0) {
		if (len <= 3.0) {
			state = ATTACK;
			enemyAttack->objectPtr_->transform_->position_ = ePos + (addVec * 2);
			enemyAttack->objectPtr_->transform_->scale_ = { 1,1,1 };
			ani_->loop_ = false;
			frontVec = target - ePos;
			frontVec = frontVec.Normalize();
			enemyAttack->SetIsBack(GetBack(0.9f));
			ani_->SetAnimation("knifeAttack");
			float radY = std::atan2(frontVec.x, frontVec.z);
			float targetAngle = ((radY * 180.0f) / (float)PI);
			transform_->rotation_.y = targetAngle;
		}
		if (RaySight(IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetPos()) == false) {
			warningTime += IFE::IFETime::sDeltaTime_;
		}
	}
	else if(isChaseDrone || attackNum == 1) {
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
		if (RaySight(IFE::ObjectManager::Instance()->GetObjectPtr("PlayerDrone")->GetComponent<PlayerDrone>()->GetPos()) == false) {
			warningTime += IFE::IFETime::sDeltaTime_;
		}
	}
	if (warningTime >= 2.0) {
		warningTime = 0.5f;
		state = WARNING;
		/*IFE::Sound::Instance()->SoundPlay("what", false, true);*/
		ani_->SetAnimation("search");
	}
}

void IFE::Boss::Attack()
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
		if (!ChaseLen(IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetPos())) {
			state = WARNING;
		}
		else {
			state = SEARCH;
		}
		ani_->SetAnimation("search");
	}
	enemyAttack->objectPtr_->GetComponent<IFE::Collider>()->GetCollider(0)->active_ = isAttack;
}

void IFE::Boss::Shot(Vector3 target_)
{
	Vector3 worldPos = enemyAttack->transform_->GetWorldPosition();
	Vector3 dPos = target_;
	attackTime += IFE::IFETime::sDeltaTime_;
	if (enemyAttack->GetIsShot()) {
		if (attackTime < 1.0f) {
			shotVec = dPos - worldPos;
			shotVec.Normalize();
			enemyAttack->objectPtr_->DrawFlag_ = false;
		}
		else {
			enemyAttack->objectPtr_->DrawFlag_ = true;
			enemyAttack->transform_->position_ += (shotVec);
		}
	}
	if (enemyAttack->GetIsShot() == false) {
		state = WARNING;
		isAttack = false;
		ani_->SetAnimation("search");
	}
	else if (attackTime > 5.0f) {
		enemyAttack->objectPtr_->DrawFlag_ = false;
		enemyAttack->SetIsShot(false);
		if (!ChaseLen(target_)) {
			state = CHASE;
		}
		else {
			state = SEARCH;
		}
		isAttack = false;
		ani_->SetAnimation("search");
		attackTime = 0;
	}
	enemyAttack->objectPtr_->GetComponent<IFE::Collider>()->GetCollider(0)->active_ = isAttack;
}

void IFE::Boss::LookAt()
{
	Vector3 ePos = transform_->position_;
	frontVec = lookfor - ePos;
	frontVec = frontVec.Normalize();
	frontVec *= Vector3(1, 0, 1);
	if (state == CHASE || state == SEARCH || state == WARNING || isOneShot == false) {
		//�J���������ɍ��킹��Y���̉�]
		float radY = std::atan2(frontVec.x, frontVec.z);
		float targetAngle = ((radY * 180.0f) / (float)PI);
		if (state != CHASE) {
			ApproachTarget(transform_->rotation_.y, targetAngle, 10.0f);
		}
		else {
			ApproachTarget(transform_->rotation_.y, targetAngle, 2.0f);
		}
	}
}

bool IFE::Boss::RaySight(Vector3 pos) {
	//���E�̋���
	float maxDistance = 20;
	//����p
	float sightAngle = 90;
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
	cosHalf *= 10;
	cosHalf = std::floor(cosHalf);

	// ���g�ƃ^�[�Q�b�g�ւ̌����̓��όv�Z
	// �^�[�Q�b�g�ւ̌����x�N�g���𐳋K������K�v�����邱�Ƃɒ���
	float innerProduct = selfDir.Dot(targetDir) / targetDir.Length();
	innerProduct *= 10;
	innerProduct = std::floor(innerProduct);

	// ���E����
	bool inSight = cosHalf <= innerProduct && targetDistance < maxDistance;

	//// ��Q�����Ȃ����ǂ����𔻒�
	if (rayDist < targetDistance && rayDist > 0) {
		// �^�[�Q�b�g������Q�����߂��ꍇ�͎��E���Ղ��Ă���
		inSight = false;
	}

	return inSight;
}

void IFE::Boss::Killed() {
	objectPtr_->GetComponent<Collider>()->GetCollider(1)->SetNoPushBackFlag(true);
	Vector3 pPos = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetPos();
	Vector3 addVec = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetActualFrontVec();
	Vector3 rot = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetRot();
	transform_->position_ = pPos + (addVec);
	transform_->rotation_ = rot;
	status_->objectPtr_->DrawFlag_ = false;
	ani_->SetAnimation("standBy", false);
}

#ifdef EditorMode
void IFE::Boss::ComponentDebugGUI()
{
}
#endif

void IFE::Boss::EnemyOnColliderHit(ColliderCore* myCollider, ColliderCore* hitCollider)
{
	myCollider;
	hitCollider;
}

void IFE::Boss::Draw()
{
}

void IFE::Boss::Finalize()
{
	delete hpUI;
}
