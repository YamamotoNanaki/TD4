#include "Mesh.h"
#include "GraphicsAPI.h"
#include "TextureManager.h"
#include <cassert>

using namespace std;
using namespace IFE;

void IFE::Mesh::Initialize()
{
	vb_.Initialize();
	vt_.Initialize();
	ib_.Initialize();
}

void IFE::Mesh::Draw()
{
	static ID3D12GraphicsCommandList* commandList = GraphicsAPI::Instance()->GetCmdList();
	//頂点バッファの設定
	if (vb_.GetSize() > 0)
	{
		commandList->IASetVertexBuffers(0, 1, vb_.GetVBView());
	}
	if (vt_.GetSize() > 0)
	{
		commandList->IASetVertexBuffers(0, 1, vt_.GetVBView());
	}
	if (material_)
	{
		material_->Draw();
	}
	//インデックスバッファの設定
	commandList->IASetIndexBuffer(ib_.GetIBView());

	//描画コマンド
	commandList->DrawIndexedInstanced((UINT)ib_.GetSize(), 1, 0, 0, 0);
}

void IFE::Mesh::Update()
{
}

VertexBuffer<VertexBone>* IFE::Mesh::GetVertexBone()
{
	return &vb_;
}

VertexBuffer<Vertex>* IFE::Mesh::GetVertex()
{
	return &vt_;
}

IndexBuffer* IFE::Mesh::GetIB()
{
	return &ib_;
}

std::vector<uint32_t> IFE::Mesh::GetIndexArray()
{
	return ib_.GetIndex();
}

std::vector<Vertex> IFE::Mesh::GetVertexArray()
{
	return vt_.GetVertex();
}

void IFE::Mesh::SetMaterial(MaterialParams mat)
{
	if (!material_)
	{
		material_ = make_unique<Material>();
		material_->Initialize();
	}

	material_->SetMaterial(mat);
}

MaterialParams IFE::Mesh::GetMaterial()
{
	return material_->GetMaterial();
}

#ifdef InverseEditorMode
#else
void IFE::Mesh::ComponentDebugGUI()
{
	if (material_)
	{
		material_->DebugGUI();
		material_->componentName_ = componentName_;
	}
}
#endif
