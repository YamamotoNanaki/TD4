#pragma once
#include "Sprite.h"
#include "ConstBuffer.h"
#include "EditorMode.h"

namespace IFE
{
	class IPostEffect : public Sprite
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource>texBuff[3];
		Microsoft::WRL::ComPtr<ID3D12Resource>depthTex;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapSRV;
		Microsoft::WRL::ComPtr<ID3D12Resource>depthBuff;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapRTV;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapDSV;
		Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineState;
		Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineStateAdd;
		Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature;
		ConstBuffer<ConstBufferPostEffect> buffer_;

	public:
		ConstBufferPostEffect* constMapPostEffect = nullptr;

	private:
		inline static const float clearColor[4] = {0,0,0,0};

		void Draw();
		void Update();
		void Initialize();
		void DrawBefore();
		void DrawAfter();
		void CreateGraphicsPipelineState();
	};
}
