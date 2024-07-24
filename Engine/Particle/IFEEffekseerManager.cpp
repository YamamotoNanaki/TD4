#include "IFEEffekseerManager.h"
#include "GraphicsAPI.h"
#include "WindowsAPI.h"
#include "StringUtil.h"
#include "IFETime.h"
#include "CameraManager.h"

#pragma comment (lib,"EffekseerRendererDX12.lib")
#pragma comment (lib,"Effekseer.lib")
#pragma comment (lib,"LLGI.lib")

IFE::IFEEffekseerManager* IFE::IFEEffekseerManager::Instance()
{
	static IFEEffekseerManager inst;
	return &inst;
}

void IFE::IFEEffekseerManager::Initialize()
{
	auto gapi = GraphicsAPI::Instance();

	// Effekseerのオブジェクトはスマートポインタで管理される。変数がなくなると自動的に削除される。

	// エフェクトのマネージャーの作成
	efkManager_ = ::Effekseer::Manager::Create(8000);

	// 描画デバイスの作成
	auto graphicsDevice = ::EffekseerRendererDX12::CreateGraphicsDevice(gapi->GetDevice(), gapi->GetCommandQueue(), 3);

	// エフェクトのレンダラーの作成
	auto format = DXGI_FORMAT_R8G8B8A8_UNORM;
	efkRenderer_ = ::EffekseerRendererDX12::Create(graphicsDevice, &format, 1, DXGI_FORMAT_UNKNOWN, false, 8000);

	// メモリプールの作成
	efkMemoryPool_ = EffekseerRenderer::CreateSingleFrameMemoryPool(efkRenderer_->GetGraphicsDevice());

	// コマンドリストの作成
	efkCommandList_ = EffekseerRenderer::CreateCommandList(efkRenderer_->GetGraphicsDevice(), efkMemoryPool_);

	// 描画モジュールの設定
	efkManager_->SetSpriteRenderer(efkRenderer_->CreateSpriteRenderer());
	efkManager_->SetRibbonRenderer(efkRenderer_->CreateRibbonRenderer());
	efkManager_->SetRingRenderer(efkRenderer_->CreateRingRenderer());
	efkManager_->SetTrackRenderer(efkRenderer_->CreateTrackRenderer());
	efkManager_->SetModelRenderer(efkRenderer_->CreateModelRenderer());

	// テクスチャ、モデル、カーブ、マテリアルローダーの設定する。
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	efkManager_->SetTextureLoader(efkRenderer_->CreateTextureLoader());
	efkManager_->SetModelLoader(efkRenderer_->CreateModelLoader());
	efkManager_->SetMaterialLoader(efkRenderer_->CreateMaterialLoader());
	efkManager_->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
}

IFE::IFEEffekseer* IFE::IFEEffekseerManager::Add(std::string name)
{
	std::u16string s = u"Data/Resources/Effekseer/" + StringToU16String(name) + u".efkefc";

	auto effect = Effekseer::Effect::Create(efkManager_, s.c_str());
	if (!effect.Get())return nullptr;
	effekList_.push_back(std::move(std::make_unique<IFEEffekseer>()));
	auto ptr = effekList_.back().get();
	ptr->Initiaize(efkManager_, effect, &effectHandles_);
	ptr->name_ = name;

	return ptr;
}

void IFE::IFEEffekseerManager::Update()
{
	efkManager_->Update();

	effekList_.remove_if([&](std::unique_ptr<IFEEffekseer>& effek) {
		return effek->GetDeleteFlag(); });
	effectHandles_.remove_if([&](Effekseer::Handle handle) {
		return !efkManager_->Exists(handle); });

	for (auto& ptr : effekList_)
	{
		ptr->Update();
	}
	// エフェクトの更新
}

void IFE::IFEEffekseerManager::Draw()
{
	// レンダラーの時間を更新
	efkRenderer_->SetTime(IFETime::sTime_);

	// カメラ行列とプロジェクション行列を設定
	efkRenderer_->SetCameraMatrix(FromIFEToEffekseerMatrix(CameraManager::sActivCamera_->GetView()->Get()));
	efkRenderer_->SetProjectionMatrix(FromIFEToEffekseerMatrix(CameraManager::sActivCamera_->GetProjection()->Get()));


	efkRenderer_->BeginRendering();
	efkManager_->Draw();
	efkRenderer_->EndRendering();
}

void IFE::IFEEffekseerManager::Reset()
{
	effectHandles_.clear();
	effekList_.clear();
}

void IFE::IFEEffekseerManager::Finalize()
{
	Reset();
}

void IFE::IFEEffekseerManager::Loading()
{
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
	return Effekseer::Vector3D(vec.x, vec.y, vec.z);
}

void IFE::IFEEffekseerManager::Output()
{
}

#ifdef EditorMode
#include "ImguiManager.h"
void IFE::IFEEffekseerManager::DebugGUI()
{
	ImguiManager* im = ImguiManager::Instance();
	im->NewGUI(U8("Effekseerマネージャー"));
	if (im->CollapsingHeaderGUI(U8("追加")))
	{
		static std::string n;
		im->TextGUI(U8("拡張子はいらないです"));
		im->InputTextGUI(U8("エフェクト名"), n);

		if (im->ButtonGUI(U8("生成")))
		{
			Add(n);
		}
	}
	for (auto& itr : effekList_)
	{
		if (im->NewTreeNode(itr->name_))
		{
			itr->DebugGUI();
			if (im->ButtonGUI(U8("削除")))
			{
				itr->Destroy();
			}
			im->EndTreeNode();
		}
	}

	im->EndGUI();
}
#endif
