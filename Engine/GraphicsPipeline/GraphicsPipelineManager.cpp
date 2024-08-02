#include "GraphicsPipelineManager.h"
#include <d3dcompiler.h>
#include "Debug.h"
#include "StringUtil.h"
#include "GraphicsAPI.h"
#include "TextureManager.h"
#include "JsonManager.h"
#include <d3dx12.h>
#include <cassert>

#pragma comment(lib,"d3dcompiler.lib")

using namespace IFE;
using namespace std;

GraphicsPipelineManager* IFE::GraphicsPipelineManager::Instance()
{
	static GraphicsPipelineManager inst;
	return &inst;
}

bool IFE::GraphicsPipelineManager::ShaderCompile(const std::string& shaderName, const SHADER_COMPILE_SETTINGS& setting)
{
	const static LPCSTR setting_shader_type[4] = { "vs_5_0","ps_5_0","gs_5_0","cs_5_1" };
	size_t num = static_cast<size_t>(setting);
	std::wstring wname = StringToWString(shaderName);
	HRESULT result = D3DCompileFromFile(wname.c_str(),  // シェーダファイル名
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", setting_shader_type[num], // エントリーポイント名、シェーダーモデル	指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0, &blobs_[num], &errorBlob_);

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		string error;
		error.resize(errorBlob_->GetBufferSize());

		copy_n((char*)errorBlob_->GetBufferPointer(), errorBlob_->GetBufferSize(), error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		return true;
	}
	return false;
}

void* defaultDirectory_;

GraphicsPipeline* IFE::GraphicsPipelineManager::CreateBasicGraphicsPipeLine()
{
	return CreateObjectGraphicsPipeLine("ModelVS", "ModelGS", "ModelPS", "Model_DEFALUT", 0, 1, 2);
}

GraphicsPipeline* IFE::GraphicsPipelineManager::CreateObjectGraphicsPipeLine(std::string v, std::string g, std::string p, std::string name, int16_t addRootParam, int16_t inputTexNum, int16_t outputTexNum)
{
	string vs = defaultDirectory_ + v + ".hlsl";
	ShaderCompile(vs, SHADER_COMPILE_SETTINGS::Vertex);
	string ps = defaultDirectory_ + p + ".hlsl";
	ShaderCompile(ps, SHADER_COMPILE_SETTINGS::Pixel);
	string gs = defaultDirectory_ + g + ".hlsl";
	ShaderCompile(gs, SHADER_COMPILE_SETTINGS::Geometry);

	vector<CD3DX12_ROOT_PARAMETER> rootParams;
	rootParams.resize(size_t(5 + inputTexNum + addRootParam));

	for (size_t i = 0; i < 5; i++)
	{
		rootParams[i].InitAsConstantBufferView((UINT)i, 0, D3D12_SHADER_VISIBILITY_ALL);
	}
	// デスクリプタレンジ
	std::vector<CD3DX12_DESCRIPTOR_RANGE> descRangeSRV;
	descRangeSRV.resize(inputTexNum);
	for (size_t i = 0; i < inputTexNum; i++)
	{
		descRangeSRV[i].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, UINT(i)); // t0 レジスタ
		rootParams[i + 5].InitAsDescriptorTable(1, &descRangeSRV[i], D3D12_SHADER_VISIBILITY_ALL);
	}

	for (size_t i = 0; i < addRootParam; i++)
	{
		UINT registerNum = UINT(i + 5);
		rootParams[registerNum + inputTexNum].InitAsConstantBufferView(registerNum, 0, D3D12_SHADER_VISIBILITY_ALL);
	}


	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
	inputLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	inputLayout.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	inputLayout.push_back({ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//横繰り返し
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//縦繰り返し
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//奥行繰り返し
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//リニア補完
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;												//ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//ピクセルシェーダーからのみ可視

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParams.front();
	rootSignatureDesc.NumParameters = (UINT)rootParams.size();
	//テクスチャ追加
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	pipelineDesc.VS.pShaderBytecode = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Vertex]->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Vertex]->GetBufferSize();
	if (blobs_[(size_t)SHADER_COMPILE_SETTINGS::Geometry] != nullptr)
	{
		pipelineDesc.GS.pShaderBytecode = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Geometry]->GetBufferPointer();
		pipelineDesc.GS.BytecodeLength = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Geometry]->GetBufferSize();
	}
	pipelineDesc.PS.pShaderBytecode = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Pixel]->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Pixel]->GetBufferSize();
	//デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthEnable = true;		//深度テストを行う
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;		//深度値フォーマット

	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;  // 背面をカリング
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//書き込み許可

	pipelineDesc.InputLayout.pInputElementDescs = inputLayout.data();
	pipelineDesc.InputLayout.NumElements = (UINT)inputLayout.size();

	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	pipelineDesc.NumRenderTargets = (UINT)outputTexNum; // 描画対象は1つ
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc;
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	blendDesc.BlendEnable = true;						//ブレンドを有効にする
	blendDesc.LogicOpEnable = false;  // LogicOpを無効にする
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//加算
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;			//ソースの値を100%使う
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;			//デストの値を  0%使う

	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
	blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//ソースのアルファ値
	blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		//1.0f-ソースのアルファ値

	for (size_t i = 0; i < outputTexNum; i++)
	{
		pipelineDesc.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
		pipelineDesc.BlendState.RenderTarget[i] = blendDesc;
	}
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	static uint8_t number = 0;

	if (CreateGraphicsPipeline(name, rootSignatureDesc, pipelineDesc, (uint8_t)PIPELINE_SETTING::Normal + number))
	{
		return nullptr;
	}
	number++;
	if (number >= uint8_t(PIPELINE_SETTING::Anim))number = 1;
	auto pipeline = pipelineList_.back().get();

	pipeline->inTex_ = inputTexNum;
	pipeline->outTex_ = outputTexNum;
	pipeline->addParam_ = addRootParam;
	return pipeline;
}

GraphicsPipeline* IFE::GraphicsPipelineManager::CreateObjectGraphicsPipeLine(std::string pipelineName, int16_t addRootParam, int16_t inputTexNum, int16_t outputTexNum)
{
	return CreateObjectGraphicsPipeLine(pipelineName + "VS", pipelineName + "GS", pipelineName + "PS", pipelineName, addRootParam, inputTexNum, outputTexNum);
}

GraphicsPipeline* IFE::GraphicsPipelineManager::CreateAnimGraphicsPipeLine()
{
	return CreateAnimObjectGraphicsPipeLine("ModelAnimVS", "ModelAnimGS", "ModelAnimPS", "ModelAnim_DEFALUT", 0, 1, 2);
}

GraphicsPipeline* IFE::GraphicsPipelineManager::CreateAnimObjectGraphicsPipeLine(std::string v, std::string g, std::string p, std::string name, int16_t addRootParam, int16_t inputTexNum, int16_t outputTexNum)
{
	string vs = defaultDirectory_ + v + ".hlsl";
	ShaderCompile(vs, SHADER_COMPILE_SETTINGS::Vertex);
	string ps = defaultDirectory_ + p + ".hlsl";
	ShaderCompile(ps, SHADER_COMPILE_SETTINGS::Pixel);
	string gs = defaultDirectory_ + g + ".hlsl";
	ShaderCompile(gs, SHADER_COMPILE_SETTINGS::Geometry);

	vector<CD3DX12_ROOT_PARAMETER> rootParams;
	rootParams.resize(size_t(6 + inputTexNum + addRootParam));

	for (size_t i = 0; i < 5; i++)
	{
		rootParams[i].InitAsConstantBufferView((UINT)i, 0, D3D12_SHADER_VISIBILITY_ALL);
	}
	// デスクリプタレンジ
	std::vector<CD3DX12_DESCRIPTOR_RANGE> descRangeSRV;
	descRangeSRV.resize(inputTexNum);
	for (size_t i = 0; i < inputTexNum; i++)
	{
		descRangeSRV[i].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, UINT(i)); // t0 レジスタ
		rootParams[i + 5].InitAsDescriptorTable(1, &descRangeSRV[i], D3D12_SHADER_VISIBILITY_ALL);
	}

	for (size_t i = 0; i < addRootParam + 1; i++)
	{
		UINT registerNum = UINT(i + 5);
		rootParams[registerNum + inputTexNum].InitAsConstantBufferView(registerNum, 0, D3D12_SHADER_VISIBILITY_ALL);
	}


	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
	inputLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	inputLayout.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	inputLayout.push_back({ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	inputLayout.push_back({ "BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	inputLayout.push_back({ "BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//横繰り返し
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//縦繰り返し
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//奥行繰り返し
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//リニア補完
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;												//ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//ピクセルシェーダーからのみ可視

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParams.front();
	rootSignatureDesc.NumParameters = (UINT)rootParams.size();
	//テクスチャ追加
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	pipelineDesc.VS.pShaderBytecode = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Vertex]->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Vertex]->GetBufferSize();
	if (blobs_[(size_t)SHADER_COMPILE_SETTINGS::Geometry] != nullptr)
	{
		pipelineDesc.GS.pShaderBytecode = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Geometry]->GetBufferPointer();
		pipelineDesc.GS.BytecodeLength = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Geometry]->GetBufferSize();
	}
	pipelineDesc.PS.pShaderBytecode = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Pixel]->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Pixel]->GetBufferSize();
	//デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthEnable = true;		//深度テストを行う
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;		//深度値フォーマット

	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;  // 背面をカリング
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//書き込み許可

	pipelineDesc.InputLayout.pInputElementDescs = inputLayout.data();
	pipelineDesc.InputLayout.NumElements = (UINT)inputLayout.size();

	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	pipelineDesc.NumRenderTargets = (UINT)outputTexNum; // 描画対象は1つ
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc;
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	blendDesc.BlendEnable = true;						//ブレンドを有効にする
	blendDesc.LogicOpEnable = false;  // LogicOpを無効にする
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//加算
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;			//ソースの値を100%使う
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;			//デストの値を  0%使う

	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
	blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//ソースのアルファ値
	blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		//1.0f-ソースのアルファ値

	for (size_t i = 0; i < outputTexNum; i++)
	{
		pipelineDesc.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
		pipelineDesc.BlendState.RenderTarget[i] = blendDesc;
	}
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	static uint8_t number = 0;

	if (CreateGraphicsPipeline(name, rootSignatureDesc, pipelineDesc, (uint8_t)PIPELINE_SETTING::Anim + number))
	{
		return nullptr;
	}
	number++;
	if (number >= uint8_t(PIPELINE_SETTING::Transparent))number = 1;

	auto pipeline = pipelineList_.back().get();

	pipeline->inTex_ = inputTexNum;
	pipeline->outTex_ = outputTexNum;
	pipeline->addParam_ = addRootParam;
	return pipeline;
}

GraphicsPipeline* IFE::GraphicsPipelineManager::CreateAnimObjectGraphicsPipeLine(std::string pipelineName, int16_t addRootParam, int16_t inputTexNum, int16_t outputTexNum)
{
	return CreateAnimObjectGraphicsPipeLine(pipelineName + "VS", pipelineName + "GS", pipelineName + "PS", pipelineName, addRootParam, inputTexNum, outputTexNum);
}

GraphicsPipeline* IFE::GraphicsPipelineManager::CreateBasic2DGraphicsPipeLine()
{
	string vs = defaultDirectory_ + "SpriteVS.hlsl";
	ShaderCompile(vs, SHADER_COMPILE_SETTINGS::Vertex);
	string ps = defaultDirectory_ + "SpritePS.hlsl";
	ShaderCompile(ps, SHADER_COMPILE_SETTINGS::Pixel);

	vector<D3D12_ROOT_PARAMETER> rootParams;

	for (size_t i = 0; i < 2; i++)
	{
		D3D12_ROOT_PARAMETER rootParamSeed;
		//定数用
		rootParamSeed.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;				//種類
		rootParamSeed.Descriptor.ShaderRegister = (UINT)i;								//デスクリプタレンジ
		rootParamSeed.Descriptor.RegisterSpace = 0;									//デスクリプタレンジ数
		rootParamSeed.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//すべてのシェーダーから見える
		rootParams.push_back(rootParamSeed);
	}

	D3D12_ROOT_PARAMETER rootParamSeed2;
	rootParamSeed2.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		//種類
	rootParamSeed2.DescriptorTable.pDescriptorRanges = &TextureManager::Instance()->GetDescRangeSRV();				//デスクリプタレンジ
	rootParamSeed2.DescriptorTable.NumDescriptorRanges = 1;							//デスクリプタレンジ数
	rootParamSeed2.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;					//すべてのシェーダーから見える
	rootParams.push_back(rootParamSeed2);


	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
	inputLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	inputLayout.push_back({ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//横繰り返し
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//縦繰り返し
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//奥行繰り返し
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//リニア補完
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;												//ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//ピクセルシェーダーからのみ可視

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParams.front();
	rootSignatureDesc.NumParameters = (UINT)rootParams.size();
	//テクスチャ追加
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	pipelineDesc.VS.pShaderBytecode = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Vertex]->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Vertex]->GetBufferSize();
	if (blobs_[(size_t)SHADER_COMPILE_SETTINGS::Geometry] != nullptr)
	{
		pipelineDesc.GS.pShaderBytecode = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Geometry]->GetBufferPointer();
		pipelineDesc.GS.BytecodeLength = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Geometry]->GetBufferSize();
	}
	pipelineDesc.PS.pShaderBytecode = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Pixel]->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Pixel]->GetBufferSize();
	//デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthEnable = false;		//深度テストを行わない
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;		//深度値フォーマット

	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // 背面をカリングしない
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//書き込み許可

	pipelineDesc.InputLayout.pInputElementDescs = inputLayout.data();
	pipelineDesc.InputLayout.NumElements = (UINT)inputLayout.size();

	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = pipelineDesc.BlendState.RenderTarget[0];
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	blendDesc.BlendEnable = true;						//ブレンドを有効にする
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//加算
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;			//ソースの値を100%使う
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;			//デストの値を  0%使う

	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
	blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//ソースのアルファ値
	blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		//1.0f-ソースのアルファ値

	if (CreateGraphicsPipeline("Sprite_DEFALUT", rootSignatureDesc, pipelineDesc, (uint8_t)PIPELINE_SETTING::Sprite))
	{
		return nullptr;
	}
	auto pipeline = pipelineList_.back().get();

	pipeline->inTex_ = 0;
	pipeline->outTex_ = 0;
	pipeline->addParam_ = 0;
	return pipeline;
}

GraphicsPipeline* IFE::GraphicsPipelineManager::CreateBasicParticleGraphicsPipeLine()
{
	string vs = defaultDirectory_ + "ParticleVS.hlsl";
	ShaderCompile(vs, SHADER_COMPILE_SETTINGS::Vertex);
	string ps = defaultDirectory_ + "ParticlePS.hlsl";
	ShaderCompile(ps, SHADER_COMPILE_SETTINGS::Pixel);
	string gs = defaultDirectory_ + "ParticleGS.hlsl";
	ShaderCompile(gs, SHADER_COMPILE_SETTINGS::Geometry);

	vector<D3D12_ROOT_PARAMETER> rootParams;

	for (size_t i = 0; i < 2; i++)
	{
		D3D12_ROOT_PARAMETER rootParamSeed;
		//定数用
		rootParamSeed.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;				//種類
		rootParamSeed.Descriptor.ShaderRegister = (UINT)i;								//デスクリプタレンジ
		rootParamSeed.Descriptor.RegisterSpace = 0;									//デスクリプタレンジ数
		rootParamSeed.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//すべてのシェーダーから見える
		rootParams.push_back(rootParamSeed);
	}

	D3D12_ROOT_PARAMETER rootParamSeed2;
	rootParamSeed2.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		//種類
	rootParamSeed2.DescriptorTable.pDescriptorRanges = &TextureManager::Instance()->GetDescRangeSRV();				//デスクリプタレンジ
	rootParamSeed2.DescriptorTable.NumDescriptorRanges = 1;							//デスクリプタレンジ数
	rootParamSeed2.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;					//すべてのシェーダーから見える
	rootParams.push_back(rootParamSeed2);


	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
	inputLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//横繰り返し
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//縦繰り返し
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//奥行繰り返し
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//リニア補完
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;												//ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//ピクセルシェーダーからのみ可視

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParams.front();
	rootSignatureDesc.NumParameters = (UINT)rootParams.size();
	//テクスチャ追加
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	pipelineDesc.VS.pShaderBytecode = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Vertex]->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Vertex]->GetBufferSize();
	if (blobs_[(size_t)SHADER_COMPILE_SETTINGS::Geometry] != nullptr)
	{
		pipelineDesc.GS.pShaderBytecode = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Geometry]->GetBufferPointer();
		pipelineDesc.GS.BytecodeLength = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Geometry]->GetBufferSize();
	}
	pipelineDesc.PS.pShaderBytecode = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Pixel]->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Pixel]->GetBufferSize();
	//デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthEnable = true;		//深度テストを行わない
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;		//深度値フォーマット

	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // 背面をカリングしない
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;	//書き込み許可

	pipelineDesc.InputLayout.pInputElementDescs = inputLayout.data();
	pipelineDesc.InputLayout.NumElements = (UINT)inputLayout.size();

	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	pipelineDesc.NumRenderTargets = 2; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = pipelineDesc.BlendState.RenderTarget[0];
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	blendDesc.BlendEnable = true;						//ブレンドを有効にする
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//加算
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;			//ソースの値を100%使う
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;			//デストの値を  0%使う

	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
	blendDesc.SrcBlend = D3D12_BLEND_ONE;				//ソースの値を100%使う
	blendDesc.DestBlend = D3D12_BLEND_ONE;				//ソースの値を100%使う

	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc2 = pipelineDesc.BlendState.RenderTarget[1];
	blendDesc2.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc2.BlendEnable = true;						//ブレンドを有効にする
	blendDesc2.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//加算
	blendDesc2.SrcBlendAlpha = D3D12_BLEND_ONE;			//ソースの値を100%使う
	blendDesc2.DestBlendAlpha = D3D12_BLEND_ZERO;			//デストの値を  0%使う
	blendDesc2.BlendOp = D3D12_BLEND_OP_ADD;				//加算
	blendDesc2.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//ソースのアルファ値
	blendDesc2.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		//1.0f-ソースのアルファ値

	if (CreateGraphicsPipeline("Particle_DEFALUT", rootSignatureDesc, pipelineDesc, (uint8_t)PIPELINE_SETTING::Transparent))
	{
		return nullptr;
	}
	auto pipeline = pipelineList_.back().get();

	pipeline->inTex_ = 0;
	pipeline->outTex_ = 0;
	pipeline->addParam_ = 0;
	return pipeline;
}

GraphicsPipeline* IFE::GraphicsPipelineManager::CreateTransparentParticleGraphicsPipeLine()
{
	string vs = defaultDirectory_ + "ParticleVS.hlsl";
	ShaderCompile(vs, SHADER_COMPILE_SETTINGS::Vertex);
	string ps = defaultDirectory_ + "ParticlePS.hlsl";
	ShaderCompile(ps, SHADER_COMPILE_SETTINGS::Pixel);
	string gs = defaultDirectory_ + "ParticleGS.hlsl";
	ShaderCompile(gs, SHADER_COMPILE_SETTINGS::Geometry);

	vector<D3D12_ROOT_PARAMETER> rootParams;

	for (size_t i = 0; i < 2; i++)
	{
		D3D12_ROOT_PARAMETER rootParamSeed;
		//定数用
		rootParamSeed.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;				//種類
		rootParamSeed.Descriptor.ShaderRegister = (UINT)i;								//デスクリプタレンジ
		rootParamSeed.Descriptor.RegisterSpace = 0;									//デスクリプタレンジ数
		rootParamSeed.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//すべてのシェーダーから見える
		rootParams.push_back(rootParamSeed);
	}

	D3D12_ROOT_PARAMETER rootParamSeed2;
	rootParamSeed2.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		//種類
	rootParamSeed2.DescriptorTable.pDescriptorRanges = &TextureManager::Instance()->GetDescRangeSRV();				//デスクリプタレンジ
	rootParamSeed2.DescriptorTable.NumDescriptorRanges = 1;							//デスクリプタレンジ数
	rootParamSeed2.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;					//すべてのシェーダーから見える
	rootParams.push_back(rootParamSeed2);


	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
	inputLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//横繰り返し
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//縦繰り返し
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//奥行繰り返し
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//リニア補完
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;												//ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//ピクセルシェーダーからのみ可視

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParams.front();
	rootSignatureDesc.NumParameters = (UINT)rootParams.size();
	//テクスチャ追加
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	pipelineDesc.VS.pShaderBytecode = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Vertex]->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Vertex]->GetBufferSize();
	if (blobs_[(size_t)SHADER_COMPILE_SETTINGS::Geometry] != nullptr)
	{
		pipelineDesc.GS.pShaderBytecode = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Geometry]->GetBufferPointer();
		pipelineDesc.GS.BytecodeLength = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Geometry]->GetBufferSize();
	}
	pipelineDesc.PS.pShaderBytecode = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Pixel]->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Pixel]->GetBufferSize();
	//デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthEnable = true;		//深度テストを行わない
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;		//深度値フォーマット

	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // 背面をカリングしない
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;	//書き込み許可

	pipelineDesc.InputLayout.pInputElementDescs = inputLayout.data();
	pipelineDesc.InputLayout.NumElements = (UINT)inputLayout.size();

	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	pipelineDesc.NumRenderTargets = 2; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = pipelineDesc.BlendState.RenderTarget[0];
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	blendDesc.BlendEnable = true;						//ブレンドを有効にする
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//加算
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;			//ソースの値を100%使う
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;			//デストの値を  0%使う

	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
	blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//ソースのアルファ値
	blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		//1.0f-ソースのアルファ値

	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc2 = pipelineDesc.BlendState.RenderTarget[1];
	blendDesc2.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc2.BlendEnable = true;						//ブレンドを有効にする
	blendDesc2.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//加算
	blendDesc2.SrcBlendAlpha = D3D12_BLEND_ONE;			//ソースの値を100%使う
	blendDesc2.DestBlendAlpha = D3D12_BLEND_ZERO;			//デストの値を  0%使う
	blendDesc2.BlendOp = D3D12_BLEND_OP_ADD;				//加算
	blendDesc2.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//ソースのアルファ値
	blendDesc2.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		//1.0f-ソースのアルファ値

	if (CreateGraphicsPipeline("ParticleTransparent_DEFALUT", rootSignatureDesc, pipelineDesc, (uint8_t)PIPELINE_SETTING::Transparent))
	{
		return nullptr;
	}
	auto pipeline = pipelineList_.back().get();

	pipeline->inTex_ = 0;
	pipeline->outTex_ = 0;
	pipeline->addParam_ = 0;
	return pipeline;
}

GraphicsPipeline* IFE::GraphicsPipelineManager::CreateSubParticleGraphicsPipeLine()
{
	string vs = defaultDirectory_ + "ParticleVS.hlsl";
	ShaderCompile(vs, SHADER_COMPILE_SETTINGS::Vertex);
	string ps = defaultDirectory_ + "ParticlePS.hlsl";
	ShaderCompile(ps, SHADER_COMPILE_SETTINGS::Pixel);
	string gs = defaultDirectory_ + "ParticleGS.hlsl";
	ShaderCompile(gs, SHADER_COMPILE_SETTINGS::Geometry);

	vector<D3D12_ROOT_PARAMETER> rootParams;

	for (size_t i = 0; i < 2; i++)
	{
		D3D12_ROOT_PARAMETER rootParamSeed;
		//定数用
		rootParamSeed.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;				//種類
		rootParamSeed.Descriptor.ShaderRegister = (UINT)i;								//デスクリプタレンジ
		rootParamSeed.Descriptor.RegisterSpace = 0;									//デスクリプタレンジ数
		rootParamSeed.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//すべてのシェーダーから見える
		rootParams.push_back(rootParamSeed);
	}

	D3D12_ROOT_PARAMETER rootParamSeed2;
	rootParamSeed2.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		//種類
	rootParamSeed2.DescriptorTable.pDescriptorRanges = &TextureManager::Instance()->GetDescRangeSRV();				//デスクリプタレンジ
	rootParamSeed2.DescriptorTable.NumDescriptorRanges = 1;							//デスクリプタレンジ数
	rootParamSeed2.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;					//すべてのシェーダーから見える
	rootParams.push_back(rootParamSeed2);


	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
	inputLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//横繰り返し
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//縦繰り返し
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//奥行繰り返し
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//リニア補完
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;												//ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//ピクセルシェーダーからのみ可視

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParams.front();
	rootSignatureDesc.NumParameters = (UINT)rootParams.size();
	//テクスチャ追加
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	pipelineDesc.VS.pShaderBytecode = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Vertex]->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Vertex]->GetBufferSize();
	if (blobs_[(size_t)SHADER_COMPILE_SETTINGS::Geometry] != nullptr)
	{
		pipelineDesc.GS.pShaderBytecode = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Geometry]->GetBufferPointer();
		pipelineDesc.GS.BytecodeLength = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Geometry]->GetBufferSize();
	}
	pipelineDesc.PS.pShaderBytecode = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Pixel]->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = blobs_[(size_t)SHADER_COMPILE_SETTINGS::Pixel]->GetBufferSize();
	//デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthEnable = true;		//深度テストを行わない
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;		//深度値フォーマット

	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // 背面をカリングしない
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;	//書き込み許可

	pipelineDesc.InputLayout.pInputElementDescs = inputLayout.data();
	pipelineDesc.InputLayout.NumElements = (UINT)inputLayout.size();

	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	pipelineDesc.NumRenderTargets = 2; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = pipelineDesc.BlendState.RenderTarget[0];
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	blendDesc.BlendEnable = true;						//ブレンドを有効にする
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//加算
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;			//ソースの値を100%使う
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;			//デストの値を  0%使う

	blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;				//加算
	blendDesc.SrcBlend = D3D12_BLEND_ONE;			//ソースのアルファ値
	blendDesc.DestBlend = D3D12_BLEND_ONE;		//1.0f-ソースのアルファ値

	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc2 = pipelineDesc.BlendState.RenderTarget[1];
	blendDesc2.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc2.BlendEnable = true;						//ブレンドを有効にする
	blendDesc2.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//加算
	blendDesc2.SrcBlendAlpha = D3D12_BLEND_ONE;			//ソースの値を100%使う
	blendDesc2.DestBlendAlpha = D3D12_BLEND_ZERO;			//デストの値を  0%使う
	blendDesc2.BlendOp = D3D12_BLEND_OP_ADD;				//加算
	blendDesc2.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//ソースのアルファ値
	blendDesc2.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		//1.0f-ソースのアルファ値

	if (CreateGraphicsPipeline("ParticleSub_DEFALUT", rootSignatureDesc, pipelineDesc, (uint8_t)PIPELINE_SETTING::Transparent))
	{
		return nullptr;
	}
	auto pipeline = pipelineList_.back().get();

	pipeline->inTex_ = 0;
	pipeline->outTex_ = 0;
	pipeline->addParam_ = 0;
	return pipeline;
}

bool IFE::GraphicsPipelineManager::CreateGraphicsPipeline(const string& pipelineName, const D3D12_ROOT_SIGNATURE_DESC& rootSignatureDesc, D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc, uint8_t pipelineNum)
{
	ID3D12Device* device = GraphicsAPI::GetDevice();
	ID3DBlob* rootSigBlob = nullptr;
	HRESULT result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob_);
	ID3D12RootSignature* rootsignature;
	ID3D12PipelineState* pipelinestate;

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		string error;
		error.resize(errorBlob_->GetBufferSize());

		copy_n((char*)errorBlob_->GetBufferPointer(), errorBlob_->GetBufferSize(), error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(SUCCEEDED(result));
		return true;
	}
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();

	pipelineDesc.pRootSignature = rootsignature;

	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelinestate));
	assert(SUCCEEDED(result));

	pipelineList_.push_back(std::make_unique<GraphicsPipeline>());
	pipelineList_.back()->SetRootSignature(rootsignature);
	pipelineList_.back()->SetPipelineState(pipelinestate);
	pipelineList_.back()->name_ = pipelineName;
	pipelineList_.back()->pipelineNum_ = pipelineNum;
	return false;
}

GraphicsPipeline* IFE::GraphicsPipelineManager::GetGraphicsPipeline(const std::string& name)
{
	for (auto& itr : pipelineList_)
	{
		if (itr->name_ == name)
		{
			return itr.get();
		}
	}
	return nullptr;
}

ID3DBlob* IFE::GraphicsPipelineManager::GetBlob(uint8_t num)
{
	return blobs_[num].Get();
}

GraphicsPipeline* IFE::GraphicsPipelineManager::CreateDefaultPostEffectPipeLine()
{
	return CreatePostEffectPipeLine("DefaultPEVS", "DefaultPEPS", "defaultPE_DEFALUT", 2, 2);
}

GraphicsPipeline* IFE::GraphicsPipelineManager::CreatePostEffectPipeLine(std::string v, std::string p, std::string name, int16_t inputTexNum, int16_t outputTexNum, int16_t rootParam)
{
	string vs = defaultDirectory_ + v + ".hlsl";
	ShaderCompile(vs, SHADER_COMPILE_SETTINGS::Vertex);
	string ps = defaultDirectory_ + p + ".hlsl";
	ShaderCompile(ps, SHADER_COMPILE_SETTINGS::Pixel);

	std::vector<CD3DX12_ROOT_PARAMETER> rootParams;
	// デスクリプタレンジ
	std::vector<CD3DX12_DESCRIPTOR_RANGE> descRangeSRV;
	descRangeSRV.resize(inputTexNum);
	rootParams.resize(inputTexNum + rootParam);
	for (size_t i = 0; i < inputTexNum; i++)
	{
		descRangeSRV[i].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, UINT(i)); // t0 レジスタ
		rootParams[i].InitAsDescriptorTable(1, &descRangeSRV[i], D3D12_SHADER_VISIBILITY_ALL);
	}
	for (size_t i = 0; i < rootParam; i++)
	{
		rootParams[i + inputTexNum].InitAsConstantBufferView(UINT(i), 0, D3D12_SHADER_VISIBILITY_ALL);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT); // s0 レジスタ

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams.data();
	rootSignatureDesc.NumParameters = UINT(rootParams.size());
	//テクスチャ追加
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// サンプルマスク
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	// デプスステンシルステート
	pipelineDesc.DepthStencilState.DepthEnable = true;		//深度テストを行う
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//書き込み許可
	pipelineDesc.DepthStencilState.StencilEnable = false;							//ステンシルテストなし
	pipelineDesc.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	pipelineDesc.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;

	pipelineDesc.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	pipelineDesc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	pipelineDesc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	pipelineDesc.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	pipelineDesc.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	pipelineDesc.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	pipelineDesc.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	pipelineDesc.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

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

	// 深度バッファのフォーマット
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	pipelineDesc.NumRenderTargets = UINT(outputTexNum);	// 描画対象は1つ
	for (size_t i = 0; i < outputTexNum; i++)
	{
		pipelineDesc.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
		// ブレンドステートの設定
		pipelineDesc.BlendState.RenderTarget[i] = blenddesc;
	}
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	pipelineDesc.VS = CD3DX12_SHADER_BYTECODE(blobs_[(size_t)SHADER_COMPILE_SETTINGS::Vertex].Get());
	pipelineDesc.PS = CD3DX12_SHADER_BYTECODE(blobs_[(size_t)SHADER_COMPILE_SETTINGS::Pixel].Get());


	if (CreateGraphicsPipeline(name, rootSignatureDesc, pipelineDesc, (uint8_t)PIPELINE_SETTING::PostEffect))
	{
		return nullptr;
	}
	auto pipeline = pipelineList_.back().get();

	pipeline->inTex_ = inputTexNum;
	pipeline->outTex_ = outputTexNum;
	pipeline->addParam_ = rootParam;
	return pipeline;
}

GraphicsPipeline* IFE::GraphicsPipelineManager::CreatePostEffectPipeLine(std::string shaderName, int16_t inputTexNum, int16_t outputTexNum, int16_t rootParam)
{
	return CreatePostEffectPipeLine(shaderName + "VS", shaderName + "PS", shaderName, inputTexNum, outputTexNum, rootParam);
}

#ifdef EditorMode
#include "ImguiManager.h"
void IFE::GraphicsPipelineManager::DebugGUI()
{
	auto gui = ImguiManager::Instance();

	gui->NewGUI(U8("グラフィックパイプラインマネージャ"));

	if (gui->CollapsingHeaderGUI(U8("追加")))
	{
		static int32_t num;
		const static std::vector<std::string> items = { "Object","ObjectAnimation","PostEffect" };
		enum Mode
		{
			Object, ObjectAnimation, PostEffect
		};
		gui->Combo(U8("追加パイプライン設定モード"), num, items);

		static std::string name;
		gui->TextGUI(U8("必ずabc.VS,abc.psの場合は「ABC」を入力してください"));
		gui->InputTextGUI(U8("パイプライン名"), name);

		static int32_t param = 0;
		static int32_t inTex = 1;
		static int32_t outTex = 1;

		gui->DragIntGUI(&param, U8("追加コンストバッファ数"), 1, 0, 100);
		gui->DragIntGUI(&inTex, U8("読み込みテクスチャ数"), 1, 0, 100);
		gui->DragIntGUI(&outTex, U8("書き出しテクスチャ数"), 1, 0, 100);

		if (!GetGraphicsPipeline(name))
		{
			switch (num)
			{
			case Mode::Object:
				if (gui->ButtonGUI("Add"))
					CreateObjectGraphicsPipeLine(name, (int16_t)param, (int16_t)inTex, (int16_t)outTex);
				break;
			case Mode::ObjectAnimation:
				if (gui->ButtonGUI("Add"))
					CreateAnimObjectGraphicsPipeLine(name, (int16_t)param, (int16_t)inTex, (int16_t)outTex);
				break;
			case Mode::PostEffect:
				if (gui->ButtonGUI("Add"))
					CreatePostEffectPipeLine(name, (int16_t)inTex, (int16_t)outTex, (int16_t)param);
				break;

			default:
				gui->TextGUI(U8("エラー:モード設定に問題があります"));
				break;
			}
		}
		else
		{
			gui->TextGUI(U8("エラー:すでに存在します。また、生成後はこの表示が出ます"));
		}
	}

	if (gui->CollapsingHeaderGUI(U8("パイプラインリスト")))
	{
		for (auto& itr : pipelineList_)
		{
			if (gui->NewTreeNode(itr->name_))
			{
				gui->TextIntGUI(U8("入力テクスチャ数"), itr->inTex_);
				gui->TextIntGUI(U8("出力テクスチャ数"), itr->outTex_);
				gui->TextIntGUI(U8("追加定数バッファ数"), itr->addParam_);
				gui->EndTreeNode();
			}
		}
	}
	gui->EndGUI();
}

std::string IFE::GraphicsPipelineManager::GetGraphicsPipelineGUI()
{
	std::vector<std::string>items;
	items.resize(pipelineList_.size());
	size_t i = 0;
	for (auto& itr : pipelineList_)
	{
		items[i] = itr->name_;
		i++;
	}

	static int32_t returnNum = 0;
	ImguiManager::Instance()->Combo("pipeline name", returnNum, items);
	return items[returnNum];
}
void IFE::GraphicsPipelineManager::Output()
{
	auto json = JsonManager::Instance();

	auto& data = json->GetJsonData();

	size_t i = 0;
	for (auto& itr : pipelineList_)
	{
		if (Contains(itr->name_, "_DEFALUT"))continue;
		data[i]["name"] = itr->name_;
		data[i]["number"] = itr->pipelineNum_;
		data[i]["inTex"] = itr->inTex_;
		data[i]["outTex"] = itr->outTex_;
		data[i]["addParam"] = itr->addParam_;
		i++;
	}

	json->Output("GraphicsPipelineManager");
}
#endif

void IFE::GraphicsPipelineManager::Loading()
{
	auto json = JsonManager::Instance();
	json->Input("GraphicsPipelineManager");

	auto data = json->GetJsonData();
	for (auto& itr : data)
	{
		std::string name;
		uint16_t num = 0;
		uint16_t inTex = 0;
		uint16_t outTex = 0;
		uint16_t addParam = 0;
		if (!json->GetData(itr, "name", name) || !json->GetData(itr, "number", num))
		{
			continue;
		}

		json->GetData(itr, "inTex", inTex);
		json->GetData(itr, "outTex", outTex);
		json->GetData(itr, "addParam", addParam);

		if (num >= uint16_t(PIPELINE_SETTING::Normal) && num < uint16_t(PIPELINE_SETTING::Anim))
		{
			CreateObjectGraphicsPipeLine(name, addParam, inTex, outTex);
		}
		else if (num >= uint16_t(PIPELINE_SETTING::Anim) && num < uint16_t(PIPELINE_SETTING::Transparent))
		{
			CreateAnimObjectGraphicsPipeLine(name, addParam, inTex, outTex);
		}
		else if (num >= uint16_t(PIPELINE_SETTING::PostEffect))
		{
			CreatePostEffectPipeLine(name, inTex, outTex, addParam);
		}
	}
}

void IFE::GraphicsPipelineManager::Reset()
{
	pipelineList_.remove_if([](std::unique_ptr<GraphicsPipeline>& p) {return !Contains(p->name_, "_DEFALUT"); });
}
