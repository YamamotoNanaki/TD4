#pragma once
#include "Component.h"
#include "Object3D.h"
#include "Collider.h"

class LaserWire:public IFE::Component
{
public:

	//現在当たっているか
	bool isHit_ = false;

public:

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

	IFE::Float3 pos_;

	std::vector<IFE::Float3> poss_;
	std::vector<IFE::Float3> rots_;
	std::vector<IFE::Float3> scales_;

	std::vector<IFE::Object3D*> objects_;

	//テストで置く描画用のオブジェクトなんかいい感じに追加する方法あるなら直す
	IFE::Object3D testobj_[10];

	int32_t oldposSize = 0;
	int32_t oldrotSize = 0;
	int32_t oldscaleSize = 0;

	IFE::Collider* cols_;

	int32_t countHitTimer_ = 0;
	int32_t hitMaxTime_ = 0;

};

