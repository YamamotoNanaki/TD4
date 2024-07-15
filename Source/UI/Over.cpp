#include "Over.h"
#include "Input.h"
#include "Scene.h"
#include "Title.h"

using namespace IFE;

void Over::Initialize()
{
	isNext = false;
}

void Over::Update()
{
	if (Input::PadTrigger(PADCODE::ALL) || Input::GetKeyTrigger(Key::Space))
	{
		Scene::Instance()->SetNextScene("title");
		isNext = true;
	}
}
