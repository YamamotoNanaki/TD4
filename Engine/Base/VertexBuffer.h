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
		// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
		uint32_t sizeVB = static_cast<uint32_t>(sizeof(vertices_[0]) * vertices_.size());

		// ���_�o�b�t�@�̐ݒ�
		D3D12_HEAP_PROPERTIES heapProp{};   // �q�[�v�ݒ�
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

		D3D12_RESOURCE_DESC resDesc{};  // ���\�[�X�ݒ�
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// ���_�o�b�t�@�̐���
		ID3D12Device* device = GraphicsAPI::Instance()->GetDevice();
		HRESULT result = device->CreateCommittedResource(
			&heapProp, // �q�[�v�ݒ�
			D3D12_HEAP_FLAG_NONE, &resDesc, // ���\�[�X�ݒ�
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff_));
		assert(SUCCEEDED(result));
		// GPU��̃o�b�t�@�ɑΉ��������z���������擾
		T* vertMap = nullptr;
		result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
		assert(SUCCEEDED(result));

		// �S���_�ɑ΂���
		for (int32_t i = 0; i < vertices_.size(); i++)
		{
			vertMap[i] = vertices_[i];   // ���W���R�s�[
		}

		// �}�b�v������
		vertBuff_->Unmap(0, nullptr);

		// ���_�o�b�t�@�r���[�̍쐬

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

		// �S���_�ɑ΂���
		for (int32_t i = 0; i < vertexCount; i++)
		{
			vertMap[i] = vertices[i];   // ���W���R�s�[
		}

		// �}�b�v������
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

		// �S���_�ɑ΂���
		for (int32_t i = 0; i < vertices.size(); i++)
		{
			vertMap[i] = vertices[i];   // ���W���R�s�[
		}

		// �}�b�v������
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
