#include "IFEEffekseer.h"
#include "IFETime.h"

void IFE::IFEEffekseer::Play(IFE::Float3 position, IFE::Float3 rotation, IFE::Float3 scale)
{
	Effekseer::Handle h = manager_->Play(effect_, 0, 0, 0);

	manager_->SetLocation(h, position[0], position[1], position[2]);
	manager_->SetRotation(h, ConvertToRadians(rotation[0]), ConvertToRadians(rotation[1]), ConvertToRadians(rotation[2]));
	manager_->SetScale(h, scale[0], scale[1], scale[2]);

	if (timeScaleFlag_)manager_->SetSpeed(h, IFETime::sTimeScale_ * timeOffset_);
	else manager_->SetSpeed(h, timeOffset_);

	playList_.push_back(std::make_pair(h, nullptr));
	handles_->push_back(h);
}

void IFE::IFEEffekseer::Play(IFE::Float3* position, IFE::Float3 rotation, IFE::Float3 scale)
{
	Effekseer::Handle h = manager_->Play(effect_, 0, 0, 0);

	manager_->SetLocation(h, position->x, position->y, position->z);
	manager_->SetRotation(h, ConvertToRadians(rotation[0]), ConvertToRadians(rotation[1]), ConvertToRadians(rotation[2]));
	manager_->SetScale(h, scale[0], scale[1], scale[2]);

	if (timeScaleFlag_)manager_->SetSpeed(h, IFETime::sTimeScale_ * timeOffset_);
	else manager_->SetSpeed(h, timeOffset_);
	playList_.push_back(std::make_pair(h, nullptr));
	handles_->push_back(h);
}

void IFE::IFEEffekseer::Update()
{
	if (playList_.size() == 0)
	{
		if (destroyFlag_)deleteFlag_ = true;
		return;
	}

	playList_.remove_if([&](std::pair<Effekseer::Handle,Float3*> h) {
		return !manager_->Exists(h.first); });

	for (auto itr : playList_)
	{
		if (timeScaleFlag_)
		{
			manager_->SetSpeed(itr.first, IFETime::sTimeScale_ * timeOffset_);
		}
		if (itr.second)
		{
			manager_->SetLocation(itr.first, itr.second->x, itr.second->y, itr.second->z);
		}
	}
}

void IFE::IFEEffekseer::Initiaize(::Effekseer::ManagerRef manager, Effekseer::EffectRef effect, std::list<Effekseer::Handle>* handle)
{
	manager_ = manager;
	effect_ = effect;
	handles_ = handle;
}

void IFE::IFEEffekseer::Destroy()
{
	destroyFlag_ = true;
	if (playList_.size() == 0)deleteFlag_ = true;
}

void IFE::IFEEffekseer::SetPosition(uint32_t num, IFE::Float3 position)
{
	if (num >= playList_.size())return;

	auto it = playList_.begin();
	std::advance(it, num);

	manager_->SetLocation(it->first, position[0], position[1], position[2]);
}

#ifdef EditorMode
#include "ImguiManager.h"

void IFE::IFEEffekseer::DebugGUI()
{
	auto im = ImguiManager::Instance();

	im->CheckBoxGUI(&timeScaleFlag_, U8("タイムスケール依存の場合true"));
	im->DragFloatGUI(&timeOffset_, U8("タイムのオフセット倍率"), 0.01f, 0.1f, 10000);

	im->TextIntGUI(U8("現在の再生数"), int32_t(playList_.size()));
	if (im->NewTreeNode(U8("テスト再生")))
	{
		static Float3 pos = { 0,0,0 };
		static Float3 rot = { 0,0,0 };
		static Float3 sca = { 1,1,1 };
		im->DragFloat3GUI(&pos, U8("ポジション"));
		im->DragFloat3GUI(&rot, U8("ローテーション"));
		im->DragFloat3GUI(&sca, U8("スケール"));
		if (im->ButtonGUI(U8("リセット")))
		{
			pos = { 0,0,0 };
			rot = { 0,0,0 };
			sca = { 1,1,1 };
		}
		if (im->ButtonGUI(U8("再生")))
		{
			Play(pos, rot, sca);
		}
		im->EndTreeNode();
	}

}
#endif
