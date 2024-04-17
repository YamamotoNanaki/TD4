#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <vector>

namespace IFE
{
	class StructuredBuffer
	{
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_ = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> inBuffer_ = nullptr;
	public:
		void Initialize(size_t classSize, size_t size);
		template <class T>
		void Map(std::vector<T> data);
		void SetComputeRootDescriptorTable(uint32_t rootParameterIndex,ID3D12GraphicsCommandList* cmdList);
		void SetGraphicsRootDescriptorTable(uint32_t rootParameterIndex);
	private:
	};
	template<class T>
	inline void StructuredBuffer::Map(std::vector<T> data)
	{
		T* cbuff = nullptr;
		inBuffer_->Map(0, nullptr, (void**)&cbuff);
		copy(data.begin(), data.end(), cbuff);
		inBuffer_->Unmap(0, nullptr);
	}
}
