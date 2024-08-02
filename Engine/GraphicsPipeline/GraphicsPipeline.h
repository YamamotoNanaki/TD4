#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <string>
#include <array>
#include <vector>
#include "EditorMode.h"

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
		uint16_t inTex_;
		uint16_t outTex_;
		uint16_t addParam_;

	public:
		void SetDrawBlendMode();

		void SetRootSignature(ID3D12RootSignature* rootsignature);
		void SetPipelineState(ID3D12PipelineState* pipelinestate);
	};
}
