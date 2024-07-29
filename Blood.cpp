#include "Blood.h"
#include "IFEEffekseerManager.h"
#include "Input.h"
#include "ObjectManager.h"
#include "Object3D.h"

using namespace IFE;

void Blood::Initialize()
{
	efk_ = IFEEffekseerManager::Instance()->GetEffekseer("blood");
	tra = ObjectManager::Instance()->GetObjectPtr("PlayerAction")->transform_;
}

void Blood::Update()
{
	pos = transform_->GetWorldPosition();
	rot = tra->rotation_;
	if (Input::GetKeyTrigger(Key::NUMPAD0))
	{
		efk_->Play(&pos, &rot, nullptr, 0, 0, { 0.25,0.25,0.25 });
	}
}
