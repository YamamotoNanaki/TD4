#include "IFEEffekseerManager.h"
#include "GraphicsAPI.h"
#include "WindowsAPI.h"

void IFE::IFEEffekseerManager::Initialize()
{
 //   // Effekseer�̃}�l�[�W�����쐬
 //   auto manager = Effekseer::Manager::Create(8000);
 //   auto gapi = GraphicsAPI::Instance();

	//// Create a  graphics device
	//// �`��f�o�C�X�̍쐬
	//auto graphicsDevice = ::EffekseerRendererDX12::CreateGraphicsDevice(gapi->GetDevice(), gapi->GetCommandQueue(), 3);

	//// Create a renderer of effects
	//// �G�t�F�N�g�̃����_���[�̍쐬
	//auto format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//auto efkRenderer = ::EffekseerRendererDX12::Create(graphicsDevice, &format, 1, DXGI_FORMAT_UNKNOWN, false, 8000);

	//// Create a memory pool
	//// �������v�[���̍쐬
	//auto efkMemoryPool = EffekseerRenderer::CreateSingleFrameMemoryPool(efkRenderer->GetGraphicsDevice());

	//// Create a command list
	//// �R�}���h���X�g�̍쐬
	//auto efkCommandList = EffekseerRenderer::CreateCommandList(efkRenderer->GetGraphicsDevice(), efkMemoryPool);

	//// Sprcify rendering modules
	//// �`�惂�W���[���̐ݒ�
	//Effekseer::ManagerRef efkManager;
	//efkManager->SetSpriteRenderer(efkRenderer->CreateSpriteRenderer());
	//efkManager->SetRibbonRenderer(efkRenderer->CreateRibbonRenderer());
	//efkManager->SetRingRenderer(efkRenderer->CreateRingRenderer());
	//efkManager->SetTrackRenderer(efkRenderer->CreateTrackRenderer());
	//efkManager->SetModelRenderer(efkRenderer->CreateModelRenderer());

}
