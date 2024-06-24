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

public:
	void Initialize();
	void Update();
	void Draw();

#ifdef EditorMode
	void PostEffectDebugGUI()override;
#endif
};