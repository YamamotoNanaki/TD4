#include "Title.h"
#include "input.h"
#include "Scene.h"
#include "Sound.h"

using namespace IFE;

void Title::Initialize()
{
	IFE::Sound::Instance()->LoadWave("title");
	IFE::Sound::Instance()->SetVolume("title",50);
	isNext = false;
}

void Title::Update()
{
	if (!isNext) {
		if (Input::PadTrigger(PADCODE::ALL) || Input::GetKeyTrigger(Key::Space))
		{
			IFE::Sound::Instance()->SoundPlay("title", false, true);
			Scene::Instance()->SetNextScene("stage01");
			isNext = true;
		}
	}
}
