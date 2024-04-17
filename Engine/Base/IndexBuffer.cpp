#include "IndexBuffer.h"
#include "GraphicsAPI.h"

using namespace IFE;

void IFE::IndexBuffer::SetIndex(const std::vector<uint32_t>& _indices)
{
	indices_ = std::move(_indices);
}

void IFE::IndexBuffer::SetIndex(uint32_t _indices[], size_t indexCount)
{
	for (int32_t i = 0; i < indexCount; i++)
	{
		this->indices_.push_back(_indices[i]);
	}
}

void IFE::IndexBuffer::Initialize()
{
	// インデックスデータ全体のサイズ
	uint32_t sizeIB = static_cast<uint32_t>(sizeof(uint32_t) * indices_.size());

	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};   // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

	// インデックスバッファの設定
	D3D12_RESOURCE_DESC resDesc{};  // リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;//インデックスバッファの生成
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	HRESULT result = GraphicsAPI::Instance()->GetDevice()->CreateCommittedResource(
		&heapProp,				//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,				//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff_));

	uint32_t* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);

	//全インデックスに対して
	for (int32_t i = 0; i < indices_.size(); i++)
	{
		indexMap[i] = indices_[i];	//インデックスをコピー
	}

	//つながりを解除
	indexBuff_->Unmap(0, nullptr);

	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R32_UINT;
	ibView_.SizeInBytes = sizeIB;
}

D3D12_INDEX_BUFFER_VIEW* IFE::IndexBuffer::GetIBView()
{
	return &ibView_;
}

size_t IFE::IndexBuffer::GetSize()
{
	return indices_.size();
}

std::vector<uint32_t> IFE::IndexBuffer::GetIndex()
{
	return indices_;
}
