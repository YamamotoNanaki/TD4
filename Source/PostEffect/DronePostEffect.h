#pragma once
#include "IPostEffect.h"
#include "ConstBuffer.h"

class DronePostEffect :public IFE::IPostEffect
{
	IPostEffect* eh_;
	IPostEffect* dp_;

	struct DroneCameraPE{
		float noiseWidth;
		float noisePower;
		float noisePosY;
		bool drone;
	};
	DroneCameraPE* buff_;
	std::unique_ptr<IFE::ConstBuffer<DroneCameraPE>>cb_;
	float timer_ = 0;
	float maxTimer_ = -1;
	bool noise_ = false;
	uint8_t noiseNum_ = 0;
	uint8_t noiseMaxNum_ = 0;

public:
	bool droneFlag_ = false;

public:
	void Initialize();
	void Update();
	void Draw();

#ifdef EditorMode
	void PostEffectDebugGUI()override;
#endif
};