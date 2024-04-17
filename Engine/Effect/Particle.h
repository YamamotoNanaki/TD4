#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>
#include "ConstStruct.h"
#include "IFMath.h"
#include "ComponentManager.h"
#include "ConstBuffer.h"
#include "VertexBuffer.h"
#include "Emitter.h"
#include "IFETime.h"

#pragma comment(lib,"d3d12.lib")

namespace IFE
{
	class TransformParticle;
	class Particle : public ComponentManagerParticle
	{
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		template<class T> using vector = std::vector<T>;
	public:
		static VertexBuffer<VertexPos>vb;
		TransformParticle* transform = nullptr;
		Emitter* emitter = nullptr;
		bool isActive = true;
		bool DrawFlag = true;
		bool deleteFlag = false;
		FrameCountTime timer;

	public:
		Particle();
		void Initialize();
		static void StaticInitialize();
		void SetComponent(Component* component);
		void SetComponentFront(Component* component);
		void Update();
		static void DrawBefore();
		void Draw();
		~Particle();
	};
}