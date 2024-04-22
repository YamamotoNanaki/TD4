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
	//’ÇÕ‚ÍÅ—Dæ
	if (state == CHASE) {

	}
	else {
		if (state == WAIT) {
			///
			//ü‚è‚ğŒ©“n‚·‚æ‚¤‚Èˆ—
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
	//•ûŒüƒxƒNƒgƒ‹æ‚Á‚ÄˆÚ“®
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
	//‘Šè‚ªplayer‚¾‚Á‚½ê‡
	if (collider.componentName_ == "Player") {

	}
}

void IFE::NormalEnemy::Finalize()
{
}

void IFE::NormalEnemy::ComponentDebugGUI()
{
}
