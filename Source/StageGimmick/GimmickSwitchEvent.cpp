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
		//ギミックを探す
		if (itr->GetComponent<LaserWire>() != nullptr || 
			itr->GetComponent<SwitchObject>() != nullptr)
		{
			//指定した奴なら取得
			if (itr->GetObjectName() == selectGimmickName_)
			{
				selectGimmickObject_ = itr.get();
				break;
			}
		}
		

	}

	isSwitch_ = isInitSwitch_;
	/*if (selectGimmickObject_->GetComponent<LaserWire>() != nullptr)
	{
		selectGimmickObject_->GetComponent<LaserWire>()->SetIsActive(isSwitch_);
	}
	else if (selectGimmickObject_->GetComponent<SwitchObject>() != nullptr)
	{
		selectGimmickObject_->GetComponent<SwitchObject>()->SetIsActive(isSwitch_);
	}*/


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

		isSwitch_ = !isSwitch_;
		isEnd_ = true;
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
		//ギミックを探す
		if (itr->GetComponent<LaserWire>() != nullptr ||
			itr->GetComponent<SwitchObject>() != nullptr)
		{	
				selectGimmickNames.push_back(itr->GetObjectName());
		}


	}

	IFE::ImguiManager* gui = IFE::ImguiManager::Instance();
	gui->Combo("selectObject", selectNum, selectGimmickNames);
	selectGimmickName_ = selectGimmickNames[selectNum];
	gui->CheckBoxGUI(&isInitSwitch_, "InitialSwitch");

}

void GimmickSwitchEvent::OutputData(nlohmann::json& json)
{
	json["selectGimmickName"] = selectGimmickName_;
	json["isSwitch"] = isSwitch_;
	json["isSwitch"] = isInitSwitch_;
}
#endif

void GimmickSwitchEvent::InputData(nlohmann::json& json)
{
	IFE::JsonManager::Instance()->GetData<std::string>(json, "selectGimmickName", selectGimmickName_);
	IFE::JsonManager::Instance()->GetData<bool>(json, "isSwitch", isSwitch_);
}

void GimmickSwitchEvent::StartInitialize()
{
	isEnd_ = false;
}