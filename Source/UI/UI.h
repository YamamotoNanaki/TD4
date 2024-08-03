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

	//�����o�֐�
public:
	void Initialize();

	// �I��
	void Finalize();

	// UI�ؑ�
	void UIChange(bool mode);

	void UIAllFalse();
};