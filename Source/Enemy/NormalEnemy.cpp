#include "NormalEnemy.h"
#include "Transform.h"
#include "Collider.h"

void IFE::NormalEnemy::Initialize()
{
	state = WAIT;
	waitTimer = 0;
}

void IFE::NormalEnemy::Initialize(const Vector3& pos_, const std::vector<Vector3>& points_) {
	state = WAIT;
	waitTimer = 0;
	points = points_;
	nowPoints = 0;
	transform_->position_ = pos_;
}

void IFE::NormalEnemy::ChangeState()
{
	//追跡は最優先
	if (state == CHASE) {

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

		}
	}
}

void IFE::NormalEnemy::Update()
{
	
}

void IFE::NormalEnemy::Move()
{
	//方向ベクトル取って移動
	Vector3 addVec;
	Vector3 ePos = transform_->position_;
	if (nowPoints == points.size()) {
		addVec = points[0] - ePos;
		nowPoints = 0;
	}
	else {
		addVec = points[nowPoints + 1] - ePos;
		nowPoints++;
	}
	addVec.Normalize();
	transform_->position_ += addVec;
}

void IFE::NormalEnemy::Chase()
{
}

void IFE::NormalEnemy::Draw()
{
	
}

void IFE::NormalEnemy::OnColliderHit(IFE::Collider collider)
{
	//相手がplayerだった場合
	if (collider.componentName_ == "Player") {

	}
}

void IFE::NormalEnemy::Finalize()
{
}

void IFE::NormalEnemy::ComponentDebugGUI()
{
}
