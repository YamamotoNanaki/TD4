#pragma once
#include "IFEMath.h"

#pragma warning(push, 0)
#include <Effekseer.h>
#include <EffekseerRendererDX12.h>
#pragma warning(pop)

namespace IFE
{
	class IFEEffekseerManager
	{
		::EffekseerRenderer::RendererRef efkRenderer;
		::Effekseer::ManagerRef efkManager;
		::Effekseer::RefPtr<EffekseerRenderer::SingleFrameMemoryPool> efkMemoryPool;
		::Effekseer::RefPtr<EffekseerRenderer::CommandList> efkCommandList;

	public:
		void Initialize();

	private:
		Effekseer::Matrix44 FromIFEToEffekseerMatrix(Matrix mat);
		Effekseer::Vector3D FromIFEToEffekseerVector3(Vector3 vec);
	};
}
