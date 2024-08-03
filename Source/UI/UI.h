#pragma once
#include"IFEMath.h"
#include"SpriteManager.h"
#include"PoseMenu.h"
#include"utility"

class UI
{
private:
	std::vector<std::pair<bool, IFE::Sprite*>>sprite_;

	PoseMenu* pause_ = nullptr;

	//ƒƒ“ƒoŠÖ”
public:
	void Initialize();

	// I—¹
	void Finalize();

	// UIØ‘Ö
	void UIChange(bool mode);

	void UIAllFalse();
};