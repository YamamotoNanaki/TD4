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

	//メンバ関数
public:
	void Initialize();

	// 終了
	void Finalize();

	// UI切替
	void UIChange(bool mode);

	void UIAllFalse();
};