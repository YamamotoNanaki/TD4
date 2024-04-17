#include "Engine.h"

using namespace IFE;

int32_t WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int32_t)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Engine* engine = Engine::Instance();
	engine->Initialize();
	engine->Run();
	engine->Finalize();
	//_CrtDumpMemoryLeaks();

	return 0;
}