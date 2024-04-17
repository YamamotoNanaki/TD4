#include "GraphicsPipeline.h"
#include <d3dcompiler.h>
#include "Debug.h"
#include "StringUtil.h"
#include "GraphicsAPI.h"
#include "TextureManager.h"
#include <cassert>

using namespace std;
using namespace IFE;

void IFE::GraphicsPipeline::SetDrawBlendMode()
{
	ID3D12GraphicsCommandList* commandList = GraphicsAPI::Instance()->GetCmdList();
	commandList->SetGraphicsRootSignature(rootsignature_.Get());
	commandList->SetPipelineState(pipelinestate_.Get());
}

void IFE::GraphicsPipeline::SetRootSignature(ID3D12RootSignature* rootsignature)
{
	rootsignature_ = rootsignature;
}

void IFE::GraphicsPipeline::SetPipelineState(ID3D12PipelineState* pipelinestate)
{
	pipelinestate_ = pipelinestate;
}
