#include "Over.h"
#include "Input.h"
#include "Scene.h"
#include "Title.h"
#include "Sound.h"

using namespace IFE;

void Over::Initialize()
{
	isNext = false;
	IFE::Sound::Instance()->LoadWave("title");
	IFE::Sound::Instance()->SetVolume("title", 50);
}

void Over::Update()
{
	if (Input::PadTrigger(PADCODE::ALL) || Input::GetKeyTrigger(Key::Space))
	{
		if (!isNext) {
			IFE::Sound::Instance()->SoundPlay("title", false, true);
		}
		Scene::Instance()->SetNextScene("title");
		isNext = true;
	}
}
