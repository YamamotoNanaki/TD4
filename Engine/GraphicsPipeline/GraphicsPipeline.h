#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <string>
#include <array>
#include <vector>

namespace IFE
{
	class GraphicsPipeline
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature_;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate_;

	public:
		uint8_t pipelineNum_ = 0;
		std::string name_ = "";

	public:
		void SetDrawBlendMode();

		void SetRootSignature(ID3D12RootSignature* rootsignature);
		void SetPipelineState(ID3D12PipelineState* pipelinestate);
	};
}
