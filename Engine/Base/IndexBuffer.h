#pragma once
#include <vector>
#include <wrl.h>
#include <d3d12.h>

namespace IFE
{
	class IndexBuffer
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff_ = nullptr;
		std::vector<uint32_t> indices_;
		D3D12_INDEX_BUFFER_VIEW ibView_{};
	public:
		void SetIndex(const std::vector<uint32_t>& indices);
		void SetIndex(uint32_t indices[], size_t indexCount);
		void Initialize();
		D3D12_INDEX_BUFFER_VIEW* GetIBView();
		size_t GetSize();
		std::vector<uint32_t> GetIndex();
	};
}
