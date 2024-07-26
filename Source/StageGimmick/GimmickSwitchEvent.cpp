#include "GimmickSwitchEvent.h"
#include "ObjectManager.h"
#include "ModelManager.h"
#include "CollideManager.h"
#include "Ease.h"
#include "Collider.h"
#include "Transform.h"
#include "LaserWire.h"
#include "SwitchObject.h"

GimmickSwitchEvent::GimmickSwitchEvent()
{

}

GimmickSwitchEvent::~GimmickSwitchEvent()
{
}


void GimmickSwitchEvent::Initialize()
{

	IFE::ObjectManager* objm = IFE::ObjectManager::Instance();
	auto& list = objm->GetObjList();

	for (auto& itr : list)
	{
		//ƒMƒ~ƒbƒN‚ð’T‚·
		if (itr->GetComponent<LaserWire>() != nullptr || 
			itr->GetComponent<SwitchObject>() != nullptr)
		{
			//Žw’è‚µ‚½“z‚È‚çŽæ“¾
			if (itr->GetObjectName() == selectGimmickName_)
			{
				selectGimmickObject_ = itr.get();
				break;
			}
		}
		

	}


}

void GimmickSwitchEvent::Update()
{
	if (!isEnd_)
	{
		if (selectGimmickObject_->GetComponent<LaserWire>() != nullptr)
		{
			selectGimmickObject_->GetComponent<LaserWire>()->SetIsActive(isSwitch_);
		}
		else if (selectGimmickObject_->GetComponent<SwitchObject>() != nullptr)
		{
			selectGimmickObject_->GetComponent<SwitchObject>()->SetIsActive(isSwitch_);
		}
	}
}
#ifdef EditorMode
#include "ImguiManager.h"
void GimmickSwitchEvent::DebugGUI()
{
	
	std::vector<std::string> selectGimmickNames;

	IFE::ObjectManager* objm = IFE::ObjectManager::Instance();
	auto& list = objm->GetObjList();

	for (auto& itr : list)
	{
		//ƒMƒ~ƒbƒN‚ð’T‚·
		if (itr->GetComponent<LaserWire>() != nullptr ||
			itr->GetComponent<SwitchObject>() != nullptr)
		{	
				selectGimmickNames.push_back(itr->GetObjectName());
		}


	}

	/*IFE::ImguiManager* gui = IFE::ImguiManager::Instance();*/
	//gui->DragFloat3GUI(&spawnPos_, "spawnPos", 0.25f, -1000, 1000);

}
#endif

void GimmickSwitchEvent::OutputData(nlohmann::json& json)
{
	json["selectGimmickName"] = selectGimmickName_;
	json["isSwitch"] = isSwitch_;
}

void GimmickSwitchEvent::InputData(nlohmann::json& json)
{
	IFE::JsonManager::Instance()->GetData<std::string>(json, "selectGimmickName", selectGimmickName_);
	IFE::JsonManager::Instance()->GetData<bool>(json, "isSwitch", isSwitch_);
}

void GimmickSwitchEvent::StartInitialize()
{
	isEnd_ = false;
}