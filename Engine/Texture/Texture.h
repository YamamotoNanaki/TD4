#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <string>
#pragma warning(push)
#pragma warning(disable:26812)
#include <DirectXTex.h>
#pragma warning(pop)

namespace IFE
{
	struct Texture
	{
	public:
		Microsoft::WRL::ComPtr<ID3D12Resource> texbuff_ = nullptr;
		D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle_{};
		D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle_{};
		std::string texName_{};
		std::string name_{};
		bool free_ = false;

	public:
		void SetTexture(uint32_t rootParameterIndex);
		Texture* GetPtr();
	};
}
