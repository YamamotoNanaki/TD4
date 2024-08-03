#include "Tutorial.h"
#include "Input.h"
#include"SpriteManager.h"
#include"ColorBuffer.h"
#include"Scene.h"
#include "PlayerAction.h"

void IFE::Tutorial::Initialize()
{
	isShowText = false;
	tutoTime = 0.0f;
	IFE::SpriteManager::Instance()->GetSpritePtr("BlackBack")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("tutoX")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("tutoY")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("R")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("L")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("backAttack")->drawFlag_ = false;
	step = CAMERA;
	isFound = false;
	oldStep = step;
	nowText = "R";
}

void IFE::Tutorial::Update()
{
	switch (step)
	{
	case CAMERA:
		CameraText();
		break;
	case MOVE:
		MoveText();
		break;
	case BACK:
		KillText();
		break;
	case ENEMY:
		KilledText();
		break;
	case DRONE:
		DroneText();
		break;
	case END:
		break;
	case FOUND:
		FoundText();
		break;
	}
	CutInStep();
	//ŽžŠÔŽ~‚ß
	if (isShowText == true) {
		HideUI();
		IFE::IFETime::sTimeScale_ = 0.0f;
	}
	IFE::SpriteManager::Instance()->GetSpritePtr("BlackBack")->drawFlag_ = isShowText;
	
}

void IFE::Tutorial::ChangeStep()
{
}

void IFE::Tutorial::CameraText()
{
	if (tutoTime > 0.7f) {
		isShowText = true;
	}
	else {
		tutoTime += IFE::IFETime::sDeltaTime_;
	}
	IFE::SpriteManager::Instance()->GetSpritePtr(nowText)->drawFlag_ = isShowText;
	NextText("L");
	if (IFE::Input::GetKeyTrigger(IFE::Key::Space) || IFE::Input::PadTrigger(IFE::PADCODE::A)) {
		isShowText = true;
	}
}

void IFE::Tutorial::MoveText()
{
	IFE::SpriteManager::Instance()->GetSpritePtr(nowText)->drawFlag_ = isShowText;
	NextText("tutoX");
}

void IFE::Tutorial::KillText()
{
	if (!isShowText) {
		if (ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetPos().x <= 87) {
			isShowText = true;
		}
	}
	IFE::SpriteManager::Instance()->GetSpritePtr(nowText)->drawFlag_ = isShowText;
	NextText("backAttack");
	if (IFE::Input::GetKeyTrigger(IFE::Key::Space) || IFE::Input::PadTrigger(IFE::PADCODE::A)) {
		isShowText = true;
	}
}

void IFE::Tutorial::KilledText()
{
	IFE::SpriteManager::Instance()->GetSpritePtr(nowText)->drawFlag_ = isShowText;
	NextText("tutoY");
}

void IFE::Tutorial::DroneText()
{
	if (!isShowText) {
		if (ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetPos().x <= 54) {
			isShowText = true;
		}
	}
	IFE::SpriteManager::Instance()->GetSpritePtr(nowText)->drawFlag_ = isShowText;
	if (IFE::Input::GetKeyTrigger(IFE::Key::Space) || IFE::Input::PadTrigger(IFE::PADCODE::A)) {
		isShowText = false;
		IFE::IFETime::sTimeScale_ = 1.0f;
		step = END;
	}
}

void IFE::Tutorial::FoundText()
{
	/*IFE::SpriteManager::Instance()->GetSpritePtr("found")->drawFlag_ = isShowText;*/
	if (IFE::Input::GetKeyTrigger(IFE::Key::Space) || IFE::Input::PadTrigger(IFE::PADCODE::A)) {
		isShowText = false;
		IFE::IFETime::sTimeScale_ = 1.0f;
		step = oldStep;
	}
}

void IFE::Tutorial::CutInStep()
{
	IFE::ObjectManager* objm = IFE::ObjectManager::Instance();
	auto& list = objm->GetObjList();
	//“G‚É”­Œ©‚³‚ê‚é
	for (auto& itr : list) {
		if (itr->GetObjectName().find("normalEnemy") != std::string::npos) {
			if (itr->GetComponent<IFE::NormalEnemy>()->GetIsFound() && !isFound) {
				isShowText = true;
				oldStep = step;
				step = FOUND;
				isFound = true;
			}
		}
	}
}

void IFE::Tutorial::NextText(const std::string& str)
{
	if (isShowText) {
		if (IFE::Input::GetKeyTrigger(IFE::Key::Space) || IFE::Input::PadTrigger(IFE::PADCODE::A)) {
			IFE::SpriteManager::Instance()->GetSpritePtr(nowText)->drawFlag_ = false;
			nowText = str;
			isShowText = false;
			uint32_t tmp = 0;
			tmp = static_cast<uint32_t>(step);
			tmp++;
			step = static_cast<Step>(tmp);
			IFE::IFETime::sTimeScale_ = 1.0f;
			tutoTime = 0.0f;
			Reset();
		}
	}
}

void IFE::Tutorial::HideUI()
{
	SpriteManager::Instance()->GetSpritePtr("RStick")->drawFlag_ = false;
	SpriteManager::Instance()->GetSpritePtr("LStickNormal")->drawFlag_ = false;
	SpriteManager::Instance()->GetSpritePtr("LStickDrone")->drawFlag_ = false;
	SpriteManager::Instance()->GetSpritePtr("Attack")->drawFlag_ = false;
	SpriteManager::Instance()->GetSpritePtr("B")->drawFlag_ = false;
	SpriteManager::Instance()->GetSpritePtr("X")->drawFlag_ = false;
	SpriteManager::Instance()->GetSpritePtr("Y")->drawFlag_ = false;
	SpriteManager::Instance()->GetSpritePtr("Pause")->drawFlag_ = false;
	SpriteManager::Instance()->GetSpritePtr("PauseText")->drawFlag_ = false;
	SpriteManager::Instance()->GetSpritePtr("CameraMove")->drawFlag_ = false;
	SpriteManager::Instance()->GetSpritePtr("CharaMove")->drawFlag_ = false;
	SpriteManager::Instance()->GetSpritePtr("ModeChangeNormal")->drawFlag_ = false;
	SpriteManager::Instance()->GetSpritePtr("Sneak")->drawFlag_ = false;
}

void IFE::Tutorial::Finalize()
{
}

void IFE::Tutorial::Reset()
{
	SpriteManager::Instance()->GetSpritePtr("RStick")->drawFlag_ = true;
	SpriteManager::Instance()->GetSpritePtr("LStickNormal")->drawFlag_ = true;
	SpriteManager::Instance()->GetSpritePtr("LStickDrone")->drawFlag_ = true;
	SpriteManager::Instance()->GetSpritePtr("Attack")->drawFlag_ = true;
	SpriteManager::Instance()->GetSpritePtr("B")->drawFlag_ = true;
	SpriteManager::Instance()->GetSpritePtr("X")->drawFlag_ = true;
	SpriteManager::Instance()->GetSpritePtr("Y")->drawFlag_ = true;
	SpriteManager::Instance()->GetSpritePtr("Pause")->drawFlag_ = true;
	SpriteManager::Instance()->GetSpritePtr("PauseText")->drawFlag_ = true;
	SpriteManager::Instance()->GetSpritePtr("CameraMove")->drawFlag_ = true;
	SpriteManager::Instance()->GetSpritePtr("CharaMove")->drawFlag_ = true;
	SpriteManager::Instance()->GetSpritePtr("ModeChangeNormal")->drawFlag_ = true;
	SpriteManager::Instance()->GetSpritePtr("Sneak")->drawFlag_ = true;
}
