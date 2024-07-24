#include "IFEEffekseer.h"
#include "IFETime.h"

void IFE::IFEEffekseer::Play(IFE::Float3 position, IFE::Float3 rotation, IFE::Float3 scale)
{
	Effekseer::Handle h = manager_->Play(effect_, 0, 0, 0);

	manager_->SetLocation(h, position[0], position[1], position[2]);
	manager_->SetRotation(h, ConvertToRadians(rotation[0]), ConvertToRadians(rotation[1]), ConvertToRadians(rotation[2]));
	manager_->SetScale(h, scale[0], scale[1], scale[2]);

	if (timeScaleFlag_)manager_->SetSpeed(h, IFETime::sTimeScale_ * timeOffset_); // 2倍速で再生
	else manager_->SetSpeed(h, timeOffset_);
	playList_.push_back(h);
	handles_->push_back(h);
}

void IFE::IFEEffekseer::Update()
{
	if (destroyFlag_ && playList_.size() == 0)deleteFlag_ = true;
	playList_.remove_if([&](Effekseer::Handle handle) {
		return !manager_->Exists(handle); });
}

void IFE::IFEEffekseer::Draw()
{
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

#ifdef EditorMode
#include "ImguiManager.h"
void IFE::IFEEffekseer::DebugGUI()
{
	auto im = ImguiManager::Instance();

	im->CheckBoxGUI(&timeScaleFlag_, U8("タイムスケール依存の場合true"));
	im->DragFloatGUI(&timeOffset_, U8("タイムのオフセット倍率"), 0.01f, 0, 10000);

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
