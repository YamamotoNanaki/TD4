#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>
#include <memory>
#include "ConstStruct.h"
#include "IFEMath.h"
#include "ComponentManager.h"
#include "ConstBuffer.h"
#include "VertexBuffer.h"
#include "Emitter.h"
#include "IFETime.h"

namespace IFE
{
	class TransformParticle;
	class Particle : public ComponentManager
	{
	public:
		static VertexBuffer<VertexPos>vb_;
		TransformParticle* transform_ = nullptr;
		Emitter* emitter_ = nullptr;
		float timer_;

	public:
		Particle();
		void Initialize(Float3 position);
		void Initialize(bool parentFlag);
		static void StaticInitialize();
		void SetComponent(std::unique_ptr<Component> component);
		void SetComponentFront(std::unique_ptr<Component> component);
		void Update();
		static void DrawBefore();
		void SetEmitter(Emitter* emitter);
		Float4 GetColor();
		ConstBufferBillboard GetMatrix();
		void Draw();
		~Particle();
	};
}