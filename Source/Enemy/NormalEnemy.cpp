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
}

void IFE::NormalEnemy::Initialize(const Vector3& pos_, const std::vector<Float3>& points_) {
	state = WAIT;
	waitTimer = 0;
	points = points_;
	nowPoints = 0;
	moveTime = 0.f;
	transform_->position_ = pos_;
}

void IFE::NormalEnemy::ChangeState()
{
	//追跡は最優先
	if (state == CHASE) {
		Chase();
	}
	else {
		if (state == WAIT) {
			///
			//周りを見渡すような処理
			///
			waitTimer++;
			if (waitTimer == 150) {
				waitTimer = 0;
				state = SEARCH;
			}
		}
		else if (state == SEARCH) {
			Search();
		}
	}
}

void IFE::NormalEnemy::Update()
{
	Chase();
}

void IFE::NormalEnemy::Search()
{
	//経由地点を線形補間(たぶんまだちゃんとうごかない)
	transform_->position_ = IFE::LerpFloat3(points[nowPoints], points[nowPoints + 1], 150, moveTime);
	if (moveTime >= 150) {
		if (nowPoints == points.size() - 1) {
			nowPoints = 0;
		}
		else {
			nowPoints++;
		}
		moveTime = 0.f;
	}
}

void IFE::NormalEnemy::Chase()
{
	//レイがまだだからとりあえず追いかける
	Vector3 ePos = transform_->position_;
	Vector3 addVec = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetPos() - ePos;
	addVec.Normalize();
	transform_->position_ += (addVec * 0.5f);
}

void IFE::NormalEnemy::Draw()
{

}

//動いてないっぽい
void IFE::NormalEnemy::OnColliderHit(IFE::Collider* collider)
{
	//相手がplayerだった場合
	if (collider->objectPtr_->GetComponent<PlayerAction>()) {
		//当たった時の処理
		componentDeleteFlag_ = true;
		objectPtr_->DrawFlag_ = false;
	}
}

void IFE::NormalEnemy::Finalize()
{
}

#ifdef EditorMode
#include "ImguiManager.h"
void IFE::NormalEnemy::ComponentDebugGUI()
{
	ImguiManager* gui = ImguiManager::Instance();
	gui->DragVectorFloat3GUI(points, "points",transform_->position_);
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
