#include "DoorEvent.h"
#include "ObjectManager.h"
#include "Ease.h"

DoorEvent::DoorEvent()
{
}

DoorEvent::~DoorEvent()
{
}


void DoorEvent::Initialize()
{
	isEnd_ = false;
}

void DoorEvent::Update()
{
	if (!isEnd_)
	{
		
	}
}
#ifdef EditorMode
#include "ImguiManager.h"
void DoorEvent::DebugGUI()
{
	//IFE::ImguiManager* gui = IFE::ImguiManager::Instance();

}
#endif

void DoorEvent::OutputData(nlohmann::json& json)
{

}

void DoorEvent::InputData(nlohmann::json& json)
{

}