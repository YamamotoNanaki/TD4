#pragma once
#include "Component.h"

namespace IFE
{
	class DebugCamera : public Component
	{
		using Component::Component;
		float distance = 20;
	public:
		void Initialize();
		void Update();
	};
}
