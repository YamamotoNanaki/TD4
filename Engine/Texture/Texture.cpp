#include "Texture.h"
#include "TextureManager.h"
#include "GraphicsAPI.h"

using namespace IFE;

void IFE::Texture::SetTexture(uint32_t rootParameterIndex)
{
	ID3D12GraphicsCommandList* commandList = GraphicsAPI::Instance()->GetCmdList();
	ID3D12DescriptorHeap* heaps[] = { TextureManager::Instance()->GetDescriptorHeap() };
	commandList->SetDescriptorHeaps(_countof(heaps), heaps);

	commandList->SetGraphicsRootDescriptorTable((UINT)rootParameterIndex, GPUHandle_);
}

Texture* IFE::Texture::GetPtr()
{
	return this;
}
