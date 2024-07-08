#pragma once
#include"Component.h"
#include"IFEMath.h"
#include"SpriteManager.h"
#include"Transform.h"
#include"Ease.h"
#include"PoseMenu.h"

enum class ConfigFlag
{
	brightness,
	masterValume,
	BGMValume,
	SEValume,
	cameraReverse,
};

struct ConfigValue
{
	float brightness;
	float masterValume;
	float BGMValume;
	float SEValume;
	bool cameraReverse;
};

class Config :public IFE::Component
{
private:

	bool oldPoseFlag_ = false;

	PoseMenu* pose_ = nullptr;

	ConfigFlag configFlag_ = ConfigFlag::brightness;
	uint8_t selectNum_ = 0;
	uint8_t minSelectNum_ = 0;
	uint8_t maxSelectNum_ = 4;

	ConfigValue configValue_;

	IFE::Vector2 oldLAnalog_ = {};

public:
	/// <summary>
	/// èâä˙âª
	/// </summary>
	void Initialize();

	/// <summary>
	/// çXêV
	/// </summary>
	void Update();

	/// <summary>
	/// èIóπ
	/// </summary>
	void Finalize();

private:

	void ConfigSelect();

	void ConfigChange();

	void GageConfig(float& configValue);
	void BottonConfig(bool& configValue);

	void Reset();
	void ConfigReset();
};