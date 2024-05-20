#include "Gimmick.h"
#include "Collider.h"

void Gimmick::Initialize()
{

}

void Gimmick::Update()
{

}

void Gimmick::Draw()
{

}

void Gimmick::Finalize()
{

}

void Gimmick::OnColliderHit(IFE::ColliderCore collider)
{
	
}

#ifdef EditorMode
#include "ImguiManager.h"
void Gimmick::ComponentDebugGUI()
{
	IFE::ImguiManager* gui = IFE::ImguiManager::Instance();
	gui;
}

void Gimmick::OutputComponent(nlohmann::json& json)
{
	json;
}
#endif

void Gimmick::LoadingComponent(nlohmann::json& json)
{
	json;
}