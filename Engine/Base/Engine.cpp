#include "Engine.h"
#include "Input.h"
#include "Scene.h"
#include "Rand.h"
#include "ComponentHelp.h"


using namespace IFE;

Engine* IFE::Engine::Instance()
{
	static Engine inst;
	return &inst;
}

void IFE::Engine::Initialize()
{
	window_->Initialize(1920, 1080,L"プロジェクトキル");
	gapi_->Initialize(*WindowsAPI::Instance()->GetHWnd());
	Input::Initalize();
	IFERand::Initialize();
	ComponentHelp::StaticHelpInitialize();
	IPostEffectHelp::StaticHelpInitialize();
}

void IFE::Engine::Run()
{
	Scene* scene = Scene::Instance();
	scene->Initialize();
	while (true)
	{
		window_->Update();
		gapi_->ResizeSwapChain();
		Input::Update();
		scene->Update();
		gapi_->DrawBefore();
		gapi_->DrawSetViewport();
		scene->Draw();
		gapi_->DrawAfter();

		if (window_->Message() || endFlag_)
		{
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
