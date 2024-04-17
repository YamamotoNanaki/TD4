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
	// �C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	uint32_t sizeIB = static_cast<uint32_t>(sizeof(uint32_t) * indices_.size());

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};   // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	D3D12_RESOURCE_DESC resDesc{};  // ���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;//�C���f�b�N�X�o�b�t�@�̐���
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	HRESULT result = GraphicsAPI::Instance()->GetDevice()->CreateCommittedResource(
		&heapProp,				//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,				//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff_));

	uint32_t* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);

	//�S�C���f�b�N�X�ɑ΂���
	for (int32_t i = 0; i < indices_.size(); i++)
	{
		indexMap[i] = indices_[i];	//�C���f�b�N�X���R�s�[
	}

	//�Ȃ��������
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
