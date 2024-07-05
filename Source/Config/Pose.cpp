#include "Pose.h"
#include "Input.h"

void Pose::Initialize()
{
}

void Pose::Update()
{
	if (IFE::Input::GetKeyTrigger(IFE::Key::Esc) || IFE::Input::PadTrigger(IFE::PADCODE::START))
	{
		poseFlag_ = !poseFlag_;
	}

	if (poseFlag_ == true)
	{
		IFE::IFETime::sTimeScale_ = 0.0f;
	}
	else
	{
		IFE::IFETime::sTimeScale_ = 1.0f;
	}
}