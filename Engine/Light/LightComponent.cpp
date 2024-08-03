#include "LightComponent.h"
#include "Transform.h"
#include "Object3D.h"

using namespace IFE;

void IFE::LightComponent::Initialize()
{
	if (!lm_)lm_ = LightManager::Instance();
	if (lightNum_ == uint8_t(-1))lightNum_ = lm_->GetPointLightNumber();
	objectPtr_->DrawFlag_ = false;
}

void IFE::LightComponent::Update()
{
	if (ls_.active)
	{
		switch (lightFlag_)
		{
		case LightFlag::Directional:
			lightNum_ = min(lm_->s_DLIGHT_NUM, lightNum_);
			lm_->SetDirLightActive(lightNum_, ls_.active);
			lm_->SetDirLightColor(lightNum_, ls_.color);
			lm_->SetDirLightDir(lightNum_, ls_.dir);
			break;
		case LightFlag::Point:
			lightNum_ = min(lm_->s_PLIGHT_NUM, lightNum_);
			lm_->SetPointLightActive(lightNum_, ls_.active);
			lm_->SetPointLightAtten(lightNum_, ls_.atten);
			lm_->SetPointLightColor(lightNum_, ls_.color);
			lm_->SetPointLightPos(lightNum_, transform_->position_);
			break;
		case LightFlag::Spot:
			lightNum_ = min(lm_->s_SLIGHT_NUM, lightNum_);
			lm_->SetSpotLightActive(lightNum_, ls_.active);
			lm_->SetSpotLightAtten(lightNum_, ls_.atten);
			lm_->SetSpotLightColor(lightNum_, ls_.color);
			lm_->SetSpotLightDir(lightNum_, ls_.dir);
			lm_->SetSpotLightFactorAngle(lightNum_, ls_.factorAngle);
			lm_->SetSpotLightPos(lightNum_, transform_->position_);
			break;
		case LightFlag::Shadow:
			lightNum_ = min(lm_->s_CSHADOW_NUM, lightNum_);
			lm_->SetCircleShadowActive(lightNum_, ls_.active);
			lm_->SetCircleShadowAtten(lightNum_, ls_.atten);
			lm_->SetCircleShadowCasterPos(lightNum_, transform_->position_);
			lm_->SetCircleShadowDir(lightNum_, ls_.dir);
			lm_->SetCircleShadowDistanceCasterLight(lightNum_, ls_.distance);
			lm_->SetCircleShadowFactorAngle(lightNum_, ls_.factorAngle);
			break;
		default:
			break;
		}
	}
	else
	{
		switch (lightFlag_)
		{
		case LightFlag::Directional:
			lightNum_ = min(lm_->s_DLIGHT_NUM, lightNum_);
			lm_->SetDirLightActive(lightNum_, ls_.active);
			break;
		case LightFlag::Point:
			lightNum_ = min(lm_->s_PLIGHT_NUM, lightNum_);
			lm_->SetPointLightActive(lightNum_, ls_.active);
			break;
		case LightFlag::Spot:
			lightNum_ = min(lm_->s_SLIGHT_NUM, lightNum_);
			lm_->SetSpotLightActive(lightNum_, ls_.active);
			break;
		case LightFlag::Shadow:
			lightNum_ = min(lm_->s_CSHADOW_NUM, lightNum_);
			lm_->SetCircleShadowActive(lightNum_, ls_.active);
			break;
		default:
			break;
		}
	}
}

void IFE::LightComponent::LoadingComponent(nlohmann::json& json)
{
	auto j = JsonManager::Instance();

	j->GetData(json, "active", ls_.active);
	j->GetData(json, "distance", ls_.distance);
	j->GetData(json, "number", lightNum_);
	uint8_t f;
	j->GetData(json, "setting", f);
	lightFlag_ = (LightFlag)f;
	ls_.color = j->InputFloat3(json["color"]);
	ls_.dir = j->InputFloat3(json["dir"]);
	ls_.atten = j->InputFloat3(json["atten"]);
	ls_.factorAngle = j->InputFloat2(json["angle"]);
}

#ifdef EditorMode
#include "ImguiManager.h"
void IFE::LightComponent::DebugInitialize()
{
	Initialize();
}
void IFE::LightComponent::ComponentDebugGUI()
{
	auto im = ImguiManager::Instance();
	int32_t i = (int32_t)lightNum_;
	int32_t s = (int32_t)lightFlag_;
	const static std::vector<std::string> items = { "Directional", "Point", "Spot", "Shadow" };
	im->Combo(U8("ライト設定"), s, items);
	im->DragIntGUI(&i, U8("ライト番号"), 1, 0, 100);
	lightNum_ = uint8_t(i);
	if (lightFlag_ != LightFlag(s))
	{
		lightFlag_ = LightFlag(s);
		if (lightFlag_ == LightFlag::Point)lightNum_ = lm_->GetPointLightNumber();
		if (lightFlag_ == LightFlag::Spot)lightNum_ = lm_->GetSpotLightNumber();
	}
	im->CheckBoxGUI(&ls_.active, U8("アクティブ"));
	if (lightFlag_ == LightFlag::Directional || lightFlag_ == LightFlag::Point || lightFlag_ == LightFlag::Spot)
	{
		im->ColorEdit3GUI(&ls_.color, U8("ライトカラー"));
	}
	if (lightFlag_ == LightFlag::Directional || lightFlag_ == LightFlag::Spot)
	{
		im->DragFloat3GUI(&ls_.dir, U8("ライトの方向"), 0.1f);
	}
	if (lightFlag_ == LightFlag::Point || lightFlag_ == LightFlag::Spot)
	{
		im->DragFloat3GUI(&ls_.atten, U8("ライトの距離減衰"), 0.1f);
	}
	switch (lightFlag_)
	{
	case LightFlag::Directional:
		lightNum_ = min(lm_->s_DLIGHT_NUM, lightNum_);
		break;
	case LightFlag::Point:
		lightNum_ = min(lm_->s_PLIGHT_NUM, lightNum_);
		break;
	case LightFlag::Spot:
		lightNum_ = min(lm_->s_SLIGHT_NUM, lightNum_);
		im->DragFloat2GUI(&ls_.factorAngle, U8("ライトの角度減衰"), 0.1f);
		break;
	case LightFlag::Shadow:
		lightNum_ = min(lm_->s_CSHADOW_NUM, lightNum_);
		im->DragFloat3GUI(&ls_.dir, U8("丸影の方向"), 0.1f);
		im->DragFloat3GUI(&ls_.atten, U8("丸影の距離減衰"), 0.1f);
		im->DragFloat2GUI(&ls_.factorAngle, U8("丸影の角度減衰"), 0.1f);
		im->DragFloatGUI(&ls_.distance, U8("有効なライトとの距離"), 0.1f);
		break;
	default:
		im->TextGUI(U8("エラー"));
		break;
	}
	static bool a = false;
	im->CheckBoxGUI(&a, U8("常に更新"));
	if (a)
	{
		Update();
	}
	if (!a && im->ButtonGUI(U8("ライト更新")))
	{
		Update();
	}
}

void IFE::LightComponent::OutputComponent(nlohmann::json& json)
{
	auto j = JsonManager::Instance();
	json["active"] = ls_.active;
	j->OutputFloat3(json["color"], ls_.color);
	j->OutputFloat3(json["dir"], ls_.dir);
	j->OutputFloat3(json["atten"], ls_.atten);
	j->OutputFloat2(json["angle"], ls_.factorAngle);
	json["distance"] = ls_.distance;
	json["number"] = lightNum_;
	json["setting"] = uint8_t(lightFlag_);
}
#endif
