#include "Over.h"
#include "Input.h"
#include "Scene.h"

using namespace IFE;

void Over::Update()
{
	if (Input::PadTrigger(PADCODE::ALL) || Input::GetKeyTrigger(Key::Space))
	{
		Scene::Instance()->SetNextScene("title");
	}
}
