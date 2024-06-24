#include "GraphicsPipelineManager.h"
#include <d3dcompiler.h>
#include "Debug.h"
#include "StringUtil.h"
#include "GraphicsAPI.h"
#include "TextureManager.h"
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
	HRESULT result = D3DCompileFromFile(wname.c_str(),  // �V�F�[�_�t�@�C����
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", setting_shader_type[num], // �G���g���[�|�C���g���A�V�F�[�_�[���f��	�w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0, &blobs_[num], &errorBlob_);

	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		string error;
		error.resize(errorBlob_->GetBufferSize());

		copy_n((char*)errorBlob_->GetBufferPointer(), errorBlob_->GetBufferSize(), error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		return true;
	}
	return false;
}

void* defaultDirectory_;

GraphicsPipeline* IFE::GraphicsPipelineManager::CreateBasicGraphicsPipeLine()
{
	return CreateObjectGraphicsPipeLine("ModelVS", "ModelGS", "ModelPS", "3dNormal", 0, 1, 2);
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
	// �f�X�N���v�^�����W
	std::vector<CD3DX12_DESCRIPTOR_RANGE> descRangeSRV;
	descRangeSRV.resize(inputTexNum);
	for (size_t i = 0; i < inputTexNum; i++)
	{
		descRangeSRV[i].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, UINT(i)); // t0 ���W�X�^
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
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���J��Ԃ�
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//�c�J��Ԃ�
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���s�J��Ԃ�
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//���j�A�⊮
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;												//�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//�s�N�Z���V�F�[�_�[����̂݉�

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParams.front();
	rootSignatureDesc.NumParameters = (UINT)rootParams.size();
	//�e�N�X�`���ǉ�
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
	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	pipelineDesc.DepthStencilState.DepthEnable = true;		//�[�x�e�X�g���s��
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;		//�[�x�l�t�H�[�}�b�g

	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;  // �w�ʂ��J�����O
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//�������݋���

	pipelineDesc.InputLayout.pInputElementDescs = inputLayout.data();
	pipelineDesc.InputLayout.NumElements = (UINT)inputLayout.size();

	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	pipelineDesc.NumRenderTargets = (UINT)outputTexNum; // �`��Ώۂ�1��
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc;
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	blendDesc.BlendEnable = true;						//�u�����h��L���ɂ���
	blendDesc.LogicOpEnable = false;  // LogicOp�𖳌��ɂ���
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//���Z
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;			//�\�[�X�̒l��100%�g��
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;			//�f�X�g�̒l��  0%�g��

	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
	blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//�\�[�X�̃A���t�@�l
	blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		//1.0f-�\�[�X�̃A���t�@�l

	for (size_t i = 0; i < outputTexNum; i++)
	{
		pipelineDesc.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
		pipelineDesc.BlendState.RenderTarget[i] = blendDesc;
	}
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	static uint8_t number = 0;

	if (CreateGraphicsPipeline(name, rootSignatureDesc, pipelineDesc, (uint8_t)PIPELINE_SETTING::Normal + number))
	{
		return nullptr;
	}
	number++;
	if (number >= uint8_t(PIPELINE_SETTING::Anim))number = 1;
	return pipelineList_.back().get();
}

GraphicsPipeline* IFE::GraphicsPipelineManager::CreateAnimGraphicsPipeLine()
{
	string vs = defaultDirectory_ + "ModelAnimVS.hlsl";
	ShaderCompile(vs, SHADER_COMPILE_SETTINGS::Vertex);
	string ps = defaultDirectory_ + "ModelAnimPS.hlsl";
	ShaderCompile(ps, SHADER_COMPILE_SETTINGS::Pixel);
	string gs = defaultDirectory_ + "ModelAnimGS.hlsl";
	ShaderCompile(gs, SHADER_COMPILE_SETTINGS::Geometry);

	vector<D3D12_ROOT_PARAMETER> rootParams;

	for (size_t i = 0; i < 5; i++)
	{
		D3D12_ROOT_PARAMETER rootParamSeed;
		//�萔�p
		rootParamSeed.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;				//���
		rootParamSeed.Descriptor.ShaderRegister = (UINT)i;								//�f�X�N���v�^�����W
		rootParamSeed.Descriptor.RegisterSpace = 0;									//�f�X�N���v�^�����W��
		rootParamSeed.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//���ׂẴV�F�[�_�[���猩����
		rootParams.push_back(rootParamSeed);
	}

	D3D12_ROOT_PARAMETER rootParamSeed2;
	rootParamSeed2.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		//���
	rootParamSeed2.DescriptorTable.pDescriptorRanges = &TextureManager::Instance()->GetDescRangeSRV();				//�f�X�N���v�^�����W
	rootParamSeed2.DescriptorTable.NumDescriptorRanges = 1;							//�f�X�N���v�^�����W��
	rootParamSeed2.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;					//���ׂẴV�F�[�_�[���猩����
	rootParams.push_back(rootParamSeed2);

	D3D12_ROOT_PARAMETER rootParamSeed;
	//�萔�p
	rootParamSeed.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;				//���
	rootParamSeed.Descriptor.ShaderRegister = (UINT)5;								//�f�X�N���v�^�����W
	rootParamSeed.Descriptor.RegisterSpace = 0;									//�f�X�N���v�^�����W��
	rootParamSeed.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//���ׂẴV�F�[�_�[���猩����
	rootParams.push_back(rootParamSeed);

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
	inputLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	inputLayout.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	inputLayout.push_back({ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	inputLayout.push_back({ "BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	inputLayout.push_back({ "BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���J��Ԃ�
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//�c�J��Ԃ�
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���s�J��Ԃ�
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//���j�A�⊮
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;												//�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//�s�N�Z���V�F�[�_�[����̂݉�

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParams.front();
	rootSignatureDesc.NumParameters = (UINT)rootParams.size();
	//�e�N�X�`���ǉ�
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
	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	pipelineDesc.DepthStencilState.DepthEnable = true;		//�[�x�e�X�g���s��
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;		//�[�x�l�t�H�[�}�b�g

	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;  // �w�ʂ��J�����O
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//�������݋���

	pipelineDesc.InputLayout.pInputElementDescs = inputLayout.data();
	pipelineDesc.InputLayout.NumElements = (UINT)inputLayout.size();

	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	pipelineDesc.NumRenderTargets = 2; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = pipelineDesc.BlendState.RenderTarget[0];
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	blendDesc.BlendEnable = true;						//�u�����h��L���ɂ���
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//���Z
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;			//�\�[�X�̒l��100%�g��
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;			//�f�X�g�̒l��  0%�g��

	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
	blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//�\�[�X�̃A���t�@�l
	blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		//1.0f-�\�[�X�̃A���t�@�l
	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc2 = pipelineDesc.BlendState.RenderTarget[1];
	blendDesc2.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc2.BlendEnable = true;						//�u�����h��L���ɂ���
	blendDesc2.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//���Z
	blendDesc2.SrcBlendAlpha = D3D12_BLEND_ONE;			//�\�[�X�̒l��100%�g��
	blendDesc2.DestBlendAlpha = D3D12_BLEND_ZERO;			//�f�X�g�̒l��  0%�g��
	blendDesc2.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
	blendDesc2.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//�\�[�X�̃A���t�@�l
	blendDesc2.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		//1.0f-�\�[�X�̃A���t�@�l

	if (CreateGraphicsPipeline("3dAnim", rootSignatureDesc, pipelineDesc, (uint8_t)PIPELINE_SETTING::Anim))
	{
		return nullptr;
	}
	return pipelineList_.back().get();
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
	// �f�X�N���v�^�����W
	std::vector<CD3DX12_DESCRIPTOR_RANGE> descRangeSRV;
	descRangeSRV.resize(inputTexNum);
	for (size_t i = 0; i < inputTexNum; i++)
	{
		descRangeSRV[i].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, UINT(i)); // t0 ���W�X�^
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
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���J��Ԃ�
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//�c�J��Ԃ�
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���s�J��Ԃ�
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//���j�A�⊮
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;												//�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//�s�N�Z���V�F�[�_�[����̂݉�

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParams.front();
	rootSignatureDesc.NumParameters = (UINT)rootParams.size();
	//�e�N�X�`���ǉ�
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
	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	pipelineDesc.DepthStencilState.DepthEnable = true;		//�[�x�e�X�g���s��
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;		//�[�x�l�t�H�[�}�b�g

	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;  // �w�ʂ��J�����O
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//�������݋���

	pipelineDesc.InputLayout.pInputElementDescs = inputLayout.data();
	pipelineDesc.InputLayout.NumElements = (UINT)inputLayout.size();

	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	pipelineDesc.NumRenderTargets = (UINT)outputTexNum; // �`��Ώۂ�1��
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc;
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	blendDesc.BlendEnable = true;						//�u�����h��L���ɂ���
	blendDesc.LogicOpEnable = false;  // LogicOp�𖳌��ɂ���
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//���Z
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;			//�\�[�X�̒l��100%�g��
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;			//�f�X�g�̒l��  0%�g��

	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
	blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//�\�[�X�̃A���t�@�l
	blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		//1.0f-�\�[�X�̃A���t�@�l

	for (size_t i = 0; i < outputTexNum; i++)
	{
		pipelineDesc.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
		pipelineDesc.BlendState.RenderTarget[i] = blendDesc;
	}
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	static uint8_t number = 0;

	if (CreateGraphicsPipeline(name, rootSignatureDesc, pipelineDesc, (uint8_t)PIPELINE_SETTING::Anim + number))
	{
		return nullptr;
	}
	number++;
	if (number >= uint8_t(PIPELINE_SETTING::Transparent))number = 1;
	return pipelineList_.back().get();
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
		//�萔�p
		rootParamSeed.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;				//���
		rootParamSeed.Descriptor.ShaderRegister = (UINT)i;								//�f�X�N���v�^�����W
		rootParamSeed.Descriptor.RegisterSpace = 0;									//�f�X�N���v�^�����W��
		rootParamSeed.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//���ׂẴV�F�[�_�[���猩����
		rootParams.push_back(rootParamSeed);
	}

	D3D12_ROOT_PARAMETER rootParamSeed2;
	rootParamSeed2.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		//���
	rootParamSeed2.DescriptorTable.pDescriptorRanges = &TextureManager::Instance()->GetDescRangeSRV();				//�f�X�N���v�^�����W
	rootParamSeed2.DescriptorTable.NumDescriptorRanges = 1;							//�f�X�N���v�^�����W��
	rootParamSeed2.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;					//���ׂẴV�F�[�_�[���猩����
	rootParams.push_back(rootParamSeed2);


	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
	inputLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	inputLayout.push_back({ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���J��Ԃ�
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//�c�J��Ԃ�
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���s�J��Ԃ�
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//���j�A�⊮
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;												//�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//�s�N�Z���V�F�[�_�[����̂݉�

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParams.front();
	rootSignatureDesc.NumParameters = (UINT)rootParams.size();
	//�e�N�X�`���ǉ�
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
	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	pipelineDesc.DepthStencilState.DepthEnable = false;		//�[�x�e�X�g���s��Ȃ�
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;		//�[�x�l�t�H�[�}�b�g

	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // �w�ʂ��J�����O���Ȃ�
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//�������݋���

	pipelineDesc.InputLayout.pInputElementDescs = inputLayout.data();
	pipelineDesc.InputLayout.NumElements = (UINT)inputLayout.size();

	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = pipelineDesc.BlendState.RenderTarget[0];
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	blendDesc.BlendEnable = true;						//�u�����h��L���ɂ���
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//���Z
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;			//�\�[�X�̒l��100%�g��
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;			//�f�X�g�̒l��  0%�g��

	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
	blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//�\�[�X�̃A���t�@�l
	blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		//1.0f-�\�[�X�̃A���t�@�l

	if (CreateGraphicsPipeline("2dNormal", rootSignatureDesc, pipelineDesc, (uint8_t)PIPELINE_SETTING::SPrite))
	{
		return nullptr;
	}
	return pipelineList_.back().get();
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
		//�萔�p
		rootParamSeed.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;				//���
		rootParamSeed.Descriptor.ShaderRegister = (UINT)i;								//�f�X�N���v�^�����W
		rootParamSeed.Descriptor.RegisterSpace = 0;									//�f�X�N���v�^�����W��
		rootParamSeed.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//���ׂẴV�F�[�_�[���猩����
		rootParams.push_back(rootParamSeed);
	}

	D3D12_ROOT_PARAMETER rootParamSeed2;
	rootParamSeed2.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		//���
	rootParamSeed2.DescriptorTable.pDescriptorRanges = &TextureManager::Instance()->GetDescRangeSRV();				//�f�X�N���v�^�����W
	rootParamSeed2.DescriptorTable.NumDescriptorRanges = 1;							//�f�X�N���v�^�����W��
	rootParamSeed2.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;					//���ׂẴV�F�[�_�[���猩����
	rootParams.push_back(rootParamSeed2);


	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
	inputLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���J��Ԃ�
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//�c�J��Ԃ�
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���s�J��Ԃ�
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//���j�A�⊮
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;												//�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//�s�N�Z���V�F�[�_�[����̂݉�

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParams.front();
	rootSignatureDesc.NumParameters = (UINT)rootParams.size();
	//�e�N�X�`���ǉ�
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
	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	pipelineDesc.DepthStencilState.DepthEnable = true;		//�[�x�e�X�g���s��Ȃ�
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;		//�[�x�l�t�H�[�}�b�g

	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // �w�ʂ��J�����O���Ȃ�
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;	//�������݋���

	pipelineDesc.InputLayout.pInputElementDescs = inputLayout.data();
	pipelineDesc.InputLayout.NumElements = (UINT)inputLayout.size();

	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	pipelineDesc.NumRenderTargets = 2; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = pipelineDesc.BlendState.RenderTarget[0];
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	blendDesc.BlendEnable = true;						//�u�����h��L���ɂ���
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//���Z
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;			//�\�[�X�̒l��100%�g��
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;			//�f�X�g�̒l��  0%�g��

	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
	blendDesc.SrcBlend = D3D12_BLEND_ONE;				//�\�[�X�̒l��100%�g��
	blendDesc.DestBlend = D3D12_BLEND_ONE;				//�\�[�X�̒l��100%�g��

	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc2 = pipelineDesc.BlendState.RenderTarget[1];
	blendDesc2.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc2.BlendEnable = true;						//�u�����h��L���ɂ���
	blendDesc2.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//���Z
	blendDesc2.SrcBlendAlpha = D3D12_BLEND_ONE;			//�\�[�X�̒l��100%�g��
	blendDesc2.DestBlendAlpha = D3D12_BLEND_ZERO;			//�f�X�g�̒l��  0%�g��
	blendDesc2.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
	blendDesc2.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//�\�[�X�̃A���t�@�l
	blendDesc2.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		//1.0f-�\�[�X�̃A���t�@�l

	if (CreateGraphicsPipeline("ParticleNormal", rootSignatureDesc, pipelineDesc, (uint8_t)PIPELINE_SETTING::Transparent))
	{
		return nullptr;
	}
	return pipelineList_.back().get();
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
		//�萔�p
		rootParamSeed.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;				//���
		rootParamSeed.Descriptor.ShaderRegister = (UINT)i;								//�f�X�N���v�^�����W
		rootParamSeed.Descriptor.RegisterSpace = 0;									//�f�X�N���v�^�����W��
		rootParamSeed.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//���ׂẴV�F�[�_�[���猩����
		rootParams.push_back(rootParamSeed);
	}

	D3D12_ROOT_PARAMETER rootParamSeed2;
	rootParamSeed2.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		//���
	rootParamSeed2.DescriptorTable.pDescriptorRanges = &TextureManager::Instance()->GetDescRangeSRV();				//�f�X�N���v�^�����W
	rootParamSeed2.DescriptorTable.NumDescriptorRanges = 1;							//�f�X�N���v�^�����W��
	rootParamSeed2.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;					//���ׂẴV�F�[�_�[���猩����
	rootParams.push_back(rootParamSeed2);


	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
	inputLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���J��Ԃ�
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//�c�J��Ԃ�
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���s�J��Ԃ�
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//���j�A�⊮
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;												//�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//�s�N�Z���V�F�[�_�[����̂݉�

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParams.front();
	rootSignatureDesc.NumParameters = (UINT)rootParams.size();
	//�e�N�X�`���ǉ�
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
	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	pipelineDesc.DepthStencilState.DepthEnable = true;		//�[�x�e�X�g���s��Ȃ�
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;		//�[�x�l�t�H�[�}�b�g

	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // �w�ʂ��J�����O���Ȃ�
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;	//�������݋���

	pipelineDesc.InputLayout.pInputElementDescs = inputLayout.data();
	pipelineDesc.InputLayout.NumElements = (UINT)inputLayout.size();

	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	pipelineDesc.NumRenderTargets = 2; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = pipelineDesc.BlendState.RenderTarget[0];
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	blendDesc.BlendEnable = true;						//�u�����h��L���ɂ���
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//���Z
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;			//�\�[�X�̒l��100%�g��
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;			//�f�X�g�̒l��  0%�g��

	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
	blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//�\�[�X�̃A���t�@�l
	blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		//1.0f-�\�[�X�̃A���t�@�l

	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc2 = pipelineDesc.BlendState.RenderTarget[1];
	blendDesc2.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc2.BlendEnable = true;						//�u�����h��L���ɂ���
	blendDesc2.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//���Z
	blendDesc2.SrcBlendAlpha = D3D12_BLEND_ONE;			//�\�[�X�̒l��100%�g��
	blendDesc2.DestBlendAlpha = D3D12_BLEND_ZERO;			//�f�X�g�̒l��  0%�g��
	blendDesc2.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
	blendDesc2.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//�\�[�X�̃A���t�@�l
	blendDesc2.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		//1.0f-�\�[�X�̃A���t�@�l

	if (CreateGraphicsPipeline("ParticleTransparent", rootSignatureDesc, pipelineDesc, (uint8_t)PIPELINE_SETTING::Transparent))
	{
		return nullptr;
	}
	return pipelineList_.back().get();
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
		//�萔�p
		rootParamSeed.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;				//���
		rootParamSeed.Descriptor.ShaderRegister = (UINT)i;								//�f�X�N���v�^�����W
		rootParamSeed.Descriptor.RegisterSpace = 0;									//�f�X�N���v�^�����W��
		rootParamSeed.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//���ׂẴV�F�[�_�[���猩����
		rootParams.push_back(rootParamSeed);
	}

	D3D12_ROOT_PARAMETER rootParamSeed2;
	rootParamSeed2.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		//���
	rootParamSeed2.DescriptorTable.pDescriptorRanges = &TextureManager::Instance()->GetDescRangeSRV();				//�f�X�N���v�^�����W
	rootParamSeed2.DescriptorTable.NumDescriptorRanges = 1;							//�f�X�N���v�^�����W��
	rootParamSeed2.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;					//���ׂẴV�F�[�_�[���猩����
	rootParams.push_back(rootParamSeed2);


	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
	inputLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���J��Ԃ�
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//�c�J��Ԃ�
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���s�J��Ԃ�
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//���j�A�⊮
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;												//�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//�s�N�Z���V�F�[�_�[����̂݉�

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParams.front();
	rootSignatureDesc.NumParameters = (UINT)rootParams.size();
	//�e�N�X�`���ǉ�
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
	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	pipelineDesc.DepthStencilState.DepthEnable = true;		//�[�x�e�X�g���s��Ȃ�
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;		//�[�x�l�t�H�[�}�b�g

	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // �w�ʂ��J�����O���Ȃ�
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;	//�������݋���

	pipelineDesc.InputLayout.pInputElementDescs = inputLayout.data();
	pipelineDesc.InputLayout.NumElements = (UINT)inputLayout.size();

	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	pipelineDesc.NumRenderTargets = 2; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = pipelineDesc.BlendState.RenderTarget[0];
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	blendDesc.BlendEnable = true;						//�u�����h��L���ɂ���
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//���Z
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;			//�\�[�X�̒l��100%�g��
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;			//�f�X�g�̒l��  0%�g��

	blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;				//���Z
	blendDesc.SrcBlend = D3D12_BLEND_ONE;			//�\�[�X�̃A���t�@�l
	blendDesc.DestBlend = D3D12_BLEND_ONE;		//1.0f-�\�[�X�̃A���t�@�l

	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc2 = pipelineDesc.BlendState.RenderTarget[1];
	blendDesc2.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc2.BlendEnable = true;						//�u�����h��L���ɂ���
	blendDesc2.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//���Z
	blendDesc2.SrcBlendAlpha = D3D12_BLEND_ONE;			//�\�[�X�̒l��100%�g��
	blendDesc2.DestBlendAlpha = D3D12_BLEND_ZERO;			//�f�X�g�̒l��  0%�g��
	blendDesc2.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
	blendDesc2.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//�\�[�X�̃A���t�@�l
	blendDesc2.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		//1.0f-�\�[�X�̃A���t�@�l

	if (CreateGraphicsPipeline("ParticleSub", rootSignatureDesc, pipelineDesc, (uint8_t)PIPELINE_SETTING::Transparent))
	{
		return nullptr;
	}
	return pipelineList_.back().get();
}

bool IFE::GraphicsPipelineManager::CreateGraphicsPipeline(const string& pipelineName, const D3D12_ROOT_SIGNATURE_DESC& rootSignatureDesc, D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc, uint8_t pipelineNum)
{
	ID3D12Device* device = GraphicsAPI::GetDevice();
	ID3DBlob* rootSigBlob = nullptr;
	HRESULT result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob_);
	ID3D12RootSignature* rootsignature;
	ID3D12PipelineState* pipelinestate;

	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		string error;
		error.resize(errorBlob_->GetBufferSize());

		copy_n((char*)errorBlob_->GetBufferPointer(), errorBlob_->GetBufferSize(), error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
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
	return CreatePostEffectPipeLine("DefaultPEVS", "DefaultPEPS", "defaultPostEffect", 1, 2);
}

GraphicsPipeline* IFE::GraphicsPipelineManager::CreatePostEffectPipeLine(std::string v, std::string p, std::string name, int16_t inputTexNum, int16_t outputTexNum, int16_t rootParam)
{
	string vs = defaultDirectory_ + v + ".hlsl";
	ShaderCompile(vs, SHADER_COMPILE_SETTINGS::Vertex);
	string ps = defaultDirectory_ + p + ".hlsl";
	ShaderCompile(ps, SHADER_COMPILE_SETTINGS::Pixel);

	std::vector<CD3DX12_ROOT_PARAMETER> rootParams;
	// �f�X�N���v�^�����W
	std::vector<CD3DX12_DESCRIPTOR_RANGE> descRangeSRV;
	descRangeSRV.resize(inputTexNum);
	rootParams.resize(inputTexNum + rootParam);
	for (size_t i = 0; i < inputTexNum; i++)
	{
		descRangeSRV[i].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, UINT(i)); // t0 ���W�X�^
		rootParams[i].InitAsDescriptorTable(1, &descRangeSRV[i], D3D12_SHADER_VISIBILITY_ALL);
	}
	for (size_t i = 0; i < rootParam;i++)
	{
		rootParams[i + inputTexNum].InitAsConstantBufferView(UINT(i), 0, D3D12_SHADER_VISIBILITY_ALL);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT); // s0 ���W�X�^

	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams.data();
	rootSignatureDesc.NumParameters = UINT(rootParams.size());
	//�e�N�X�`���ǉ�
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// �T���v���}�X�N
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	// �f�v�X�X�e���V���X�e�[�g
	pipelineDesc.DepthStencilState.DepthEnable = true;		//�[�x�e�X�g���s��
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//�������݋���
	pipelineDesc.DepthStencilState.StencilEnable = false;							//�X�e���V���e�X�g�Ȃ�
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

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	pipelineDesc.NumRenderTargets = UINT(outputTexNum);	// �`��Ώۂ�1��
	for (size_t i = 0; i < outputTexNum; i++)
	{
		pipelineDesc.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
		// �u�����h�X�e�[�g�̐ݒ�
		pipelineDesc.BlendState.RenderTarget[i] = blenddesc;
	}
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	pipelineDesc.VS = CD3DX12_SHADER_BYTECODE(blobs_[(size_t)SHADER_COMPILE_SETTINGS::Vertex].Get());
	pipelineDesc.PS = CD3DX12_SHADER_BYTECODE(blobs_[(size_t)SHADER_COMPILE_SETTINGS::Pixel].Get());


	if (CreateGraphicsPipeline(name, rootSignatureDesc, pipelineDesc, (uint8_t)PIPELINE_SETTING::PostEffect))
	{
		return nullptr;
	}
	return pipelineList_.back().get();
}

GraphicsPipeline* IFE::GraphicsPipelineManager::CreatePostEffectPipeLine(std::string shaderName, int16_t inputTexNum, int16_t outputTexNum, int16_t rootParam)
{
	return CreatePostEffectPipeLine(shaderName + "VS", shaderName + "PS", shaderName, inputTexNum, outputTexNum, rootParam);
}

