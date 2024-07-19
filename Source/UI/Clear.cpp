#include "Clear.h"
#include "Input.h"
#include "Scene.h"

using namespace IFE;

void Clear::Initialize()
{
	/*IFE::Sound::Instance()->LoadWave("clear");
	IFE::Sound::Instance()->SetVolume("clear", 50);*/
}

void Clear::Update()
{
	/*IFE::Sound::Instance()->SoundPlay("clear",true,false);*/
	if (Input::PadTrigger(PADCODE::ALL) || Input::GetKeyTrigger(Key::Space))
	{
		Scene::Instance()->SetNextScene("title");
	}
}
