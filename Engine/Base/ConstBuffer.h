#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <cassert>
#include "GraphicsAPI.h"

namespace IFE
{
	template<class T>
	class ConstBuffer
	{
	private:
		//定数バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> cbuffResource_;
		T* mapObj_ = nullptr;

		static ID3D12GraphicsCommandList* sCmdList_;
	public:
		ConstBuffer();
		~ConstBuffer();

		void SetConstBuffView(uint32_t rootParameterIndex);
		T* GetCBMapObject();
	};

	template<class T>
	ID3D12GraphicsCommandList* ConstBuffer<T>::sCmdList_ = nullptr;

	template<class T>
	inline ConstBuffer<T>::ConstBuffer()
	{
		//定数バッファのヒープ設定
		D3D12_HEAP_PROPERTIES heapProp{};
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		//定数バッファのリソース設定
		D3D12_RESOURCE_DESC resdesc{};
		resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resdesc.Width = (sizeof(T) + 0xff) & ~0xff;
		resdesc.Height = 1;
		resdesc.DepthOrArraySize = 1;
		resdesc.MipLevels = 1;
		resdesc.SampleDesc.Count = 1;
		resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		HRESULT result = GraphicsAPI::Instance()->GetDevice()->CreateCommittedResource(
			&heapProp, D3D12_HEAP_FLAG_NONE, &resdesc,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&cbuffResource_));
		assert(SUCCEEDED(result));

		result = cbuffResource_->Map(0, nullptr, (void**)&mapObj_);
		assert(SUCCEEDED(result));

		sCmdList_ = GraphicsAPI::Instance()->GetCmdList();
	}
	template<class T>
	inline ConstBuffer<T>::~ConstBuffer()
	{
		cbuffResource_->Unmap(0, nullptr);
	}
	template<class T>
	inline void ConstBuffer<T>::SetConstBuffView(uint32_t rootParameterIndex)
	{
		sCmdList_->SetGraphicsRootConstantBufferView(rootParameterIndex, cbuffResource_->GetGPUVirtualAddress());
	}
	template<class T>
	inline T* ConstBuffer<T>::GetCBMapObject()
	{
		return mapObj_;
	}
}
