#pragma once
#include "IPostEffect.h"
#include <list>
#include "EditorMode.h"

namespace IFE
{
	class PostEffectManager
	{
		std::list<IPostEffect>postEffects;
	public:
		void Draw();
		void Update();
		void Initialize();
		void DrawBefore();
		void DrawAfter();
	};
}
