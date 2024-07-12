#include "Title.h"
#include "input.h"
#include "Scene.h"
#include "Sound.h"

using namespace IFE;

void Title::Initialize()
{
	IFE::Sound::Instance()->LoadMP3("title");
	IFE::Sound::Instance()->SetVolume("title",50);
	IFE::Sound::Instance()->SoundPlay("title", true, true);
}

void Title::Update()
{
	if (Input::PadTrigger(PADCODE::ALL) || Input::GetKeyTrigger(Key::Space))
	{
		Scene::Instance()->SetNextScene("stage01");
	}
}
