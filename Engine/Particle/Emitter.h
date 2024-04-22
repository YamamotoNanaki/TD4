#pragma once
#include "Particle.h"
#include "Texture.h"
#include "GraphicsPipeline.h"
#include <nlohmann/json.hpp>
#include <memory>
#include <list>
#include <string>
//#include "PostEffect.h"

namespace IFE
{
	class TransformParticle;
	struct CBColor
	{
		Float4 color[400];
	};
	struct CBTrans
	{
		Matrix mat[400];
		Matrix matBillboard[400];
	};
	class Emitter :public ComponentManager
	{

	public:
		std::list<std::unique_ptr<Particle>>particles_;
		bool deleteFlag_ = false;
		bool emitterDeleteFlag_ = false;
		TransformParticle* transform_ = nullptr;
		bool isActive_ = true;
		bool DrawFlag_ = true;
		Texture* tex_ = nullptr;
		std::string emitterName_;
		GraphicsPipeline* gp_;
		float particleMaxTime_ = 1;

	public:
		//PostEffect* postEffect = nullptr;
		ConstBuffer<CBTrans> transformCB_;
		ConstBuffer<CBColor> colorCB_;
		CBTrans* transformMap = nullptr;
		CBColor* colorMap = nullptr;

	public:
		Emitter();
		void Initialize();
		void CopyValue(Emitter* ptr);
		static void StaticInitialize();
		void SetComponent(std::unique_ptr<Component> component);
		void SetComponentFront(std::unique_ptr<Component> component);
		void SetTexture(const std::string& texName);
		Particle* AddParticle(bool parentFlag);
		Particle* AddParticle(Float3 pos);
		Particle* AddParticle();
		void Update();
		//static void DrawBefore();
		void Draw();
		~Emitter();

#ifdef InverseEditorMode
#else
		void DebugGUI(bool flagdelete);
		void ComponentGUI();
		void OutputScene(nlohmann::json& j);
		void DebugUpdate();
#endif
		void LoadingScene(nlohmann::json& j);
	};
}
