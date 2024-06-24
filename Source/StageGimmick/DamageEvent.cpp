#include "DamageEvent.h"
#include "ObjectManager.h"
#include "Player.h"

DamageEvent::DamageEvent()
{
}

DamageEvent::~DamageEvent()
{
}


void DamageEvent::Initialize()
{
	isEnd_ = false;
}

void DamageEvent::Update()
{
	if (!isEnd_)
	{
		IFE::ObjectManager::Instance()->GetObjectPtr("PlayerAction")->GetComponent<PlayerAction>()->DecHp();
		isEnd_ = true;
	}
}
#ifdef EditorMode
#include "ImguiManager.h"
void DamageEvent::DebugGUI()
{
	IFE::ImguiManager* gui = IFE::ImguiManager::Instance();

}
#endif