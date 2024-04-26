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
	//�ǐՂ͍ŗD��
	if (state == CHASE) {
		Chase();
	}
	else {
		if (state == WAIT) {
			///
			//��������n���悤�ȏ���
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
	//�o�R�n�_����`���(���Ԃ�܂������Ƃ������Ȃ�)
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
	//���C���܂�������Ƃ肠�����ǂ�������
	Vector3 ePos = transform_->position_;
	Vector3 addVec = IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetPos() - ePos;
	addVec.Normalize();
	transform_->position_ += (addVec * 0.5f);
}

void IFE::NormalEnemy::Draw()
{

}

//�����ĂȂ����ۂ�
void IFE::NormalEnemy::OnColliderHit(IFE::Collider* collider)
{
	//���肪player�������ꍇ
	if (collider->objectPtr_->GetComponent<PlayerAction>()) {
		//�����������̏���
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
