#include "IFEEffekseerManager.h"
#include "GraphicsAPI.h"
#include "WindowsAPI.h"

#pragma comment (lib,"EffekseerRendererDX12.lib")
#pragma comment (lib,"Effekseer.lib")
#pragma comment (lib,"LLGI.lib")

void IFE::IFEEffekseerManager::Initialize()
{
	auto gapi = GraphicsAPI::Instance();

	// Effekseerのオブジェクトはスマートポインタで管理される。変数がなくなると自動的に削除される。

	// エフェクトのマネージャーの作成
	efkManager = ::Effekseer::Manager::Create(8000);

	// 描画デバイスの作成
	auto graphicsDevice = ::EffekseerRendererDX12::CreateGraphicsDevice(gapi->GetDevice(), gapi->GetCommandQueue(), 3);

	// エフェクトのレンダラーの作成
	auto format = DXGI_FORMAT_R8G8B8A8_UNORM;
	efkRenderer = ::EffekseerRendererDX12::Create(graphicsDevice, &format, 1, DXGI_FORMAT_UNKNOWN, false, 8000);

	// メモリプールの作成
	efkMemoryPool = EffekseerRenderer::CreateSingleFrameMemoryPool(efkRenderer->GetGraphicsDevice());

	// コマンドリストの作成
	efkCommandList = EffekseerRenderer::CreateCommandList(efkRenderer->GetGraphicsDevice(), efkMemoryPool);

	// 描画モジュールの設定
	efkManager->SetSpriteRenderer(efkRenderer->CreateSpriteRenderer());
	efkManager->SetRibbonRenderer(efkRenderer->CreateRibbonRenderer());
	efkManager->SetRingRenderer(efkRenderer->CreateRingRenderer());
	efkManager->SetTrackRenderer(efkRenderer->CreateTrackRenderer());
	efkManager->SetModelRenderer(efkRenderer->CreateModelRenderer());

	// テクスチャ、モデル、カーブ、マテリアルローダーの設定する。
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	efkManager->SetTextureLoader(efkRenderer->CreateTextureLoader());
	efkManager->SetModelLoader(efkRenderer->CreateModelLoader());
	efkManager->SetMaterialLoader(efkRenderer->CreateMaterialLoader());
	efkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
}

Effekseer::Matrix44 IFE::IFEEffekseerManager::FromIFEToEffekseerMatrix(Matrix mat)
{
	Effekseer::Matrix44 r;
	mat[0].ToArray(r.Values[0]);
	mat[1].ToArray(r.Values[1]);
	mat[2].ToArray(r.Values[2]);
	mat[3].ToArray(r.Values[3]);
	return r;
}

Effekseer::Vector3D IFE::IFEEffekseerManager::FromIFEToEffekseerVector3(Vector3 vec)
{
	return Effekseer::Vector3D(vec.x,vec.y,vec.z);
}