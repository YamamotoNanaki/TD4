#pragma once
#include"Component.h"
#include"IFEMath.h"
#include "Transform.h"

namespace IFE {
	class Propeller :public IFE::Component
	{
		using Component::Component;

	public:
		//‚Ð‚½‚·‚ç‰ñ“]
		void Turn();

	};
}