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

		void Draw();
		void Update();
		void Initialize();
		void ObjectDrawBefore();
		void ObjectDrawAfter();
		void Reset();
		void Finalize();
		IPostEffect* GetPostEffect(std::string name);

#ifdef EditorMode
		void DebugGUI();
#endif
	};
}
