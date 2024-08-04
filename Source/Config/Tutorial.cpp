#include "Tutorial.h"
#include "Input.h"
#include"SpriteManager.h"
#include"ColorBuffer.h"
#include"Scene.h"
#include "PlayerAction.h"
#include "Player.h"
#include "Scene.h"
#include "PoseMenu.h"

void IFE::Tutorial::Initialize()
{
	isShowText = false;
	tutoTime = 0.0f;
	bottanTime = 0.0f;
	textShowTime = MAX_SHOWTIME;
	IFE::SpriteManager::Instance()->GetSpritePtr("BlackBack")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("decide")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("decidePop")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("tutoX")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("tutoY")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("R")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("L")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("found")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("scanning")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("comeback")->drawFlag_ = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("backAttack")->drawFlag_ = false;
	step = CAMERA;
	isFound = false;
	oldStep = step;
	nowText = "R";
	if (IFE::Scene::Instance()->GetSceneName() == "stage02") {
		isPlayTutorial = true;
	}
}

void IFE::Tutorial::Update()
{
	if (IFE::SpriteManager::Instance()->GetSpritePtr("Pause")->GetComponent<PoseMenu>()->GetPoseFlag() == false)
	{
		if (isPlayTutorial) {
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
			case SCAN:
				ScanText();
				break;
			case COMEBACK:
				ComeBackText();
				break;
			case END:
				break;
			case FOUND:
				FoundText();
				break;
			}
			CutInStep();
			StopTime();
			IFE::SpriteManager::Instance()->GetSpritePtr("BlackBack")->drawFlag_ = isShowText;
			IFE::SpriteManager::Instance()->GetSpritePtr("decide")->drawFlag_ = isShowText;
		}
	}

	if (oldPauseFlag_ == false && IFE::SpriteManager::Instance()->GetSpritePtr("Pause")->GetComponent<PoseMenu>()->GetPoseFlag() == true)
	{
		HideText();
	}
	if (oldPauseFlag_ == true && IFE::SpriteManager::Instance()->GetSpritePtr("Pause")->GetComponent<PoseMenu>()->GetPoseFlag() == false)
	{
		if (isShowText) {
			ShowText();
		  }
	}
	oldPauseFlag_ = IFE::SpriteManager::Instance()->GetSpritePtr("Pause")->GetComponent<PoseMenu>()->GetPoseFlag();
}

void IFE::Tutorial::StopTime()
{
	//ŽžŠÔŽ~‚ß
	if (isShowText == true) {
		HideUI();
		IFE::IFETime::sTimeScale_ = 0.0f;
		bottanTime++;
		if (bottanTime >= 40) {
			isUp = !isUp;
			bottanTime = 0;
		}
		IFE::SpriteManager::Instance()->GetSpritePtr("decidePop")->drawFlag_ = isUp;
	}
	else {
		IFE::SpriteManager::Instance()->GetSpritePtr("decidePop")->drawFlag_ = false;
	}
}

void IFE::Tutorial::CameraText()
{
	if (tutoTime > 0.7f) {
		isShowText = true;
		NextText("L");
		if (IFE::Input::GetKeyTrigger(IFE::Key::Space) || IFE::Input::PadTrigger(IFE::PADCODE::A)) {
			isShowText = true;
		}
	}
	else {
		tutoTime += IFE::IFETime::sDeltaTime_;
	}
	IFE::SpriteManager::Instance()->GetSpritePtr(nowText)->drawFlag_ = isShowText;
}

void IFE::Tutorial::MoveText()
{
	IFE::SpriteManager::Instance()->GetSpritePtr(nowText)->drawFlag_ = isShowText;
	NextText("tutoX");
}

void IFE::Tutorial::KillText()
{
	if (ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetPos().x <= 87) {
		isShowText = true;
		NextText("backAttack");
		if (nowText == "backAttack") {
			isShowText = true;
		}
	}
	IFE::SpriteManager::Instance()->GetSpritePtr(nowText)->drawFlag_ = isShowText;
}

void IFE::Tutorial::KilledText()
{
	IFE::SpriteManager::Instance()->GetSpritePtr(nowText)->drawFlag_ = isShowText;
	NextText("tutoY");
}

void IFE::Tutorial::DroneText()
{
	if (ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->GetPos().x <= 54) {
		isShowText = true;
		NextText("scanning");
	}
	IFE::SpriteManager::Instance()->GetSpritePtr(nowText)->drawFlag_ = isShowText;
}

void IFE::Tutorial::ScanText()
{
	if (ObjectManager::Instance()->GetObjectPtr("playerObject")->GetComponent<Player>()->GetFirstDrone()) {
		isShowText = true;
		NextText("comeback");
	}
	IFE::SpriteManager::Instance()->GetSpritePtr(nowText)->drawFlag_ = isShowText;
}

void IFE::Tutorial::ComeBackText()
{
	if (ObjectManager::Instance()->GetObjectPtr("playerObject")->GetComponent<Player>()->GetFirstRecovery()) {
		isShowText = true;
		if (IFE::Input::GetKeyTrigger(IFE::Key::Space) || IFE::Input::PadTrigger(IFE::PADCODE::A)) {
			IFE::SpriteManager::Instance()->GetSpritePtr(nowText)->drawFlag_ = false;
			isShowText = false;
			IFE::IFETime::sTimeScale_ = 1.0f;
			step = END;
			Reset();
		}
	}
	IFE::SpriteManager::Instance()->GetSpritePtr(nowText)->drawFlag_ = isShowText;
}

void IFE::Tutorial::FoundText()
{
	IFE::SpriteManager::Instance()->GetSpritePtr("found")->drawFlag_ = true;
	if (IFE::Input::GetKeyTrigger(IFE::Key::Space) || IFE::Input::PadTrigger(IFE::PADCODE::A)) {
		IFE::SpriteManager::Instance()->GetSpritePtr("found")->drawFlag_ = false;
		isShowText = false;
		IFE::IFETime::sTimeScale_ = 1.0f;
		step = oldStep;
		Reset();
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
	if (textShowTime > 0) {
		textShowTime--;
	}
	if (isShowText && textShowTime <= 0) {
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
			textShowTime = MAX_SHOWTIME;
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

void IFE::Tutorial::HideText()
{
	if (isShowText)
	{
		IFE::SpriteManager::Instance()->GetSpritePtr("decide")->drawFlag_ = false;
		IFE::SpriteManager::Instance()->GetSpritePtr(nowText)->drawFlag_ = false;
	}
}

void IFE::Tutorial::ShowText()
{
	IFE::SpriteManager::Instance()->GetSpritePtr("decide")->drawFlag_ = true;
	IFE::SpriteManager::Instance()->GetSpritePtr(nowText)->drawFlag_ = true;
}

void IFE::Tutorial::Finalize()
{
}

void IFE::Tutorial::Reset()
{
	SpriteManager::Instance()->GetSpritePtr("RStick")->drawFlag_ = true;
	SpriteManager::Instance()->GetSpritePtr("LStickNormal")->drawFlag_ = true;
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

#ifdef EditorMode
#include "ImguiManager.h"
void IFE::Tutorial::ComponentDebugGUI()
{
}

void IFE::Tutorial::OutputComponent(nlohmann::json& json)
{
	json;
}
#endif

void IFE::Tutorial::LoadingComponent(nlohmann::json& json)
{
	json;
}