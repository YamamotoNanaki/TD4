#pragma once
#include "IPostEffect.h"

namespace IFE
{
	class DefaultPostEffect : public IPostEffect
	{
	public:
		void Draw();
		void Update();
		void Initialize();
		void DrawBefore();
		void DrawAfter();
	};
}
