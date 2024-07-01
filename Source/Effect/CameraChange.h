#pragma once
#include "Component.h"
#include "Player.h"
#include "Sprite.h"

class CameraChange :public IFE::Component
{
	Player::CameraChangeParam* ccp_;
	std::vector<IFE::Sprite*> sprite_;
	bool endFlag_ = true;

public:
	void Initialize()override;
	void Update()override;

private:
	void Add();
	void UpdateSquare();
	void UpdateEnd();
	void End();
};

