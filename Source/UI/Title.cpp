#include "Title.h"
#include "input.h"
#include "Scene.h"
#include "Sound.h"

using namespace IFE;

//void Title::Initialize()
//{
//	//IFE::Sound::Instance()->LoadWave("title");
//}

void Title::Update()
{
	if (Input::PadTrigger(PADCODE::ALL) || Input::GetKeyTrigger(Key::Space))
	{
	/*	IFE::Sound::Instance()->SoundPlay(0, false, true);*/
		Scene::Instance()->SetNextScene("stage01");
	}
}
