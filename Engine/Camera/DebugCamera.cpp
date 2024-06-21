#include "DebugCamera.h"
#ifdef EditorMode
#include "Input.h"
#include "Transform.h"
#include "WindowsAPI.h"
#include "CameraManager.h"

void IFE::DebugCamera::Initialize()
{
	transformCamera_->projectionSize_ = { (float)WindowsAPI::Instance()->winWidth_ ,(float)WindowsAPI::Instance()->winHeight_ };
	//transformCamera_->eyeTargetUpFlag_ = true;
	//transformCamera_->eye_ = { 0,400,0 };
	//transformCamera_->target_ = { 0,0,0 };
	scaleX = 1.0f / transformCamera_->projectionSize_.x;
	scaleY = 1.0f / transformCamera_->projectionSize_.y;
}

void IFE::DebugCamera::Update()
{
	Input* input = Input::Instance();
	static bool flag = false;
	if (input->GetMouseTrigger(Mouse::Middle))
	{
		flag = !flag;
		if (flag)
		{
			Vector3 forward = transformCamera_->target_ - transformCamera_->eye_;
			distance = forward.Length();
			forward.Normalize();

			Vector3 right = Vector3Cross(transformCamera_->up_, forward);
			right.Normalize();

			Vector3 up = Vector3Cross(forward, right);
			up.Normalize();

			Matrix newMatRot(
				right.x, right.y, right.z, 0.0f,
				up.x, up.y, up.z, 0.0f,
				forward.x, forward.y, forward.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
			matRot_ = newMatRot;
		}
	}
	if (flag)
	{
		Float2 move = input->GetMousePointScalar();
		int32_t wheel = input->GetMouseWheel();
		bool dirty = false;
		float angleX = 0;
		float angleY = 0;

		if (input->GetMousePush(Mouse::Left))
		{
			float dy = move.x * scaleY * rotScale / 10;
			float dx = move.y * scaleX * rotScale / 10;

			angleX = -dx * float(M_PI);
			angleY = -dy * float(M_PI);
			dirty = true;
		}

		if (input->GetMousePush(Mouse::Right))
		{
			float dx = move.x / (20.5f - moveScale);
			float dy = move.y / (20.5f - moveScale);

			Vector3 move2 = { -dx, +dy, 0 };
			move2 = Vector3Transform(move2, matRot_);

			Float3 eye = transformCamera_->eye_;
			Float3 target = transformCamera_->target_;

			eye.x += move2.x;
			eye.y += move2.y;
			eye.z += move2.z;

			target.x += move2.x;
			target.y += move2.y;
			target.z += move2.z;

			transformCamera_->eye_ = eye;
			transformCamera_->target_ = target;

			dirty = true;
		}

		if (wheel != 0)
		{
			distance -= wheel / 10.0f;
			distance = distance > 1.0f ? distance : 1.0f;
			dirty = true;
		}

		if (dirty) {
			Matrix newMatRot;
			newMatRot *= MatrixRotationX(-angleX);
			newMatRot *= MatrixRotationY(-angleY);

			matRot_ = newMatRot * matRot_;


			Vector3 newTargetEye = { 0.0f, 0.0f, -distance };
			Vector3 newUp = { 0.0f, 1.0f, 0.0f };

			newTargetEye = Vector3Transform(newTargetEye, matRot_);
			newUp = Vector3Transform(newUp, matRot_);

			const Float3& newTarget = transformCamera_->target_;
			transformCamera_->eye_ = { (newTarget.x + newTargetEye.x), (newTarget.y + newTargetEye.y), newTarget.z + newTargetEye.z };
			transformCamera_->up_ = { newUp.x, newUp.y, newUp.z };
		}
	}
	transformCamera_->Update();
}

#include "ImguiManager.h"

void IFE::DebugCamera::ComponentDebugGUI()
{
	auto im = ImguiManager::Instance();
	im->DragFloatGUI(&moveScale, "moveScale", 0.05f, 0.25f, 20.f);
	im->DragFloatGUI(&rotScale, "rotScale", 0.05f, 0.25f, 20.f);
}

#endif