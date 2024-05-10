#include "NormalEnemy.h"
#include "Transform.h"
#include "Collider.h"
#include "Ease.h"
#include "IFETime.h"
#include "ObjectManager.h"

void IFE::NormalEnemy::Initialize()
{
	state = WAIT;
	waitTimer = 0;
	nextPoint = 0;
	isAttack = false;
	//std::vector<Collider*>colliders = objectPtr_->GetComponents<Collider>();
	////���C(����)
	//for (size_t i = 0; i < colliders.size(); i++) {
	//	colliders[0]->SetColliderType(ColliderType::SPHERE);
	//	colliders[1]->SetColliderType(ColliderType::RAY);
	//	if (colliders[i]->GetColliderType() == ColliderType::RAY) {
	//		colliders[i]->rayDir_ = { 0,0,-1 };
	//	}
	//}
}

void IFE::NormalEnemy::ChangeState()
{
	//�ǐՂ͍ŗD��
	if (state == CHASE) {
		Chase();
	}
	else if (state == WARNING) {
		Warning();
	}
	else {
		if (state == WAIT) {
			Wait();
		}
		else if (state == SEARCH) {
			Search();
		}
	}
}

void IFE::NormalEnemy::Update()
{
	std::vector<Collider*>colliders = objectPtr_->GetComponents<Collider>();
	ChangeState();
}

void IFE::NormalEnemy::Wait()
{
	///��������n������
	if (waitTimer < 50) {
		transform_->eulerAngleDegrees_ += { 0, -1.5f, 0 };
	}
	else if (waitTimer < 150) {
		transform_->eulerAngleDegrees_ += { 0, 1.5f, 0 };
	}
	else if (waitTimer < WAIT_TIME) {
		transform_->eulerAngleDegrees_ += { 0, -1.5f, 0 };
	}
	///
	waitTimer++;
	if (waitTimer == WAIT_TIME) {
		waitTimer = 0;
		transform_->rotation_ = { 0,0,0 };
		state = SEARCH;
	}
}

void IFE::NormalEnemy::Warning()
{
	//�ٕς̏�Ԃ���������ǐՂֈڍs
	state = CHASE;
}

void IFE::NormalEnemy::Search()
{
	//�|�C���g1�ȏ�
	if (points.size() > 0) {
		//�o�R�n�_����(���󃋁[�v���邾��)
		Vector3 dirVec = points[nextPoint] - transform_->position_;
		dirVec.Normalize();
		transform_->position_ += (dirVec * MOVE_VELO);

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
			}
		}
	}
}

void IFE::NormalEnemy::Chase()
{
	//���C���܂�������Ƃ肠�����ǂ�������
	Vector3 ePos = transform_->position_;
	Vector3 pPos = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetPos();
	Vector3 addVec = pPos - ePos;
	addVec.Normalize();
	transform_->position_ += (addVec * 0.1f);

	//�߂Â����牣��
	double len = sqrt(pow(ePos.x - pPos.x, 2) + pow(ePos.y - pPos.y, 2) +
		pow(ePos.z - pPos.z, 2));
	if (len <= 5.0) {
		Attack();
	}
}

void IFE::NormalEnemy::Attack()
{
	if (isAttack == false) {
		isAttack = true;
	}
}

void IFE::NormalEnemy::Draw()
{

}

void IFE::NormalEnemy::OnColliderHit(ColliderCore* myCollider, ColliderCore* hitCollider)
{
	if (myCollider->GetColliderType() == ColliderType::RAY)
	{
		if (state != CHASE) {
			state = CHASE;
			objectPtr_->SetColor({1,0,0,1});
		}
	}
	//���肪player�������ꍇ
	if (myCollider->GetColliderType() == ColliderType::SPHERE)
	{
		if (hitCollider->objectPtr_->GetComponent<PlayerAction>()) {
			//�����������̏���
			objectPtr_->Destroy();
		}
	}
	//�G�ƓG
	if (myCollider->GetColliderType() == ColliderType::SPHERE)
	{
		if (hitCollider->objectPtr_->GetComponent<NormalEnemy>()) {
			Vector3 ePos = transform_->position_;
			Vector3 hitPos = hitCollider->objectPtr_->GetComponent<NormalEnemy>()->GetPos();
			Vector3 addVec = hitPos - ePos;
			addVec.Normalize();
			addVec *= Vector3(-1, 0, -1);
			transform_->position_+= (addVec * 0.1f);
		}
	}
}

IFE::Vector3 IFE::NormalEnemy::GetPos() {
	Vector3 temp = transform_->position_;
	return temp;
}

void IFE::NormalEnemy::Finalize()
{
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
