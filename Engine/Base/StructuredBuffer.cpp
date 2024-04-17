#include "StructuredBuffer.h"
#include "GraphicsAPI.h"
#include <d3dx12.h>
#include <cassert>

void IFE::StructuredBuffer::Initialize(size_t classSize, size_t size)
{
	HRESULT result = S_OK;

	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;
	descHeapDesc.NumDescriptors = 2;//UAV,SRV
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	result = GraphicsAPI::GetDevice()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descriptorHeap_));
	assert(SUCCEEDED(result));

	CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_UPLOAD);// = {};
	D3D12_RESOURCE_DESC resDesc = {};

	//入力バッファ作成
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.Width = classSize * size;//入力サイズを計算
	resDesc.DepthOrArraySize = 1;
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Height = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	result = GraphicsAPI::GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&inBuffer_));

	assert(SUCCEEDED(result));


	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;//バッファとして
	srvDesc.Buffer.NumElements = (UINT)size;//要素の総数
	srvDesc.Buffer.StructureByteStride = (UINT)classSize;//1個当たりの大きさ
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	auto handle = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += GraphicsAPI::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	GraphicsAPI::GetDevice()->CreateShaderResourceView(inBuffer_.Get(), &srvDesc, handle);
}

void IFE::StructuredBuffer::SetComputeRootDescriptorTable(uint32_t rootParameterIndex, ID3D12GraphicsCommandList* cmdList)
{
	ID3D12DescriptorHeap* descHeaps[] = { descriptorHeap_.Get() };
	cmdList->SetDescriptorHeaps(1, descHeaps);//ディスクリプタヒープのセット

	//ルートパラメータのセット
	cmdList->SetComputeRootDescriptorTable(rootParameterIndex, descriptorHeap_->GetGPUDescriptorHandleForHeapStart());
}

void IFE::StructuredBuffer::SetGraphicsRootDescriptorTable(uint32_t rootParameterIndex)
{
	ID3D12DescriptorHeap* descHeaps[] = { descriptorHeap_.Get() };
	auto cmdList = GraphicsAPI::Instance()->GetCmdList();
	cmdList->SetDescriptorHeaps(1, descHeaps);//ディスクリプタヒープのセット

	//ルートパラメータのセット
	cmdList->SetGraphicsRootDescriptorTable(rootParameterIndex, descriptorHeap_->GetGPUDescriptorHandleForHeapStart());
}
