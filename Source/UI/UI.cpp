#include "UI.h"

void IFE::UI::Finalize()
{

}

void IFE::UI::UIChange(bool mode)
{
	if (mode == true)
	{
		//ドローンモードのUI
		SpriteManager::Instance()->GetSpritePtr("flame")->drawFlag_ = true;
		SpriteManager::Instance()->GetSpritePtr("Down")->drawFlag_ = true;
		SpriteManager::Instance()->GetSpritePtr("Up")->drawFlag_ = true;
		SpriteManager::Instance()->GetSpritePtr("ModeChangeDrone")->drawFlag_ = true;
		SpriteManager::Instance()->GetSpritePtr("CameraMove")->drawFlag_ = false;
		SpriteManager::Instance()->GetSpritePtr("CharaMove")->drawFlag_ = false;
		SpriteManager::Instance()->GetSpritePtr("ModeChangeNormal")->drawFlag_ = false;
		SpriteManager::Instance()->GetSpritePtr("SpeedChange")->drawFlag_ = true;
		SpriteManager::Instance()->GetSpritePtr("Sneak")->drawFlag_ = false;
		SpriteManager::Instance()->GetSpritePtr("RT")->drawFlag_ = true;
		SpriteManager::Instance()->GetSpritePtr("LT")->drawFlag_ = true;
		SpriteManager::Instance()->GetSpritePtr("B")->drawFlag_ = false;
		SpriteManager::Instance()->GetSpritePtr("X")->drawFlag_ = false;
		SpriteManager::Instance()->GetSpritePtr("Y")->drawFlag_ = false;
		SpriteManager::Instance()->GetSpritePtr("Pause")->drawFlag_ = false;
		SpriteManager::Instance()->GetSpritePtr("PauseText")->drawFlag_ = false;
		SpriteManager::Instance()->GetSpritePtr("PauseDrone")->drawFlag_ = true;
		SpriteManager::Instance()->GetSpritePtr("PauseTextDrone")->drawFlag_ = true;
		SpriteManager::Instance()->GetSpritePtr("YDrone")->drawFlag_ = true;
		SpriteManager::Instance()->GetSpritePtr("RStick")->drawFlag_ = false;
		SpriteManager::Instance()->GetSpritePtr("LStickNormal")->drawFlag_ = false;
		SpriteManager::Instance()->GetSpritePtr("Attack")->drawFlag_ = false;
	}
	else
	{
		//アクションモードのUI
		SpriteManager::Instance()->GetSpritePtr("flame")->drawFlag_ = false;
		SpriteManager::Instance()->GetSpritePtr("Down")->drawFlag_ = false;
		SpriteManager::Instance()->GetSpritePtr("Up")->drawFlag_ = false;
		SpriteManager::Instance()->GetSpritePtr("ModeChangeDrone")->drawFlag_ = false;
		SpriteManager::Instance()->GetSpritePtr("CameraMove")->drawFlag_ = true;
		SpriteManager::Instance()->GetSpritePtr("CharaMove")->drawFlag_ = true;
		SpriteManager::Instance()->GetSpritePtr("ModeChangeNormal")->drawFlag_ = true;
		SpriteManager::Instance()->GetSpritePtr("SpeedChange")->drawFlag_ = false;
		SpriteManager::Instance()->GetSpritePtr("Sneak")->drawFlag_ = true;
		SpriteManager::Instance()->GetSpritePtr("RT")->drawFlag_ = false;
		SpriteManager::Instance()->GetSpritePtr("LT")->drawFlag_ = false;
		SpriteManager::Instance()->GetSpritePtr("B")->drawFlag_ = true;
		SpriteManager::Instance()->GetSpritePtr("X")->drawFlag_ = true;
		SpriteManager::Instance()->GetSpritePtr("Y")->drawFlag_ = true;
		SpriteManager::Instance()->GetSpritePtr("Pause")->drawFlag_ = true;
		SpriteManager::Instance()->GetSpritePtr("PauseText")->drawFlag_ = true;
		SpriteManager::Instance()->GetSpritePtr("PauseDrone")->drawFlag_ = false;
		SpriteManager::Instance()->GetSpritePtr("PauseTextDrone")->drawFlag_ = false;
		SpriteManager::Instance()->GetSpritePtr("YDrone")->drawFlag_ = false;
		SpriteManager::Instance()->GetSpritePtr("RStick")->drawFlag_ = true;
		SpriteManager::Instance()->GetSpritePtr("LStickNormal")->drawFlag_ = true;
		SpriteManager::Instance()->GetSpritePtr("Attack")->drawFlag_ = true;
	}
}

void IFE::UI::Damage(int8_t hp)
{
	hp--;
}


