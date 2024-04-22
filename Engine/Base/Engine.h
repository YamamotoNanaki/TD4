#pragma once
#include "WindowsAPI.h"
#include "GraphicsAPI.h"
#include "EditorMode.h"

namespace IFE
{
	class Engine final
	{
	private:
		Engine() {}
		WindowsAPI* window_ = WindowsAPI::Instance();
		GraphicsAPI* gapi_ = GraphicsAPI::Instance();
		inline static bool endFlag_ = false;

	public:
		Engine(const Engine& obj) = delete;
		Engine& operator=(const Engine& obj) = delete;
		static Engine* Instance();
		void Initialize();
		void Run();
		void Finalize();
		static void SetShutdown();
	};
}
