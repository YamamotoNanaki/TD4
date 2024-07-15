#include "Title.h"
#include "input.h"
#include "Scene.h"
#include "Sound.h"
#include "Over.h"

using namespace IFE;

void Title::Initialize()
{
	IFE::Sound::Instance()->LoadWave("title");
	IFE::Sound::Instance()->SetVolume("title",50);
	isNext = false;
}

void Title::Update()
{
		if (Input::PadTrigger(PADCODE::ALL) || Input::GetKeyTrigger(Key::Space))
		{
			if (!isNext) {
				IFE::Sound::Instance()->SoundPlay("title", false, true);
			}
			isNext = true;
			Scene::Instance()->SetNextScene("stage01");
		}
}
