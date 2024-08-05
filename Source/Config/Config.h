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
	float brightness = 0.0f;
	float masterValume = 1;
	float BGMValume = 1;
	float SEValume = 1;
	bool cameraReverse = false;
};

class Config :public IFE::Component
{
private:

	bool oldConfigFlag_ = false;

	PoseMenu* pose_ = nullptr;

	ConfigFlag configFlag_ = ConfigFlag::brightness;
	int8_t minSelectNum_ = 0;
	int8_t maxSelectNum_ = 4;
	int8_t selectNum_ = 0;

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

	const ConfigValue& GetConfigValue();

private:

	void ConfigSelect();

	void ConfigChange();

	void GageConfig(float& configValue);
	void BottonConfig(bool& configValue);

	void SelectNow(ConfigFlag flag, std::string name);

	void Reset();
	void ConfigReset();
};