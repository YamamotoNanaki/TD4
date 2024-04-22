#include "Engine.h"
#include "Input.h"
#include "Scene.h"
#include "Rand.h"


using namespace IFE;

Engine* IFE::Engine::Instance()
{
	static Engine inst;
	return &inst;
}

void IFE::Engine::Initialize()
{
	window_->Initialize(1600, 900,L"TD4");
	gapi_->Initialize(*WindowsAPI::Instance()->GetHWnd());
	Input::Initalize();
	IFERand::Initialize();
}

void IFE::Engine::Run()
{
	Scene* scene = Scene::Instance();
	scene->Initialize();
	while (true)
	{
		Input::Update();
		scene->Update();
		gapi_->DrawBefore();
		gapi_->DrawSetViewport();
		scene->Draw();
		gapi_->DrawAfter();

		if (window_->Message() || endFlag_)
		{
			int a = 0;
			a++;
#ifdef EditorMode
			scene->OutputScene();
#endif
			scene->Finalize();
			return;
		}
	}
}

void IFE::Engine::Finalize()
{
	window_->Finalize();
	gapi_->Finalize();
}

void IFE::Engine::SetShutdown()
{
	endFlag_ = true;
}
