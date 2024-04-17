#pragma once
#include <list>
#include <memory>
#include "Emitter.h"
#include "Texture.h"
#include "IFMath.h"

namespace IFE
{
	class ParticleManager
	{
		std::list<std::unique_ptr<Emitter>>emitterList;
	private:
		ParticleManager() {}
		ParticleManager(const ParticleManager&);
		ParticleManager& operator=(const ParticleManager&) {}
		~ParticleManager()
		{
			emitterList.clear();
		}

	public:
		static ParticleManager* Instance();
		static void Finalize();
		void Initialize();
		void Update();
		void Draw();
		void AddInitialize(std::string emitterName, std::string textureName);
		Emitter* Add(std::string emitterName);
		Emitter* GetEmitterPtr(std::string emitterName);
		Emitter* Instantiate(Emitter* gameObject, Float3 position, float rotation);
		Emitter* Instantiate(Emitter* gameObject, Float3 position);
		Emitter* Instantiate(Emitter* gameObject);
		void SetTexture(std::string emitterName, std::string texName);
		std::list<std::unique_ptr<Emitter>>* GetEmitterList();
		void Reset();
		bool SearchName(std::string name);

#ifdef _DEBUG
		void DebugGUI();
		void OutputScene();
		void DebugUpdate();
#endif
		void LoadingScene();

	private:
		std::string GetNewName(std::string emitterName);
	};
}
