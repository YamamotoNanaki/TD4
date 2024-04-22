#pragma once
#include "ConstStruct.h"
#include "Texture.h"
#include <wrl.h>
#include <string>
#include <vector>
#include <d3d12.h>
#include <memory>
#include "ConstStruct.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Component.h"
#include "Material.h"

namespace IFE
{
	class Mesh : public Component
	{
		using Component::Component;
	private:
		VertexBuffer<VertexBone>vb_;
		VertexBuffer<Vertex>vt_;
		IndexBuffer ib_;
		std::unique_ptr<Material> material_;

	public:
		void Initialize()override;
		void Draw()override;
		void Update()override;
		VertexBuffer<VertexBone>* GetVertexBone();
		VertexBuffer<Vertex>* GetVertex();
		IndexBuffer* GetIB();
		std::vector<uint32_t> GetIndexArray();
		std::vector<Vertex> GetVertexArray();
		void SetMaterial(MaterialParams mat);
		MaterialParams GetMaterial();

#ifdef InverseEditorMode
#else
		void ComponentDebugGUI()override;
#endif
	};
};
