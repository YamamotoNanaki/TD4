#pragma once
#include <list>
#include <memory>
#include "Emitter.h"
#include "Texture.h"
#include "EditorMode.h"
#include "IFEMath.h"

namespace IFE
{
	class ParticleManager
	{
		std::list<std::unique_ptr<Emitter>>emitterList_;
	private:
		ParticleManager() {}
		ParticleManager(const ParticleManager&);
		ParticleManager& operator=(const ParticleManager&) {}
		~ParticleManager()
		{
			emitterList_.clear();
		}

	public:
		static ParticleManager* Instance();
		static void Finalize();
		void Initialize();
		void Update();
		void Draw();
		void AddInitialize(const std::string& emitterName, const std::string& textureName);
		Emitter* Add(const std::string& emitterName);
		Emitter* GetEmitterPtr(const std::string& emitterName);
		Emitter* SearchEmitter(const std::string& name);
		Emitter* Instantiate(const std::string& emitterName, const Float3& position, const std::string& newObjectName = "");
		Emitter* Instantiate(const std::string& emitterName, const std::string& newObjectName = "");
		void SetTexture(const std::string& emitterName, const std::string& texName);
		std::list<std::unique_ptr<Emitter>>* GetEmitterList();
		void Reset();
		bool SearchName(const std::string& name);

#ifdef InverseEditorMode
#else
		void DebugGUI();
		void OutputScene();
		void DebugUpdate();
#endif
		void LoadingScene();

	private:
		std::string GetNewName(const std::string& emitterName);
	};
}
