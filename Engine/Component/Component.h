#pragma once
#include <list>
#include <string>
#include <typeinfo>
#include "JsonManager.h"
#include "EditorMode.h"

namespace IFE
{
	class Object3D;
	class Sprite;
	class Particle;
	class Emitter;
	class Transform;
	class Transform2D;
	class TransformParticle;
	class TransformCamera;
	class ComponentManager;
	class ColliderCore;
	class Camera;
	class Component
	{
	public:
		Object3D* objectPtr_ = nullptr;
		Sprite* spritePtr_ = nullptr;
		Particle* particlePtr_ = nullptr;
		Emitter* emitterPtr_ = nullptr;
		Camera* cameraPtr_ = nullptr;
		std::string componentName_;
		bool componentDeleteFlag_ = false;
	protected:
		virtual void Initialize() {};

	public:
		Transform* transform_ = nullptr;
		TransformParticle* transformParticle_ = nullptr;
		Transform2D* transform2D_ = nullptr;
		TransformCamera* transformCamera_ = nullptr;
		void INITIALIZE();
		virtual void Update() {};
		virtual void Draw() {};
		virtual ~Component();
		Component(ComponentManager* component);
		Component() {}
		void SetTransform();
		std::string GetComponentName();
		void SetComponentName(const std::string& componentName);
		void SetComponents(ComponentManager* cm);
		bool GetComponentDeleteFlag();
		Object3D* GetObjectPtr();
		Sprite* GetSpritePtr();
		Emitter* GetEmitterPtr();
		Particle* GetParticlePtr();
		Camera* GetCameraPtr();
		virtual void Copy(Component* component);
		virtual void OnColliderHit(ColliderCore* myCollider, ColliderCore* hitCollider);
		void CopyValue(Component* component);
		virtual void CopyValueComponent(Component* component);
#ifdef InverseEditorMode
#else
		virtual void DebugGUI(uint32_t num);
		virtual void ComponentDebugGUI() {};
		void OutputScene(nlohmann::json& json);
		virtual void OutputComponent(nlohmann::json& json);
#endif
		void LoadingScene(nlohmann::json& json);
		virtual void LoadingComponent(nlohmann::json& json);
	};
}
