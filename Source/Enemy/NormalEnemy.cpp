#include "NormalEnemy.h"
#include "Transform.h"
#include "Collider.h"
#include "IFETime.h"
#include "ObjectManager.h"
#include "ModelManager.h"
#include "StageCollideManageer.h"
#include "TextureManager.h"

void IFE::NormalEnemy::Initialize()
{
	state = SEARCH;
	preState = state;
	waitTimer = 0;
	nextPoint = 0;
	attackTime = 0;
	rayDist = 0.0f;
	preRayDist = 0.0f;
	isFound = false;
	isAttack = false;
	hp_ = 100;
	decHp_ = 0;
	isHit_ = false;
	hitTime_ = 0;
	objectPtr_->SetColor({ 1, 0, 1, 1 });
	frontVec = { 0,0,0 };
	lookfor = { 0,0,0 };
	//HPUI
	if (!hpUI)
	{
		auto ptr = IFE::ObjectManager::Instance()->AddInitialize("EnemyHp", ModelManager::Instance()->GetModel("hppanel"));
		ptr->AddComponent<EnemyHp>();
		hpUI = ptr->GetComponent<EnemyHp>();
		ptr = IFE::ObjectManager::Instance()->AddInitialize("EnemyHp", ModelManager::Instance()->GetModel("hppanel"));
		ptr->AddComponent<EnemyHp>();
		status_ = ptr->GetComponent<EnemyHp>();
	}

	//�U��
	auto ptr = IFE::ObjectManager::Instance()->AddInitialize("EnemyAttack", ModelManager::Instance()->GetModel("dice"));
	ptr->AddComponent<EnemyAttack>();
	enemyAttack = ptr->GetComponent<EnemyAttack>();
	enemyAttack->transform_->parent_ = transform_;
	enemyAttack->objectPtr_->transform_->position_ = { 0, 0, 2 };
}

void IFE::NormalEnemy::ChangeState()
{
	if (hp_ == 0) {
		state = DEAD;
	}
	//�U���͍ŗD��
	switch (state)
	{
	case IFE::BaseEnemy::WAIT:

		Wait();
		break;
	case IFE::BaseEnemy::SEARCH:

		Search();
		break;
	case IFE::BaseEnemy::WARNING:

		Warning();
		break;
	case IFE::BaseEnemy::CHASE:

		Chase();
		break;
	case IFE::BaseEnemy::ATTACK:

		Attack();
		break;
	case IFE::BaseEnemy::DEAD:
		break;
	default:
		break;
	}
}

void IFE::NormalEnemy::Update()
{
	if (isFound == false && hitColl_ != nullptr) {
		if (rayDist == 0) {
			rayDist = preRayDist;
			isFound = true;
		}
	}
	//��Ԃ��擾
	preState = state;
	ChangeState();
	//hitcool
	if (isHit_ == true) {
		hitTime_--;
		if (hitTime_ == 0) {
			isHit_ = false;
		}
	}
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
		transform_->eulerAngleDegrees_ += (Float3(0, -15, 0) * IFE::IFETime::sDeltaTime_);
	}
	else if (waitTimer < 150) {
		transform_->eulerAngleDegrees_ += (Float3(0, 15, 0) * IFE::IFETime::sDeltaTime_);
	}
	else if (waitTimer < WAIT_TIME) {
		transform_->eulerAngleDegrees_ += (Float3(0, -15, 0) * IFE::IFETime::sDeltaTime_);
	}
	///
	waitTimer++;
	if (waitTimer == WAIT_TIME) {
		waitTimer = 0;
		transform_->rotation_ = { 0,0,0 };
		state = SEARCH;
	}

	//�O�t���[���ɓG�������Ă����Ȃ�x���̐���
	if (isFound == true) {
		objectPtr_->SetColor({ 0.5f, 0.5f, 0, 1 });
		state = WARNING;
		isFound = false;
	}
}

void IFE::NormalEnemy::Warning()
{
	//�ٕς̏�Ԃ���������ǐՂֈڍs
	warningTime++;
	state = preState;

	if (warningTime == 75) {
		warningTime = 0;
		objectPtr_->SetColor({ 0.8f, 0, 0, 1 });
		state = CHASE;
	}
}

void IFE::NormalEnemy::Search()
{
	//�|�C���g1�ȏ�
	if (points.size() > 0) {
		lookfor = points[nextPoint];
		LookAt();
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
			}
			else {
				nextPoint++;
				state = WAIT;
			}
		}
	}

	//�O�t���[���ɓG�������Ă����Ȃ�x���̐���
	if (isFound == true) {
		objectPtr_->SetColor({ 0.5f, 0.5f, 0, 1 });
		state = WARNING;
		isFound = false;
	}
}

void IFE::NormalEnemy::Chase()
{
	//�Ƃ肠�����ǂ�������
	Vector3 ePos = transform_->position_;
	Vector3 pPos = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetPos();
	//player�̕�������
	lookfor = pPos;
	LookAt();
	Vector3 addVec = pPos - ePos;
	addVec.Normalize();
	transform_->position_ += (addVec * CHASE_VELO * IFE::IFETime::sDeltaTime_);

	//�߂Â����牣��
	double len = sqrt(pow(ePos.x - pPos.x, 2) + pow(ePos.y - pPos.y, 2) +
		pow(ePos.z - pPos.z, 2));
	if (len <= 5.0) {
		enemyAttack->objectPtr_->DrawFlag_ = true;
		state = ATTACK;
		enemyAttack->objectPtr_->transform_->position_ = { 0, 0, 2 };
		isAttack = true;
	}
	warningTime++;
	if (warningTime == 200) {
		warningTime = 0;
		//�O�t���[���ɓG�������Ă����Ȃ�x���̐���
		if (isFound == true) {
			objectPtr_->SetColor({ 0.5f, 0.5f, 0, 1 });
			state = WARNING;
			isFound = false;
		}
		else {
			objectPtr_->SetColor({ 0.8f, 0, 1, 1 });
			state = SEARCH;
		}
	}
}

void IFE::NormalEnemy::Attack()
{
	attackTime++;
	if (enemyAttack->objectPtr_->DrawFlag_ == false) {
		isAttack = false;
	}
	if (attackTime == 50) {
		enemyAttack->objectPtr_->DrawFlag_ = false;
		enemyAttack->objectPtr_->transform_->position_ = { 0, -10, 0 };
	}
	else if (attackTime == 150) {
		attackTime = 0;
		state = CHASE;
	}
	enemyAttack->objectPtr_->GetComponent<IFE::Collider>()->GetCollider(0)->active_ = isAttack;
}

void IFE::NormalEnemy::LookAt()
{
	Vector3 ePos = transform_->position_;
	frontVec = lookfor - ePos;
	//�J���������ɍ��킹��Y���̉�]
	float radY = std::atan2(frontVec.x, frontVec.z);
	transform_->eulerAngleDegrees_ = { ePos.x,radY * 180.0f / (float)PI,ePos.z };
	//�J���������ɍ��킹��X���̉�]
	Vector3 rotaVec = { frontVec.x,0,frontVec.z };
	float length = rotaVec.Length();
	float radX = std::atan2(-frontVec.y, length);
	transform_->eulerAngleDegrees_ = { radX * 180.0f / (float)PI ,radY * 180.0f / (float)PI,0 };
}

void IFE::NormalEnemy::DecHp()
{
	if (isHit_ == false) {
		hp_ -= 25;
		decHp_ = 25;
		hitTime_ = HIT_COOLTIME;
		isHit_ = true;
	}
}

void IFE::NormalEnemy::OneShot()
{
	if (isHit_ == false) {
		hp_ -= hp_;
		decHp_ = hp_;
		hitTime_ = HIT_COOLTIME;
		isHit_ = true;
	}
}

void IFE::NormalEnemy::Draw()
{

}

void IFE::NormalEnemy::OnColliderHit(ColliderCore* myCollider, ColliderCore* hitCollider)
{
	//����
	if (myCollider->GetColliderType() == ColliderType::RAY)
	{
		if (state == SEARCH || state == WAIT && hitCollider->objectPtr_->GetComponent<PlayerAction>()) {
			preRayDist = myCollider->rayDistance;
			hitColl_ = hitCollider;
		}
		//�ǂ��������ꍇ
		if (state == WARNING && hitCollider->objectPtr_->GetComponent<StageCollideManageer>()) {
			state = SEARCH;
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
