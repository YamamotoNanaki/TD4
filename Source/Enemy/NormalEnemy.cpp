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
	pointA = 0;
	pointB = pointA + 1;
}

void IFE::NormalEnemy::Initialize(const Vector3& pos_, const std::vector<Float3>& points_) {
	state = WAIT;
	waitTimer = 0;
	points = points_;
	pointA = 0;
	pointB = pointA + 1;
	moveTime = 0.f;
	transform_->position_ = pos_;
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
	/*Chase();*/
	Search();
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
	//�o�R�n�_����(���󃋁[�v���邾��)
	transform_->position_ = IFE::LerpFloat3(points[pointA], points[pointB], LERP_TIME, moveTime);

	//���̒n�_��
	if (moveTime == LERP_TIME) {
		moveTime = 0;
		//�I�_
		if (pointA == points.size() - 2) {
			pointA++;
			pointB = 0;
		}
		//�I�_����n�_��
		else if (pointB == 0) {
			pointA = 0;
			pointB++;
		}
		//����ȊO
		else {
			pointA++;
			pointB++;
		}
	}
	else {
		moveTime++;
	}
}

void IFE::NormalEnemy::Chase()
{
	//���C���܂�������Ƃ肠�����ǂ�������
	Vector3 ePos = transform_->position_;
	Vector3 addVec = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetPos() - ePos;
	addVec.Normalize();
	transform_->position_ += (addVec * 0.5f);
}

void IFE::NormalEnemy::Draw()
{

}

void IFE::NormalEnemy::OnColliderHit(IFE::Collider* collider)
{
	//���肪player�������ꍇ
	if (collider->objectPtr_->GetComponent<PlayerAction>()) {
		//�����������̏���
		/*componentDeleteFlag_ = true;
		objectPtr_->DrawFlag_ = false;*/
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
