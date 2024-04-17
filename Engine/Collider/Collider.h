#pragma once
#include "Component.h"
#include "CollisionPrimitive.h"

namespace IFE
{
	enum class ColliderType
	{
		SPHERE, MESH
	};
	enum class Attribute
	{
		LANDSHAPE = 0b1 << 0, ALLIES = 0b1 << 1, ENEMYS = 0b1 << 2,
		ALL = 0xffff
	};
	class Collider : public Component
	{
		using Component::Component;
	private:
		Float3 offsetPosition_;
		Float3 offsetScale_;
		Float3* parentPosition_ = nullptr;
		Float3* parentScale_ = nullptr;
		ColliderType colliderType_;
		bool pushBack_ = false;
		bool notPushBack_ = false;
		bool groundJudge_ = false;

		std::unique_ptr<MeshCollider> meshCollider_;
	public:
		Vector3 interPoint_;
		uint16_t attribute_ = 0;
		bool onGround_ = false;
		RaycastHit groundHit_;

	public:
		void Initialize()override;
		void Draw()override;
		void Update()override;

		Float3 GetOffsetPosition();
		Float3 GetOffsetScale();
		Float3 GetColliderPosition();
		Float3 GetColliderScale();
		ColliderType GetColliderType();
		void SetColliderType(const ColliderType& colliderType);
		void SetOffsetPosition(const Float3& pos);
		void SetOffsetScale(const Float3& sca);

		MeshCollider* GetMeshCollider();
		bool GetPushBackFlag();
		bool GetNoPushBackFlag();
		bool GetGroundJudgeFlag();
		void SetPushBackFlag(bool flag);
		void SetNoPushBackFlag(bool flag);
		void SetGroundJudgeFlag(bool flag);

	private:
		void GetParentParms();
	public:

		void LoadingComponent(nlohmann::json& json)override;

#ifdef NDEBUG
#else
		void OutputComponent(nlohmann::json& json)override;
		void ComponentDebugGUI()override;
#endif
	};
}
