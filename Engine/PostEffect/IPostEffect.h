#pragma once
#include "Sprite.h"
#include "ConstBuffer.h"
#include "GraphicsPipeline.h"

namespace IFE
{
	class IPostEffect : public Sprite
	{
	protected:
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>depthSRV;
		Microsoft::WRL::ComPtr<ID3D12Resource>depthBuff;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapRTV;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapDSV;
		Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineState;
		Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineStateAdd;
		Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature;

	public:
		std::string name;

	protected:
		inline static const float clearColor[4] = { 0,0,0,0 };
		std::vector<Texture*>tex_;
		int16_t texSize_ = 0;
		GraphicsPipeline* gp_;

	protected:
		virtual void Draw() = 0;
		virtual void Update() {};
		virtual void Initialize() {};
		virtual void DrawBefore() {};
		virtual void DrawAfter() {};

	public:
		void SetInitParams(int16_t texSize);
		void PostEffectInitialize();
		void PostEffectUpdate();
		void PostEffectDraw();
		void PostEffectDrawBefore();
		void PostEffectDrawAfter();
	};
}
