#include "DebugCamera.h"
#include "Input.h"
#include "Transform.h"
#include "WindowsAPI.h"

void IFE::DebugCamera::Initialize()
{
	transformCamera_->projectionSize_ = { (float)WindowsAPI::Instance()->winWidth_ ,(float)WindowsAPI::Instance()->winHeight_ };
	//transformCamera_->eyeTargetUpFlag_ = true;
	//transformCamera_->eye_ = { 0,400,0 };
	//transformCamera_->target_ = { 0,0,0 };
}

void IFE::DebugCamera::Update()
{
	//static float scaleX = 1.0f / transformCamera_->projectionSize_.x;
	//static float scaleY = 1.0f / transformCamera_->projectionSize_.y;
	//Input* input = Input::Instance();
	//static bool flag = false;
	//if (input->GetMouseTrigger(Mouse::Middle))flag = !flag;
	//if (flag)
	//{
	//	static Float2 pos;
	//	Float2 oldpos = pos;
	//	pos = input->GetMousePoint();
	//	Float2 move = { oldpos.x - pos.x,oldpos.y - pos.y };
	//	bool dirty = false;
	//	float angleX = 0;
	//	float angleY = 0;

	//	if (input->GetMousePush(Mouse::Left))
	//	{
	//		float dy = move.x * scaleY;
	//		float dx = move.y * scaleX;

	//		angleX = -dx * float(M_PI);
	//		angleY = -dy * float(M_PI);
	//		dirty = true;
	//	}

	//	if (input->GetMousePush(Mouse::Right))
	//	{
	//		float dx = move.x / 20.0f;
	//		float dy = move.y / 20.0f;

	//		Vector3 move2 = { -dx, +dy, 0 };
	//		move2 = Vector3Transform(move2, transformCamera_->matRot_);

	//		Float3 eye = transformCamera_->eye_;
	//		Float3 target = transformCamera_->target_;

	//		eye.x += move2.x;
	//		eye.y += move2.y;
	//		eye.z += move2.z;

	//		target.x += move2.x;
	//		target.y += move2.y;
	//		target.z += move2.z;

	//		transformCamera_->eye_ = eye;
	//		transformCamera_->target_ = target;

	//		dirty = true;
	//	}

	//	if (input->GetMouseWheel() != 0)
	//	{
	//		distance -= input->GetMouseWheel() / 10.0f;
	//		distance = distance > 1.0f ? distance : 1.0f;
	//		dirty = true;
	//	}

	//	if (dirty) {
	//		Matrix newMatRot;
	//		newMatRot *= MatrixRotationX(-angleX);
	//		newMatRot *= MatrixRotationY(-angleY);

	//		transformCamera_->matRot_ = newMatRot * transformCamera_->matRot_;


	//		Vector3 newTargetEye = { 0.0f, 0.0f, -distance };
	//		Vector3 newUp = { 0.0f, 1.0f, 0.0f };

	//		newTargetEye = Vector3Transform(newTargetEye, transformCamera_->matRot_);
	//		newUp = Vector3Transform(newUp, transformCamera_->matRot_);

	//		const Float3& newTarget = transformCamera_->target_;
	//		transformCamera_->eye_ = { newTarget.x + newTargetEye.x, newTarget.y + newTargetEye.y, newTarget.z + newTargetEye.z };
	//		transformCamera_->up_ = { newUp.x, newUp.y, newUp.z };
	//	}
	//}
}
