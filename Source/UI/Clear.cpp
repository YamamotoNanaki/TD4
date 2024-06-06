#include "Clear.h"
#include "Input.h"
#include "Scene.h"

using namespace IFE;

void Clear::Update()
{
	if (Input::PadTrigger(PADCODE::ALL) || Input::GetKeyTrigger(Key::Space))
	{
		Scene::Instance()->SetNextScene("title");
	}
}
