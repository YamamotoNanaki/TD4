#include "Tutorial.h"
#include "Input.h"
#include"SpriteManager.h"
#include"ColorBuffer.h"
#include"Scene.h"
#include "PlayerAction.h"

void IFE::Tutorial::Initialize()
{
	isShowText = true;
	IFE::SpriteManager::Instance()->GetSpritePtr("BlackBack")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("tutoX")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("tutoY")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("R")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("L")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("backAttack")->drawFlag_ = false;
	step = CAMERA;
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
	}
	//ŽžŠÔŽ~‚ß
	if (isShowText == true) {
		IFE::IFETime::sTimeScale_ = 0.0f;
	}
	IFE::SpriteManager::Instance()->GetSpritePtr("BlackBack")->drawFlag_ = isShowText;
}

void IFE::Tutorial::CameraText()
{
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
		if (ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetPos().x <= 95) {
			isShowText = true;
		}
	}
	IFE::SpriteManager::Instance()->GetSpritePtr(nowText)->drawFlag_ = isShowText;
	NextText("backAttack");
}

void IFE::Tutorial::KilledText()
{
	IFE::SpriteManager::Instance()->GetSpritePtr(nowText)->drawFlag_ = isShowText;
	NextText("tutoY");
}

void IFE::Tutorial::DroneText()
{
	IFE::SpriteManager::Instance()->GetSpritePtr(nowText)->drawFlag_ = isShowText;
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
		}
	}
}

void IFE::Tutorial::Finalize()
{
}

void IFE::Tutorial::Reset()
{
}
