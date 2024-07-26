#pragma once
#include "IPostEffect.h"
#include <list>
#include <string>
#include "EditorMode.h"
#include "DefaultPostEffect.h"

namespace IFE
{
	class PostEffectManager
	{
		std::list<std::unique_ptr<IPostEffect>>postEffects;
		IPostEffect* defaultPE;
	public:
		PostEffectManager() {}
		PostEffectManager(const PostEffectManager&) {}
		PostEffectManager& operator=(const PostEffectManager&) {}
		~PostEffectManager() {}
		static PostEffectManager* Instance();

		void Add(std::string name,uint16_t initParm);
		void Draw();
		void Update();
		void Initialize();
		void ObjectDrawBefore();
		void ObjectDrawAfter();
		void Reset();
		static void Finalize();
		IPostEffect* GetPostEffect(std::string name);
		inline IPostEffect* GetDefaultPE() { return defaultPE; };

		void Loading();
#ifdef EditorMode
		void Output();
		void DebugGUI();
#endif
	};
}
