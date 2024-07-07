#pragma once
#include "Component.h"
#include "IFEMath.h"
#include "Object3D.h"

class MapUI:public IFE::Component
{
public:
	MapUI();
	~MapUI();

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider);

#ifdef EditorMode

	void ComponentDebugGUI()override;
	void OutputComponent(nlohmann::json& json)override;
#endif
	void LoadingComponent(nlohmann::json& json)override;

private:

	IFE::Object3D* playerObj_ = nullptr;

};

