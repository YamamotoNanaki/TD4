#pragma once
#include "Component.h"
#include "Matrix.h"

namespace IFE
{
	class DebugCamera : public Component
	{
		using Component::Component;
#ifdef EditorMode
		float distance = 20;
		Matrix matRot_;
		float scaleX;
		float scaleY;
		float moveScale = 10;
		float rotScale = 10;
	public:
		void Initialize();
		void Update();

		void ComponentDebugGUI();

#endif
	};
}
