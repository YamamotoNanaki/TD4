#include "IFEEffekseerManager.h"
#include "GraphicsAPI.h"
#include "WindowsAPI.h"
#include "StringUtil.h"
#include "IFETime.h"
#include "CameraManager.h"
#include "JsonManager.h"

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

	// Effekseer�̃I�u�W�F�N�g�̓X�}�[�g�|�C���^�ŊǗ������B�ϐ����Ȃ��Ȃ�Ǝ����I�ɍ폜�����B

	// �G�t�F�N�g�̃}�l�[�W���[�̍쐬
	efkManager_ = ::Effekseer::Manager::Create(10000);

	// �G�t�F�N�g�̃����_���[�̍쐬
	auto format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	efkRenderer_ = ::EffekseerRendererDX12::Create(gapi->GetDevice(), gapi->GetCommandQueue(), 3, &format, 1, DXGI_FORMAT_D32_FLOAT, false, 10000);

	efkManager_->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	// �`�惂�W���[���̐ݒ�
	efkManager_->SetSpriteRenderer(efkRenderer_->CreateSpriteRenderer());
	efkManager_->SetRibbonRenderer(efkRenderer_->CreateRibbonRenderer());
	efkManager_->SetRingRenderer(efkRenderer_->CreateRingRenderer());
	efkManager_->SetTrackRenderer(efkRenderer_->CreateTrackRenderer());
	efkManager_->SetModelRenderer(efkRenderer_->CreateModelRenderer());

	// �e�N�X�`���A���f���A�J�[�u�A�}�e���A�����[�_�[�̐ݒ肷��B
	// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���ł���B
	efkManager_->SetTextureLoader(efkRenderer_->CreateTextureLoader());
	efkManager_->SetModelLoader(efkRenderer_->CreateModelLoader());
	efkManager_->SetMaterialLoader(efkRenderer_->CreateMaterialLoader());
	efkManager_->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

	// �������v�[���̍쐬
	efkMemoryPool_ = EffekseerRenderer::CreateSingleFrameMemoryPool(efkRenderer_->GetGraphicsDevice());

	// �R�}���h���X�g�̍쐬
	efkCommandList_ = EffekseerRenderer::CreateCommandList(efkRenderer_->GetGraphicsDevice(), efkMemoryPool_);
	efkRenderer_->SetCommandList(efkCommandList_);
}

IFE::IFEEffekseer* IFE::IFEEffekseerManager::Add(std::string name)
{
	std::u16string s = u"Data/Resources/Effekseer/" + StringToU16String(name);

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
	efkMemoryPool_->NewFrame();
	efkManager_->Update();

	effekList_.remove_if([&](std::unique_ptr<IFEEffekseer>& effek) {
		return effek->GetDeleteFlag(); });
	effectHandles_.remove_if([&](Effekseer::Handle handle) {
		return !efkManager_->Exists(handle); });

	for (auto& ptr : effekList_)
	{
		ptr->Update();
	}
	// �G�t�F�N�g�̍X�V
}

void IFE::IFEEffekseerManager::Draw()
{
	EffekseerRendererDX12::BeginCommandList(efkCommandList_, GraphicsAPI::Instance()->GetCmdList());

	// �����_���[�̎��Ԃ��X�V
	efkRenderer_->SetTime(IFETime::sTime_);

	// �J�����s��ƃv���W�F�N�V�����s���ݒ�
	efkRenderer_->SetCameraMatrix(FromIFEToEffekseerMatrix(CameraManager::sActivCamera_->GetView()->Get()));
	efkRenderer_->SetProjectionMatrix(FromIFEToEffekseerMatrix(CameraManager::sActivCamera_->GetProjection()->Get()));

	efkRenderer_->BeginRendering();
	Effekseer::Manager::DrawParameter drawParameter;
	drawParameter.ZNear = 0.0f;
	drawParameter.ZFar = 1.0f;
	drawParameter.ViewProjectionMatrix = efkRenderer_->GetCameraProjectionMatrix();
	efkManager_->Draw(drawParameter);
	efkRenderer_->EndRendering();

	EffekseerRendererDX12::EndCommandList(efkCommandList_);
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

IFE::IFEEffekseer* IFE::IFEEffekseerManager::GetEffekseer(std::string name)
{
	for (auto& itr : effekList_)
	{
		if (MatchesPrefix(itr->name_, name))
		{
			return itr.get();
		}
	}
	return nullptr;
}

void IFE::IFEEffekseerManager::Loading()
{
	JsonManager* jm = JsonManager::Instance();
	jm->Input("EffekserManager");
	nlohmann::json js = jm->GetJsonData();
	for (auto& j : js)
	{
		std::string name;
		if (!jm->GetData(j, "name", name))continue;

		auto e = Add(name);
		jm->GetData(j, "offset", e->timeOffset_);
		jm->GetData(j, "scaleFlag", e->timeScaleFlag_);
	}
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

#ifdef EditorMode
#include "ImguiManager.h"
void IFE::IFEEffekseerManager::Output()
{
	JsonManager* jm = JsonManager::Instance();
	nlohmann::json& j = jm->GetJsonData();
	uint32_t i = 0;
	for (auto& itr : effekList_)
	{
		j[i]["name"] = itr->name_;
		j[i]["offset"] = itr->timeOffset_;
		j[i]["scaleFlag"] = itr->timeScaleFlag_;
		i++;
	}
	jm->Output("EffekserManager");
}

void IFE::IFEEffekseerManager::DebugGUI()
{
	ImguiManager* im = ImguiManager::Instance();
	im->NewGUI(U8("Effekseer�}�l�[�W���["));
	if (im->CollapsingHeaderGUI(U8("�ǉ�")))
	{
		static std::string n;
		im->TextGUI(U8("�g���q�K�v"));
		im->InputTextGUI(U8("�G�t�F�N�g��"), n);

		if (im->ButtonGUI(U8("����")))
		{
			Add(n);
		}
	}
	for (auto& itr : effekList_)
	{
		if (im->NewTreeNode(itr->name_))
		{
			itr->DebugGUI();
			if (im->ButtonGUI(U8("�폜")))
			{
				itr->Destroy();
			}
			im->EndTreeNode();
		}
	}

	im->EndGUI();
}
#endif
