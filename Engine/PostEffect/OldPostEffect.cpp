#include "OldPostEffect.h"
#include "WindowsAPI.h"
#include "GraphicsAPI.h"
#include "JsonManager.h"
#include <cassert>
#include <d3dx12.h>
#include <d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

using namespace IFE;
using namespace Microsoft::WRL;

const float OldPostEffect::clearColor[4] = { 0,0,0,0.0f };

OldPostEffect::OldPostEffect() :Sprite() {}

IFE::OldPostEffect::~OldPostEffect()
{
}

void OldPostEffect::Draw(bool add)
{
	ID3D12GraphicsCommandList* cmdList = GraphicsAPI::Instance()->GetCmdList();
	if (add)
	{
		cmdList->SetPipelineState(pipelineStateAdd.Get());
	}
	else
	{
		cmdList->SetPipelineState(pipelineState.Get());
	}
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	ID3D12Device* device = GraphicsAPI::Instance()->GetDevice();

	cmdList->IASetVertexBuffers(0, 1, vb_.GetVBView());

	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	//デスクリプタヒープをセット
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 定数バッファビューをセット
	buffer_.SetConstBuffView(0);
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 0, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	//cmdList->SetGraphicsRootDescriptorTable(2, CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 1, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	//cmdList->SetGraphicsRootDescriptorTable(3, CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 2, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	// 描画コマンド
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void IFE::OldPostEffect::Update()
{
	Sprite::Update();
}

void OldPostEffect::Initialize()
{
	HRESULT result;

	ID3D12Device* device = GraphicsAPI::Instance()->GetDevice();
	uint32_t w = WindowsAPI::Instance()->winWidth_;
	uint32_t h = WindowsAPI::Instance()->winHeight_;

	Sprite::Initialize();

	SetVBInit();

	D3D12_RESOURCE_DESC texresDesc{};
	texresDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texresDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	texresDesc.Width = w;
	texresDesc.Height = h;
	texresDesc.DepthOrArraySize = 1;
	texresDesc.MipLevels = 1;
	texresDesc.SampleDesc.Count = 1;
	texresDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	D3D12_HEAP_PROPERTIES c{};
	c.Type = D3D12_HEAP_TYPE_DEFAULT;

	D3D12_CLEAR_VALUE d = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor);

	for (uint16_t i = 0; i < 1; i++)
	{
		result = device->CreateCommittedResource(
			&c, D3D12_HEAP_FLAG_NONE, &texresDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&d, IID_PPV_ARGS(&texBuff[i]));
		assert(SUCCEEDED(result));
	}

	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;

	result = device->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	for (uint16_t i = 0; i < 1; i++)
	{
		device->CreateShaderResourceView(texBuff[i].Get(), &srvDesc, CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeapSRV->GetCPUDescriptorHandleForHeapStart(), i, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	}
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;

	result = device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));
	for (uint16_t i = 0; i < 1; i++)
	{
		device->CreateRenderTargetView(texBuff[i].Get(), nullptr, CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));
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

	CreateGraphicsPipelineState();

	constMapPostEffect = buffer_.GetCBMapObject();
	assert(SUCCEEDED(result));

	constMapPostEffect->NFocusWidth = 0.03f;
	constMapPostEffect->FFocusWidth = 0.04f;
	constMapPostEffect->FocusDepth = 0.08f;
}

void OldPostEffect::DrawBefore()
{
	ID3D12Device* device = GraphicsAPI::Instance()->GetDevice();
	ID3D12GraphicsCommandList* cmdList = GraphicsAPI::Instance()->GetCmdList();
	for (uint16_t i = 0; i < 1; i++)
	{
		D3D12_RESOURCE_BARRIER a = CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
		cmdList->ResourceBarrier(1, &a);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE rtvH[1];
	for (uint16_t i = 0; i < 1; i++)
	{
		rtvH[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	}
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();

	cmdList->OMSetRenderTargets(1, rtvH, false, &dsvH);

	auto w = WindowsAPI::Instance()->winWidth_;
	auto h = WindowsAPI::Instance()->winHeight_;

	CD3DX12_VIEWPORT viewport[1];
	CD3DX12_RECT rect[1];
	for (uint16_t i = 0; i < 1; i++)
	{
		viewport[i] = CD3DX12_VIEWPORT(0.f, 0.f, (float)w, (float)h);
		rect[i] = CD3DX12_RECT(0, 0, (LONG)w, (LONG)h);
	}


	cmdList->RSSetViewports(1, viewport);
	cmdList->RSSetScissorRects(1, rect);

	for (uint16_t i = 0; i < 1; i++)
	{
		cmdList->ClearRenderTargetView(rtvH[i], clearColor, 0, nullptr);
	}
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

void OldPostEffect::DrawAfter()
{
	for (uint16_t i = 0; i < 1; i++)
	{
		D3D12_RESOURCE_BARRIER a = CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		GraphicsAPI::Instance()->GetCmdList()->ResourceBarrier(1, &a);
	}
}

void IFE::OldPostEffect::SetRGBShift(float shift)
{
	constMapPostEffect->sigma = shift;
}

void IFE::OldPostEffect::SetContrast(float contrast)
{
	constMapPostEffect->contrast = contrast;
}

void IFE::OldPostEffect::SetBrightness(float brightness)
{
	constMapPostEffect->brightness = brightness;
}

void IFE::OldPostEffect::SetVBInit()
{
	Vertex2D vertices[4] = {
		{{-1,-1,0},{0,1}},
		{{-1,+1,0},{0,0}},
		{{1,-1,0},{1,1}},
		{{1,+1,0},{1,0}},
	};

	vb_.SetVerticle(vertices, _countof(vertices));
	vb_.Initialize();
}

void IFE::OldPostEffect::SetVBGame()
{
	Vertex2D vertices[4] = {
		{{-1,-1,0},{0,1}},
		{{-1,+1,0},{0,0}},
		{{0,-1,0},{1,1}},
		{{0,+1,0},{1,0}},
	};

	vb_.SetVerticle(vertices, _countof(vertices));
	vb_.Initialize();
}

#ifdef InverseEditorMode
#else
#include "ImguiManager.h"
void IFE::OldPostEffect::DebugGUI()
{
	auto im = ImguiManager::Instance();
	im->NewGUI("PostEffect");

	im->DragFloatGUI(&constMapPostEffect->NFocusWidth, "NFocus", 0.0005f, 0, 0.1f);
	im->DragFloatGUI(&constMapPostEffect->FFocusWidth, "FFocus", 0.0005f, 0, 0.1f);
	im->DragFloatGUI(&constMapPostEffect->FocusDepth, "FocusDepth", 0.0005f, 0, 0.1f);

	im->EndGUI();
}

void IFE::OldPostEffect::OutputScene()
{
	auto jm = JsonManager::Instance();

	jm->JsonReset();
	auto& json = jm->GetJsonData();

	json["NFocus"] = constMapPostEffect->NFocusWidth;
	json["FFocus"] = constMapPostEffect->FFocusWidth;
	json["FocusDepth"] = constMapPostEffect->FocusDepth;

	jm->Output("PostEffectManager");
}

#endif
void IFE::OldPostEffect::LoadingScene()
{
	auto jm = JsonManager::Instance();
	jm->Input("PostEffectManager");
	if (jm->IsError())
	{
		return;
	}
	auto json = jm->GetJsonData();

	constMapPostEffect->NFocusWidth = json["NFocus"];
	constMapPostEffect->FFocusWidth = json["FFocus"];
	constMapPostEffect->FocusDepth = json["FocusDepth"];
}

//void IFE::OldPostEffect::SetGrayscale(bool gray)
//{
//	constMapPostEffect->gray = gray;
//}

//void IFE::OldPostEffect::SetSepia(float sepia)
//{
//	constMapPostEffect->sepia = sepia;
//}

void OldPostEffect::CreateGraphicsPipelineState()
{
	ID3D12Device* device = GraphicsAPI::Instance()->GetDevice();

	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob>vsBlob;
	ComPtr<ID3DBlob>psBlob;
	ComPtr<ID3DBlob>errorBlob;

	result = D3DCompileFromFile(
		L"Data/Shaders/PostEffectVS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());

		assert(0);
	}
	result = D3DCompileFromFile(
		L"Data/Shaders/PostEffectPS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());

		assert(0);
	}


	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS; // 常に上書きルール

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	//gpipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	//gpipeline.RTVFormats[2] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV0;
	descRangeSRV0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ
	//CD3DX12_DESCRIPTOR_RANGE descRangeSRV1;
	//descRangeSRV1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1); // t1 レジスタ
	//CD3DX12_DESCRIPTOR_RANGE descRangeSRV2;
	//descRangeSRV2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2); // t1 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV0, D3D12_SHADER_VISIBILITY_ALL);
	//rootparams[2].InitAsDescriptorTable(1, &descRangeSRV1, D3D12_SHADER_VISIBILITY_ALL);
	//rootparams[3].InitAsDescriptorTable(1, &descRangeSRV2, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT); // s0 レジスタ

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));

	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = rootSignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
	blenddesc.SrcBlend = D3D12_BLEND_ONE;				//ソースの値を100%使う
	blenddesc.DestBlend = D3D12_BLEND_ONE;				//ソースの値を100%使う
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;			//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;			//デストの値を  0%使う
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	gpipeline.BlendState.RenderTarget[1] = blenddesc;
	gpipeline.BlendState.RenderTarget[2] = blenddesc;
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineStateAdd));
	assert(SUCCEEDED(result));
}
