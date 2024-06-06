#include "Title.h"
#include "input.h"
#include "Scene.h"

using namespace IFE;

void Title::Update()
{
	if (Input::PadTrigger(PADCODE::ALL) || Input::GetKeyTrigger(Key::Space))
	{
		Scene::Instance()->SetNextScene("stage01");
	}
}
