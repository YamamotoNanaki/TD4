#pragma once
#include "Particle.h"
#include "Texture.h"
#include "GraphicsPipeline.h"
#include <memory>
#include <list>
#include <string>
//#include "PostEffect.h"

namespace IFE
{
	class TransformParticle;
	class Emitter :public ComponentManagerEmitter
	{

	public:
		std::list<std::unique_ptr<Particle>>particles;
		bool deleteFlag = false;
		bool emitterDeleteFlag = false;
		TransformParticle* transform = nullptr;
		bool isActive = true;
		bool DrawFlag = true;
		Texture* tex = nullptr;
		std::string emitterName;
		GraphicsPipeline gp;

	public:
		//PostEffect* postEffect = nullptr;

	public:
		Emitter();
		void Initialize();
		static void StaticInitialize();
		void SetComponent(Component* component);
		void SetComponentFront(Component* component);
		void SetTexture(std::string texName);
		void Update();
		//static void DrawBefore();
		void Draw();
		~Emitter();

#ifdef _DEBUG
		void DebugGUI(bool flagdelete);
		void ComponentGUI();
		void OutputScene();
		void DebugUpdate();
#endif
	};
}
