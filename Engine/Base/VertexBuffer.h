#pragma once
#include <vector>
#include <array>
#include <wrl.h>
#include <d3d12.h>
#include <cassert>
#include "GraphicsAPI.h"

namespace IFE
{
	template <class T>
	class VertexBuffer
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;
		std::vector<T> vertices_;
		D3D12_VERTEX_BUFFER_VIEW vbView_{};

	public:
		void Reset();
		void SetVerticle(const std::vector<T>& vertices);
		void SetVerticle(T vertices[], size_t vertexCount);
		void Initialize();
		void Transfer(T vertices[], size_t vertexCount);
		void Transfer(const std::vector<T>& vertices);
		D3D12_VERTEX_BUFFER_VIEW* GetVBView();
		size_t GetSize();
		std::vector<T> GetVertex();
	};

	template<class T>
	inline void VertexBuffer<T>::Reset()
	{
		vertices_.clear();
	}

	template<class T>
	inline void VertexBuffer<T>::SetVerticle(const std::vector<T>& vertices)
	{
		Reset();
		vertices_ = std::move(vertices);
	}

	template<class T>
	inline void VertexBuffer<T>::SetVerticle(T vertices[], size_t vertexCount)
	{
		Reset();
		for (int32_t i = 0; i < vertexCount; i++)
		{
			vertices_.push_back(vertices[i]);
		}
	}

	template<class T>
	inline void VertexBuffer<T>::Initialize()
	{
		if (vertices_.size() == 0)return;
		// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
		uint32_t sizeVB = static_cast<uint32_t>(sizeof(vertices_[0]) * vertices_.size());

		// 頂点バッファの設定
		D3D12_HEAP_PROPERTIES heapProp{};   // ヒープ設定
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

		D3D12_RESOURCE_DESC resDesc{};  // リソース設定
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Width = sizeVB; // 頂点データ全体のサイズ
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// 頂点バッファの生成
		ID3D12Device* device = GraphicsAPI::Instance()->GetDevice();
		HRESULT result = device->CreateCommittedResource(
			&heapProp, // ヒープ設定
			D3D12_HEAP_FLAG_NONE, &resDesc, // リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff_));
		assert(SUCCEEDED(result));
		// GPU上のバッファに対応した仮想メモリを取得
		T* vertMap = nullptr;
		result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
		assert(SUCCEEDED(result));

		// 全頂点に対して
		for (int32_t i = 0; i < vertices_.size(); i++)
		{
			vertMap[i] = vertices_[i];   // 座標をコピー
		}

		// マップを解除
		vertBuff_->Unmap(0, nullptr);

		// 頂点バッファビューの作成

		vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
		vbView_.SizeInBytes = sizeVB;
		vbView_.StrideInBytes = sizeof(vertices_[0]);
	}
	template<class T>
	inline void VertexBuffer<T>::Transfer(T vertices[], size_t vertexCount)
	{
		T* vertMap = nullptr;
		if (!vertBuff_)return;
#ifdef _DEBUG
		HRESULT result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
		assert(SUCCEEDED(result));
#else
		vertBuff_->Map(0, nullptr, (void**)&vertMap);
#endif

		// 全頂点に対して
		for (int32_t i = 0; i < vertexCount; i++)
		{
			vertMap[i] = vertices[i];   // 座標をコピー
		}

		// マップを解除
		vertBuff_->Unmap(0, nullptr);
		Reset();
		SetVerticle(vertices, vertexCount);
	}
	template<class T>
	inline void VertexBuffer<T>::Transfer(const std::vector<T>& vertices)
	{
		T* vertMap = nullptr;
#ifdef _DEBUG
		HRESULT result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
		assert(SUCCEEDED(result));
#else
		vertBuff_->Map(0, nullptr, (void**)&vertMap);
#endif

		// 全頂点に対して
		for (int32_t i = 0; i < vertices.size(); i++)
		{
			vertMap[i] = vertices[i];   // 座標をコピー
		}

		// マップを解除
		vertBuff_->Unmap(0, nullptr);
		vertices_ = std::move(vertices);
	}
	template<class T>
	inline D3D12_VERTEX_BUFFER_VIEW* VertexBuffer<T>::GetVBView()
	{
		return &vbView_;
	}
	template<class T>
	inline size_t VertexBuffer<T>::GetSize()
	{
		return vertices_.size();
	}
	template<class T>
	inline std::vector<T> VertexBuffer<T>::GetVertex()
	{
		return vertices_;
	}
}
