#pragma once
#include "Component.h"
#include "IFEEffekseer.h"
#include "Transform.h"

namespace IFE { class BaseEnemy; };

class Blood : public IFE::Component
{
	using Component::Component;

	IFE::IFEEffekseer* efk_;
	IFE::Transform* tra;
	bool start_;
	bool* isAttack_;

public:
	void Initialize();
	void Update();
	void Play(IFE::BaseEnemy* enemy);

#ifdef EditorMode
	void ComponentDebugGUI();
#endif;
};

