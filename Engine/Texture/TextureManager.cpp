#include "TextureManager.h"
#include "GraphicsAPI.h"
#include "Util.h"
#include "JsonManager.h"
#include "Debug.h"
#include "ImguiManager.h"
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXTex.h>

using namespace IFE;
using namespace DirectX;
using namespace std;

TextureManager::TextureManager()
{
	descRangeSRV_.NumDescriptors = 1;															//テクスチャ一つ
	descRangeSRV_.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;									//種別はテクスチャ
	descRangeSRV_.BaseShaderRegister = 0;														//0番スロットから
	descRangeSRV_.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
}

TextureManager* IFE::TextureManager::Instance()
{
	static TextureManager inst;
	return &inst;
}

void IFE::TextureManager::Initialize()
{
	const size_t kMaxSRVCount = 2056;

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

#ifdef _DEBUG
	HRESULT result = GraphicsAPI::GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));
#else
	GraphicsAPI::GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap_));

#endif

	textureSize_ = 0;
	for (int32_t i = 1; i < sTEX_MAX_; i++) {
		tex_[i].texbuff_.Reset();
		tex_[i].CPUHandle_.ptr = 0;
		tex_[i].GPUHandle_.ptr = 0;
		tex_[i].texName_.clear();
		tex_[i].name_.clear();
		tex_[i].free_ = false;
	}
}

Texture* IFE::TextureManager::GetTexture(const std::string& filename)
{
	for (uint16_t i = 1; i < 1000; i++)
	{
		if (tex_[i].free_ == false)continue;
		if (tex_[i].texName_ == filename)return &tex_[i];
		if (tex_[i].name_ == filename)return &tex_[i];
	}
	return nullptr;
}

[[nodiscard]] static ID3D12Resource* UpLoadTextureData(
	ID3D12Resource* texture, const DirectX::ScratchImage& mipImages)
{
	ID3D12GraphicsCommandList* cmdList = GraphicsAPI::Instance()->GetCmdList();
	ID3D12Device* device = GraphicsAPI::GetDevice();

	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	DirectX::PrepareUpload(
		device, mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(),
		subresources);
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture, 0, UINT(subresources.size()));
	ID3D12Resource* intermediateResource;
	// 定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	// 定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = intermediateSize;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT result = device->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&intermediateResource));
	assert(SUCCEEDED(result));
	UpdateSubresources(
		cmdList, texture, intermediateResource, 0, 0, UINT(subresources.size()),
		subresources.data());
	// Textureへの転送後は利用できるようD3D12_RESOURCE_STATE_COPY_DESTからD3D12_RESOURCE_STATE_GENERIC_READへ変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	cmdList->ResourceBarrier(1, &barrier);
	result;
	return intermediateResource;
}

Texture* IFE::TextureManager::LoadTexture(const std::string& filename, int32_t number)
{
	assert(textureSize_ < sTEX_MAX_ && "ヒープサイズが足りません");

	//WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	uint16_t num = 0;
	if (number == -1)
	{
		for (uint16_t i = 1; i < 1000; i++)
		{
			if (tex_[i].free_ == false)continue;
			if (tex_[i].texName_ == filename)return &tex_[i];
		}

		for (uint16_t i = 1; i < 1000; i++)
		{
			if (tex_[i].free_ == false)
			{
				num = i;
				break;
			}
		}
	}
	else
	{
		num = (uint16_t)number;
	}

	Texture newtex;
	newtex.texName_ = filename;
	string separator = ".";
	string::size_type pos = 0;
	pos = filename.find(separator, pos);
	newtex.name_ = filename.substr(0, pos);
	string file = "Data/Resources/Texture/" + filename;
	wchar_t szFile[256];
	MultiByteToWideChar(CP_ACP, 0, file.c_str(), -1, szFile, _countof(szFile));


	size_t pos1;
	string fileExt;

	pos1 = filename.rfind('.');

	if (pos1 != wstring::npos)
	{
		fileExt = filename.substr(pos1 + 1, filename.size() - pos1 - 1);
	}
	else
	{
		fileExt = "";
	}

	HRESULT result;
	if (fileExt == "dds")
	{
		result = LoadFromDDSFile(szFile, DDS_FLAGS_NONE, &metadata, scratchImg);
	}
	else
	{
		result = LoadFromWICFile(szFile, WIC_FLAGS_NONE, &metadata, scratchImg);
		metadata.format = MakeSRGB(metadata.format);
		ScratchImage mipChain{};
		// ミップマップ生成
		result = GenerateMipMaps(
			scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
			TEX_FILTER_DEFAULT, 0, mipChain);
		if (SUCCEEDED(result)) {
			scratchImg = std::move(mipChain);
			metadata = scratchImg.GetMetadata();
		}
	}

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format, metadata.width, (UINT)metadata.height, (UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);

	// ヒーププロパティ
	D3D12_HEAP_PROPERTIES heapProps{};
	heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
	ID3D12Device* device = GraphicsAPI::GetDevice();

	// テクスチャ用バッファの生成
	result = device->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &texresDesc,
		D3D12_RESOURCE_STATE_COPY_DEST, // 転送用指定
		nullptr, IID_PPV_ARGS(&newtex.texbuff_));
	assert(SUCCEEDED(result));

	ID3D12Resource* intermediateResource =
		UpLoadTextureData(newtex.texbuff_.Get(), scratchImg);
	GraphicsAPI::Instance()->ExecuteCommand();

	intermediateResource->Release();

	auto descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	newtex.CPUHandle_ = CD3DX12_CPU_DESCRIPTOR_HANDLE(srvHeap_->GetCPUDescriptorHandleForHeapStart(), num, descriptorSize);
	newtex.GPUHandle_ = CD3DX12_GPU_DESCRIPTOR_HANDLE(srvHeap_->GetGPUDescriptorHandleForHeapStart(), num, descriptorSize);
	newtex.free_ = true;

	tex_[num] = newtex;

	D3D12_RESOURCE_DESC resDesc2 = tex_[num].texbuff_->GetDesc();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};			//設定構造体
	//srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;	//RGBA
	srvDesc.Format = resDesc2.Format;					//画像読み込み
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;		//2dテクスチャ
	srvDesc.Texture2D.MipLevels = resDesc2.MipLevels;

	//ヒープの２番目にシェーダーリソースビュー作成
	device->CreateShaderResourceView(
		tex_[num].texbuff_.Get(),		//ビューと関連付けるバッファ
		&srvDesc,		//テクスチャ設定情報
		tex_[num].CPUHandle_);

	textureSize_++;
	return &tex_[num];
}

D3D12_DESCRIPTOR_RANGE& IFE::TextureManager::GetDescRangeSRV()
{
	return descRangeSRV_;
}

ID3D12DescriptorHeap* IFE::TextureManager::GetDescriptorHeap()
{
	return srvHeap_.Get();
}

#ifdef NDEBUG
#else
void IFE::TextureManager::DebugGUI()
{
	ImguiManager* im = ImguiManager::Instance();

	static bool load = false;
	static bool search = false;
	im->TextureGUI(&load, &search);
	if (load)
	{
		std::function<void(std::string)> guiFunc = [&](std::string filename)
		{
			LoadTexture(filename);
		};
		im->TextureLoadGUI(guiFunc);
	}
	if (search)
	{
		im->SearchTextureGUI(tex_);
	}
	im->ShowTextureGUI(tex_);
	im->EndGUI();
}

void IFE::TextureManager::OutputScene()
{
	JsonManager* j = JsonManager::Instance();
	j->JsonReset();
	nlohmann::json& js = j->GetJsonData();
	int32_t num = 0;
	for (uint16_t i = 1; i < 1000; i++)
	{
		if (tex_[i].free_ == false)continue;
		js["Texture"][num] = tex_[i].texName_;
		num++;
	}
	j->Output("Texture");
}
#endif

void IFE::TextureManager::LoadingScene()
{
	JsonManager* j = JsonManager::Instance();
	j->Input("Texture");
	nlohmann::json js = j->GetJsonData();
	for (auto& i : js["Texture"])
	{
		LoadTexture(i);
	}
	j->JsonReset();
}