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
	//í«ê’ÇÕç≈óDêÊ
	if (state == CHASE) {

	}
	else {
		if (state == WAIT) {
			///
			//é¸ÇËÇå©ìnÇ∑ÇÊÇ§Ç»èàóù
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
	
}

void IFE::NormalEnemy::Draw()
{
	
}

void IFE::NormalEnemy::OnColliderHit(IFE::Collider collider)
{

}

void IFE::NormalEnemy::Finalize()
{
}

void IFE::NormalEnemy::ComponentDebugGUI()
{
}
