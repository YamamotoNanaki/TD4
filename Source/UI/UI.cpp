#include "UI.h"
using namespace IFE;

void UI::Initialize()
{
	pause_ = IFE::SpriteManager::Instance()->GetSpritePtr("Pause")->GetComponent<PoseMenu>();

	sprite_.clear();

	sprite_.push_back(std::make_pair(true, SpriteManager::Instance()->GetSpritePtr("flame")));
	sprite_.push_back(std::make_pair(true, SpriteManager::Instance()->GetSpritePtr("Down")));
	sprite_.push_back(std::make_pair(true, SpriteManager::Instance()->GetSpritePtr("Up")));
	sprite_.push_back(std::make_pair(true, SpriteManager::Instance()->GetSpritePtr("ModeChangeDrone")));
	sprite_.push_back(std::make_pair(true, SpriteManager::Instance()->GetSpritePtr("SpeedChange")));
	sprite_.push_back(std::make_pair(true, SpriteManager::Instance()->GetSpritePtr("RT")));
	sprite_.push_back(std::make_pair(true, SpriteManager::Instance()->GetSpritePtr("LT")));
	sprite_.push_back(std::make_pair(true, SpriteManager::Instance()->GetSpritePtr("PauseDrone")));
	sprite_.push_back(std::make_pair(true, SpriteManager::Instance()->GetSpritePtr("YDrone")));
	sprite_.push_back(std::make_pair(true, SpriteManager::Instance()->GetSpritePtr("PauseTextDrone")));
	sprite_.push_back(std::make_pair(true, SpriteManager::Instance()->GetSpritePtr("LStickDrone")));

	sprite_.push_back(std::make_pair(false, SpriteManager::Instance()->GetSpritePtr("Sneak")));
	sprite_.push_back(std::make_pair(false, SpriteManager::Instance()->GetSpritePtr("B")));
	sprite_.push_back(std::make_pair(false, SpriteManager::Instance()->GetSpritePtr("X")));
	sprite_.push_back(std::make_pair(false, SpriteManager::Instance()->GetSpritePtr("Y")));
	sprite_.push_back(std::make_pair(false, SpriteManager::Instance()->GetSpritePtr("Pause")));
	sprite_.push_back(std::make_pair(false, SpriteManager::Instance()->GetSpritePtr("RStick")));
	sprite_.push_back(std::make_pair(false, SpriteManager::Instance()->GetSpritePtr("LStickNormal")));
	sprite_.push_back(std::make_pair(false, SpriteManager::Instance()->GetSpritePtr("Attack")));
	sprite_.push_back(std::make_pair(false, SpriteManager::Instance()->GetSpritePtr("CameraMove")));
	sprite_.push_back(std::make_pair(false, SpriteManager::Instance()->GetSpritePtr("CharaMove")));
	sprite_.push_back(std::make_pair(false, SpriteManager::Instance()->GetSpritePtr("ModeChangeNormal")));
	sprite_.push_back(std::make_pair(false, SpriteManager::Instance()->GetSpritePtr("PauseText")));
}

void UI::Finalize()
{

}

void UI::UIChange(bool mode)
{
	bool isJudge = false;
	for (size_t i = 0; i < sprite_.size(); i++)
	{
		if (mode == sprite_[i].first) {
			isJudge = true;
		}
		else {
			isJudge = false;
		}
		sprite_[i].second->drawFlag_ = isJudge;
	}
}

void UI::UIAllFalse()
{
	for (size_t i = 0; i < sprite_.size(); i++)
	{
		sprite_[i].second->drawFlag_ = false;
	}
}