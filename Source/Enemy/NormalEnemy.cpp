#include "NormalEnemy.h"
#include "Transform.h"
#include "Collider.h"
#include "IFETime.h"
#include "ObjectManager.h"
#include "ModelManager.h"
#include "StageCollideManageer.h"

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
	//HPUI
	if (!hp_)
	{
		auto ptr = IFE::ObjectManager::Instance()->AddInitialize("EnemyHp", ModelManager::Instance()->GetModel("hppanel"));
		ptr->AddComponent<EnemyHp>();
		hp_ = ptr->GetComponent<EnemyHp>();
	}
	//攻撃
	auto ptr = IFE::ObjectManager::Instance()->AddInitialize("EnemyAttack", ModelManager::Instance()->GetModel("dice"));
	ptr->AddComponent<EnemyAttack>();
	enemyAttack = ptr->GetComponent<EnemyAttack>();
	enemyAttack->transform_->parent_ = transform_;
	enemyAttack->objectPtr_->transform_->position_ += {0, 0, 2};
}

void IFE::NormalEnemy::ChangeState()
{
	//攻撃は最優先
	if (state == ATTACK) {
		Attack();
	}
	else if (state == CHASE) {
		Chase();
		objectPtr_->SetColor({ 1,0,0,1 });
	}
	else if (state == WARNING) {
		Warning();
		objectPtr_->SetColor({ 0.5f,0.5f,0,1 });
	}
	else {
		if (state == WAIT) {
			Wait();
			objectPtr_->SetColor({ 1,0,0,1 });
		}
		else if (state == SEARCH) {
			Search();
			objectPtr_->SetColor({ 1,0,1,1 });
		}
	}
}

void IFE::NormalEnemy::Update()
{
	if (isFound == false && hitColl_ != nullptr) {
		if (rayDist == 0) {
			rayDist = preRayDist;
			if (hitColl_->objectPtr_->GetComponent<PlayerAction>()) {
				isFound = true;
			}
		}
		else if (rayDist > preRayDist) {
			rayDist = preRayDist;
			if (hitColl_->objectPtr_->GetComponent<PlayerAction>()) {
				isFound = true;
			}
		}
		else if(rayDist <= preRayDist) {

		}
	}
	//状態を取得
	preState = state;
	//前フレームに敵を見つけていたなら警戒体制に
	if (isFound == true && state != WARNING) {
		state = WARNING;
		isFound = false;
	}
	//毎フレーム初期化
	rayDist = 0.0f;
	ChangeState();
	//hp表示
	hp_->Update(transform_->position_);
	//死亡
	if (hp_->GetHp() == 0) {
		hp_->objectPtr_->Destroy();
		objectPtr_->Destroy();
	}
}

void IFE::NormalEnemy::Wait()
{
	///周りを見渡す処理
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
}

void IFE::NormalEnemy::Warning()
{
	//異変の状態が続いたら追跡へ移行
	warningTime++;
	state = preState;

	if (warningTime == 75) {
		warningTime = 0;
		state = CHASE;
	}
}

void IFE::NormalEnemy::Search()
{
	//ポイント1以上
	if (points.size() > 0) {
		LookAt(points[nextPoint]);
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
			}
			else {
				nextPoint++;
				state = WAIT;
			}
		}
	}
}

void IFE::NormalEnemy::Chase()
{
	//とりあえず追いかける
	Vector3 ePos = transform_->position_;
	Vector3 pPos = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetPos();
	//playerの方を向く
	LookAt(pPos);
	Vector3 addVec = pPos - ePos;
	addVec.Normalize();
	transform_->position_ += (addVec * CHASE_VELO * IFE::IFETime::sDeltaTime_);

	//近づいたら殴る
	double len = sqrt(pow(ePos.x - pPos.x, 2) + pow(ePos.y - pPos.y, 2) +
		pow(ePos.z - pPos.z, 2));
	if (len <= 5.0) {
		enemyAttack->objectPtr_->DrawFlag_ = true;
		state = ATTACK;
	}
	warningTime++;
	if (warningTime == 200) {
		warningTime = 0;
		state = SEARCH;
	}
}

void IFE::NormalEnemy::Attack()
{
	enemyAttack->Update();
	attackTime++;
	if (attackTime == 50) {
		attackTime = 0;
		enemyAttack->objectPtr_->DrawFlag_ = false;
		state = CHASE;
	}
}

void IFE::NormalEnemy::LookAt(Vector3 lookfor)
{
	Vector3 ePos = transform_->position_;
	Vector3 frontVec = lookfor - ePos;
	//カメラ方向に合わせてY軸の回転
	float radY = std::atan2(frontVec.x, frontVec.z);
	transform_->eulerAngleDegrees_={ ePos.x,radY * 180.0f / (float)PI,ePos.z };
	//カメラ方向に合わせてX軸の回転
	Vector3 rotaVec = { frontVec.x,0,frontVec.z };
	float length = rotaVec.Length();
	float radX = std::atan2(-frontVec.y, length);
	transform_->eulerAngleDegrees_ = { radX * 180.0f / (float)PI ,radY * 180.0f / (float)PI,0 };
}

void IFE::NormalEnemy::Draw()
{

}

void IFE::NormalEnemy::OnColliderHit(ColliderCore* myCollider, ColliderCore* hitCollider)
{
	//発見
	if (myCollider->GetColliderType() == ColliderType::RAY)
	{
		if (state == SEARCH || state == WAIT) {
			preRayDist = myCollider->rayDistance;
			hitColl_ = hitCollider;
		}
		/*if (state == SEARCH || state == WAIT && hitCollider->objectPtr_->GetComponent<PlayerAction>()) {
			preRayDist = myCollider->rayDistance;
			hitColl_ = hitCollider;
		}*/
	}
	//相手がplayerだった場合
	if (myCollider->GetColliderType() == ColliderType::SPHERE)
	{
		if (hitCollider->objectPtr_->GetComponent<PlayerAttack>()) {
			//当たった時の処理
			hp_->DecHp();
		}
	}
}

IFE::Vector3 IFE::NormalEnemy::GetPos() {
	Vector3 temp = transform_->position_;
	return temp;
}

void IFE::NormalEnemy::Finalize()
{
	delete hp_;
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
