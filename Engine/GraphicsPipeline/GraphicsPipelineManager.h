#pragma once
#include <list>
#include <memory>
#include "GraphicsPipeline.h"

namespace IFE
{
	enum class SHADER_COMPILE_SETTINGS
	{
		Vertex, Pixel, Geometry, Compute
	};
	enum class PIPELINE_SETTING
	{
		Normal = 0, Anim = 20, Transparent = 40, SPrite = 100
	};
	class GraphicsPipelineManager
	{
	private:
		std::list<std::unique_ptr<GraphicsPipeline>> pipelineList_;
		Microsoft::WRL::ComPtr<ID3DBlob> errorBlob_;
		std::string defaultDirectory_ = "Data/Shaders/";
		std::array<Microsoft::WRL::ComPtr<ID3DBlob>, 4> blobs_ = { nullptr,nullptr,nullptr,nullptr };
		uint8_t NumMax_ = 0;

	public:
		GraphicsPipelineManager(const GraphicsPipelineManager& obj) = delete;
		GraphicsPipelineManager& operator=(const GraphicsPipelineManager& obj) = delete;
		static GraphicsPipelineManager* Instance();

		bool ShaderCompile(const std::string& shaderName, const SHADER_COMPILE_SETTINGS& setting);
		GraphicsPipeline* CreateBasicGraphicsPipeLine();
		GraphicsPipeline* CreateAnimGraphicsPipeLine();
		GraphicsPipeline* CreateBasic2DGraphicsPipeLine();
		GraphicsPipeline* CreateBasicParticleGraphicsPipeLine();
		GraphicsPipeline* CreateTransparentParticleGraphicsPipeLine();
		GraphicsPipeline* CreateSubParticleGraphicsPipeLine();
		bool CreateGraphicsPipeline(const std::string& pipelineName, const D3D12_ROOT_SIGNATURE_DESC& rootSignatureDesc,
			D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc, uint8_t pipelineNum);
		GraphicsPipeline* GetGraphicsPipeline(const std::string& name);
		ID3DBlob* GetBlob(uint8_t num);

	private:
		GraphicsPipelineManager() {}
	};
}
