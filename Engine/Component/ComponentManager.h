#pragma once
#include "Component.h"
#include "Debug.h"
#include <list>
#include <vector>
#include <string>
#include <typeinfo>
#include <memory>
#include "EditorMode.h"

namespace IFE
{
	class ComponentManager
	{
	private:
		ComponentManager& operator=(const ComponentManager& obj) = delete;
	public:
		std::list<std::unique_ptr<Component>> componentList_;

	private:
		size_t colNum_ = 0;

	public:
		Object3D* objectPtr_ = nullptr;
		Sprite* spritePtr_ = nullptr;
		Emitter* emitterPtr_ = nullptr;
		Particle* particlePtr_ = nullptr;
		Camera* cameraPtr_ = nullptr;

	public:
		template <class T>
		void AddComponent();
		template <class T>
		void AddComponent(std::unique_ptr<Component> ptr);
		template <class T>
		void AddComponentBack();
		template <class T>
		void AddComponentBack(std::unique_ptr<Component> ptr);
		template <class T>
		void AddComponent(std::unique_ptr<Component> ptr, Object3D* components);
		template <class T>
		void AddComponentBack(std::unique_ptr<Component> ptr, Object3D* components);
		template <class T>
		void AddComponent(std::unique_ptr<Component> ptr, Sprite* components);
		template <class T>
		void AddComponentBack(std::unique_ptr<Component> ptr, Sprite* components);
		template <class T>
		void AddComponent(std::unique_ptr<Component> ptr, Emitter* components);
		template <class T>
		void AddComponentBack(std::unique_ptr<Component> ptr, Emitter* components);
		template <class T>
		void AddComponent(std::unique_ptr<Component> ptr, Particle* components);
		template <class T>
		void AddComponentBack(std::unique_ptr<Component> ptr, Particle* components);
		template <class T>
		void AddComponent(std::unique_ptr<Component> ptr, Camera* components);
		template <class T>
		void AddComponentBack(std::unique_ptr<Component> ptr, Camera* components);
		template <class T>
		T* GetComponent();
		template <class T>
		void DeleteComponent();
		template <class T>
		void RemoveComponent();
		void Initialize();
		void Update();
		void Draw();
		void Finalize();
		void SetTransform();
		void SetObjectPtr(Object3D* objectPtr);
		void SetSpritePtr(Sprite* spritePtr);
		void SetEmitterPtr(Emitter* emitterPtr);
		void SetParticlePtr(Particle* particlePtr);
		void SetCameraPtr(Camera* cameraPtr);
		std::vector<std::string> GetAllComponentName();
		void OnColliderHit(Collider* collider);
		void CopyValue(ComponentManager* ptr);
#ifdef InverseEditorMode
#else
		void DebugGUI();
		void OutputScene(nlohmann::json& json);
		void DebugUpdate();
#endif
		void LoadingScene(nlohmann::json& json, const std::string& comName);

		virtual ~ComponentManager() {};
	private:
		std::string SetName(const std::string& name);
	};
	template<class T>
	inline void ComponentManager::AddComponent()
	{
		auto ptr = std::make_unique<T>(this);
		ptr->SetComponentName(SetName(typeid(T).name()));
		ptr->INITIALIZE();
		componentList_.push_front(std::move(ptr));
	}
	template<class T>
	inline void ComponentManager::AddComponent(std::unique_ptr<Component> ptr)
	{
		ptr->SetComponents(this);
		ptr->INITIALIZE();
		componentList_.push_front(std::move(ptr));
	}
	template<class T>
	inline void ComponentManager::AddComponentBack()
	{
		auto ptr = std::make_unique<T>(this);
		ptr->SetComponentName(SetName(typeid(T).name()));
		ptr->INITIALIZE();
		componentList_.push_back(std::move(ptr));
	}
	template<class T>
	inline void ComponentManager::AddComponentBack(std::unique_ptr<Component> ptr)
	{
		ptr->SetComponents(this);
		ptr->INITIALIZE();
		componentList_.push_back(std::move(ptr));
	}
	template<class T>
	inline void ComponentManager::AddComponent(std::unique_ptr<Component> ptr, Object3D* components)
	{
		if (components)ptr->SetComponents(components);
		componentList_.push_front(std::move(ptr));
	}
	template<class T>
	inline void ComponentManager::AddComponentBack(std::unique_ptr<Component> ptr, Object3D* components)
	{
		if (components)ptr->SetComponents(components);
		componentList_.push_back(std::move(ptr));
	}
	template<class T>
	inline T* ComponentManager::GetComponent()
	{
		if (this == nullptr)return nullptr;
		for (auto& itr : componentList_)
		{
			T* ptr = dynamic_cast<T*>(itr.get());
			if (ptr)return ptr;
		}
		return nullptr;
	}
	template<class T>
	inline void ComponentManager::DeleteComponent()
	{
		for (auto& itr : componentList_)
		{
			T* ptr = dynamic_cast<T*>(itr);
			if (ptr)
			{
				delete ptr;
				componentList_.remove(itr);
			}
		}
	}
	template<class T>
	inline void ComponentManager::RemoveComponent()
	{
		for (auto& itr : componentList_)
		{
			T* ptr = dynamic_cast<T*>(itr.get());
			if (ptr)
			{
				componentList_.remove(itr);
			}
		}
	}
	template<class T>
	inline void ComponentManager::AddComponent(std::unique_ptr<Component> ptr, Sprite* components)
	{
		if (components)ptr->SetComponents(components);
		componentList_.push_front(std::move(ptr));
	}
	template<class T>
	inline void ComponentManager::AddComponentBack(std::unique_ptr<Component> ptr, Sprite* components)
	{
		if (components)ptr->SetComponents(components);
		componentList_.push_back(std::move(ptr));
	}
	template<class T>
	inline void ComponentManager::AddComponent(std::unique_ptr<Component> ptr, Emitter* components)
	{
		if (components)ptr->SetComponents(components);
		componentList_.push_front(std::move(ptr));
	}
	template<class T>
	inline void ComponentManager::AddComponentBack(std::unique_ptr<Component> ptr, Emitter* components)
	{
		if (components)ptr->SetComponents(components);
		componentList_.push_back(std::move(ptr));
	}
	template<class T>
	inline void ComponentManager::AddComponent(std::unique_ptr<Component> ptr, Particle* components)
	{
		if (components)ptr->SetComponents(components);
		componentList_.push_front(std::move(ptr));
	}
	template<class T>
	inline void ComponentManager::AddComponentBack(std::unique_ptr<Component> ptr, Particle* components)
	{
		if (components)ptr->SetComponents(components);
		componentList_.push_back(std::move(ptr));
	}
	template<class T>
	inline void ComponentManager::AddComponent(std::unique_ptr<Component> ptr, Camera* components)
	{
		if (components)ptr->SetComponents(components);
		componentList_.push_front(std::move(ptr));
	}
	template<class T>
	inline void ComponentManager::AddComponentBack(std::unique_ptr<Component> ptr, Camera* components)
	{
		if (components)ptr->SetComponents(components);
		componentList_.push_back(std::move(ptr));
	}
}
