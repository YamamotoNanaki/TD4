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
	std::vector<Collider*>colliders = objectPtr_->GetComponents<Collider>();
	//レイ(視線)
	for (size_t i = 0; i < colliders.size(); i++) {
		colliders[0]->SetColliderType(ColliderType::SPHERE);
		colliders[1]->SetColliderType(ColliderType::RAY);
		if (colliders[i]->GetColliderType() == ColliderType::RAY) {
			colliders[i]->rayDir_ = { 0,0,-1 };
		}
	}
}

void IFE::NormalEnemy::ChangeState()
{
	//追跡は最優先
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
	///周りを見渡す処理
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
	//異変の状態が続いたら追跡へ移行
	state = CHASE;
}

void IFE::NormalEnemy::Search()
{
	//ポイント1以上
	if (points.size() > 0) {
		//経由地点を補間(現状ループするだけ)
		Vector3 dirVec = points[nextPoint] - transform_->position_;
		dirVec.Normalize();
		transform_->position_ += (dirVec * MOVE_VELO);

		//次の地点へ
		double len = sqrt(pow(transform_->position_.x - points[nextPoint].x, 2) + pow(transform_->position_.y - points[nextPoint].y, 2) +
			pow(transform_->position_.z - points[nextPoint].z, 2));

		//これで行けるのか2024
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
	//レイがまだだからとりあえず追いかける
	Vector3 ePos = transform_->position_;
	Vector3 pPos = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetPos();
	Vector3 addVec = pPos - ePos;
	addVec.Normalize();
	transform_->position_ += (addVec * 0.1f);

	//近づいたら殴る
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

void IFE::NormalEnemy::OnColliderHit(Collider* myCollider, Collider* hitCollider)
{
	if (myCollider->GetColliderType() == ColliderType::RAY)
	{
		if (state != CHASE) {
			state = CHASE;
			objectPtr_->SetColor({1,0,0,1});
		}
	}
	//相手がplayerだった場合
	if (myCollider->GetColliderType() == ColliderType::SPHERE)
	{
		if (hitCollider->objectPtr_->GetComponent<PlayerAction>()) {
			//当たった時の処理
			objectPtr_->Destroy();
		}
	}
	//敵と敵
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
