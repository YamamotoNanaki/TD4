#include "PoseMenu.h"
#include "Input.h"

PoseMenu::PoseMenu()
{
}

PoseMenu::~PoseMenu()
{
}

void PoseMenu::Initialize()
{
}

void PoseMenu::Update()
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

void PoseMenu::Finalize()
{
}

bool PoseMenu::GetPoseFlag()
{
	return poseFlag_;
}
