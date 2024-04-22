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
	p_ = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>();
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
	
}

void IFE::NormalEnemy::Search()
{
	//経由地点を線形補間(たぶんまだちゃんとうごかない)
	transform_->position_ = IFE::LerpFloat3(points[nowPoints], points[nowPoints + 1],150,moveTime);
	if (moveTime >= 150) {
		if (nowPoints == points.size() -1) {
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
	Vector3 addVec = p_->GetObjectPtr()->transform_->position_ - transform_->position_;
	addVec.Normalize();
	transform_->position_ += addVec;
}

void IFE::NormalEnemy::Draw()
{
	
}

void IFE::NormalEnemy::OnColliderHit(IFE::Collider collider)
{
	//相手がplayerだった場合
	if (collider.componentName_ == "Player") {
		//当たった時の処理
	}
}

void IFE::NormalEnemy::Finalize()
{
}

void IFE::NormalEnemy::ComponentDebugGUI()
{
}
