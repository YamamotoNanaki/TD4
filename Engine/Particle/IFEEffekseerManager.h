#pragma once
#include "IFEMath.h"
#include <list>
#include <memory>
#include "EditorMode.h"
#include "IFEEffekseer.h"

#pragma warning(push, 0)
#include <Effekseer.h>
#include <EffekseerRendererDX12.h>
#pragma warning(pop)

namespace IFE
{
	class IFEEffekseerManager
	{
		::EffekseerRenderer::RendererRef efkRenderer_;
		::Effekseer::ManagerRef efkManager_;
		::Effekseer::RefPtr<EffekseerRenderer::SingleFrameMemoryPool> efkMemoryPool_;
		::Effekseer::RefPtr<EffekseerRenderer::CommandList> efkCommandList_;
		std::list<std::unique_ptr<IFEEffekseer>> effekList_;
		std::list<Effekseer::Handle> effectHandles_;
		IFEEffekseerManager() {}
		IFEEffekseerManager(const IFEEffekseerManager&);
		IFEEffekseerManager& operator=(const IFEEffekseerManager&) {}
		~IFEEffekseerManager()
		{
			Finalize();
		}

	public:
		static IFEEffekseerManager* Instance();
		void Initialize();
		IFEEffekseer* Add(std::string name);
		void Update();
		void Draw();
		void Reset();
		void Finalize();
		IFEEffekseer* GetEffekseer(std::string name);

		void Loading();
	private:
		Effekseer::Matrix44 FromIFEToEffekseerMatrix(Matrix mat);
		Effekseer::Vector3D FromIFEToEffekseerVector3(Vector3 vec);

	public:
#ifdef EditorMode
		void DebugGUI();
		void Output();
#endif
	};
}
