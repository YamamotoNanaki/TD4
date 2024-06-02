#pragma once
#include "IPostEffect.h"
#include <list>
#include "EditorMode.h"
#include "DefaultPostEffect.h"

namespace IFE
{
	class PostEffectManager
	{
		std::list<std::unique_ptr<IPostEffect>>postEffects;
		IPostEffect* defaultPE;
	public:
		void Draw();
		void Update();
		void Initialize();
		void ObjectDrawBefore();
		void ObjectDrawAfter();
		void Finalize();
	};
}
