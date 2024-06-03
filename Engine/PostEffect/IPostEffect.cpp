#include "IPostEffect.h"
#include "WindowsAPI.h"
#include "GraphicsAPI.h"
#include "JsonManager.h"
#include "TextureManager.h"
#include <cassert>
#include <d3dx12.h>
#include <d3dcompiler.h>

void IFE::IPostEffect::SetInitParams(int16_t texSize)
{
	texSize_ = texSize;
	tex_.resize(texSize_);
}

void IFE::IPostEffect::PostEffectInitialize()
{
	if (texSize_ == 0)SetInitParams(1);

	Initialize();

	HRESULT result;

	ID3D12Device* device = GraphicsAPI::Instance()->GetDevice();
	uint32_t w = WindowsAPI::Instance()->winWidth_;
	uint32_t h = WindowsAPI::Instance()->winHeight_;

	Sprite::Initialize();

	Vertex2D vertices[4] = {
		{{-1,-1,0},{0,1}},
		{{-1,+1,0},{0,0}},
		{{1,-1,0},{1,1}},
		{{1,+1,0},{1,0}},
	};

	vb_.SetVerticle(vertices, _countof(vertices));
	vb_.Initialize();

	for (uint16_t i = 0; i < texSize_; i++)
	{
		tex_[i] = TextureManager::Instance()->CreateRanderTexture(name_ + "_Render_" + std::to_string(i));
	}

	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = texSize_;

	result = device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));
	for (uint16_t i = 0; i < texSize_; i++)
	{
		device->CreateRenderTargetView(tex_[i]->texbuff_.Get(), nullptr, CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));
	}

	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT, w, h, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	D3D12_HEAP_PROPERTIES e = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3D12_CLEAR_VALUE f = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

	result = device->CreateCommittedResource(&e, D3D12_HEAP_FLAG_NONE,
		&depthResDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &f, IID_PPV_ARGS(&depthBuff));

	assert(SUCCEEDED(result));

	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;

	result = device->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	device->CreateDepthStencilView(depthBuff.Get(), &dsvDesc, descHeapDSV->GetCPUDescriptorHandleForHeapStart());
}

void IFE::IPostEffect::PostEffectUpdate()
{
	if(updateFlag_)Update();
	Sprite::Update();
}

void IFE::IPostEffect::PostEffectDraw()
{
	if (!drawFlag_)return;
	ID3D12GraphicsCommandList* cmdList = GraphicsAPI::Instance()->GetCmdList();
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//ID3D12Device* device = GraphicsAPI::Instance()->GetDevice();
	cmdList->IASetVertexBuffers(0, 1, vb_.GetVBView());
	gp_->SetDrawBlendMode();

	Draw();

	cmdList->DrawInstanced(4, 1, 0, 0);
}

void IFE::IPostEffect::PostEffectDrawBefore()
{
	ID3D12Device* device = GraphicsAPI::Instance()->GetDevice();
	ID3D12GraphicsCommandList* cmdList = GraphicsAPI::Instance()->GetCmdList();
	for (uint16_t i = 0; i < texSize_; i++)
	{
		D3D12_RESOURCE_BARRIER a = CD3DX12_RESOURCE_BARRIER::Transition(tex_[i]->texbuff_.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
		cmdList->ResourceBarrier(1, &a);
	}
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtvH;
	for (uint16_t i = 0; i < texSize_; i++)
	{
		rtvH.push_back(CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));
	}
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();

	cmdList->OMSetRenderTargets(texSize_, rtvH.data(), false, &dsvH);

	auto w = WindowsAPI::Instance()->winWidth_;
	auto h = WindowsAPI::Instance()->winHeight_;

	std::vector<CD3DX12_VIEWPORT> viewport;
	std::vector<CD3DX12_RECT> rect;
	for (uint16_t i = 0; i < texSize_; i++)
	{
		viewport.push_back(CD3DX12_VIEWPORT(0.f, 0.f, (float)w, (float)h));
		rect.push_back(CD3DX12_RECT(0, 0, (LONG)w, (LONG)h));
	}


	cmdList->RSSetViewports(texSize_, viewport.data());
	cmdList->RSSetScissorRects(texSize_, rect.data());

	for (uint16_t i = 0; i < texSize_; i++)
	{
		cmdList->ClearRenderTargetView(rtvH[i], clearColor, 0, nullptr);
	}
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	DrawBefore();

	rtvH.clear();
	viewport.clear();
	rect.clear();
}

void IFE::IPostEffect::PostEffectDrawAfter()
{
	for (uint16_t i = 0; i < texSize_; i++)
	{
		D3D12_RESOURCE_BARRIER a = CD3DX12_RESOURCE_BARRIER::Transition(tex_[i]->texbuff_.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		GraphicsAPI::Instance()->GetCmdList()->ResourceBarrier(1, &a);
	}
	DrawAfter();
}
