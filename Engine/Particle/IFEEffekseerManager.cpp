#include "IFEEffekseerManager.h"
#include "GraphicsAPI.h"
#include "WindowsAPI.h"

void IFE::IFEEffekseerManager::Initialize()
{
 //   // Effekseerのマネージャを作成
 //   auto manager = Effekseer::Manager::Create(8000);
 //   auto gapi = GraphicsAPI::Instance();

	//// Create a  graphics device
	//// 描画デバイスの作成
	//auto graphicsDevice = ::EffekseerRendererDX12::CreateGraphicsDevice(gapi->GetDevice(), gapi->GetCommandQueue(), 3);

	//// Create a renderer of effects
	//// エフェクトのレンダラーの作成
	//auto format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//auto efkRenderer = ::EffekseerRendererDX12::Create(graphicsDevice, &format, 1, DXGI_FORMAT_UNKNOWN, false, 8000);

	//// Create a memory pool
	//// メモリプールの作成
	//auto efkMemoryPool = EffekseerRenderer::CreateSingleFrameMemoryPool(efkRenderer->GetGraphicsDevice());

	//// Create a command list
	//// コマンドリストの作成
	//auto efkCommandList = EffekseerRenderer::CreateCommandList(efkRenderer->GetGraphicsDevice(), efkMemoryPool);

	//// Sprcify rendering modules
	//// 描画モジュールの設定
	//Effekseer::ManagerRef efkManager;
	//efkManager->SetSpriteRenderer(efkRenderer->CreateSpriteRenderer());
	//efkManager->SetRibbonRenderer(efkRenderer->CreateRibbonRenderer());
	//efkManager->SetRingRenderer(efkRenderer->CreateRingRenderer());
	//efkManager->SetTrackRenderer(efkRenderer->CreateTrackRenderer());
	//efkManager->SetModelRenderer(efkRenderer->CreateModelRenderer());

}
