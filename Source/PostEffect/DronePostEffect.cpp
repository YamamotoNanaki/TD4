#include "DronePostEffect.h"
#include "GraphicsPipelineManager.h"
#include "PostEffectManager.h"
#include "Rand.h"
#include "IFETime.h"

using namespace IFE;
using namespace std;

void DronePostEffect::Initialize()
{
	name_ = "DronePostEffect";
	gp_ = GraphicsPipelineManager::Instance()->CreatePostEffectPipeLine("DronePostEffect", 2, 1, 1);
	eh_ = PostEffectManager::Instance()->GetPostEffect("EnemyHighlighting");
	dp_ = PostEffectManager::Instance()->GetPostEffect("DefaultPostEffect");
	cb_ = make_unique<IFE::ConstBuffer<DroneCameraPE>>();
	buff_ = cb_->GetCBMapObject();
	buff_->drone = false;
	updateFlag_ = true;
	droneFlag_ = false;
}

void DronePostEffect::Update()
{
	if (droneFlag_)
	{
		buff_->drone = true;
		if (maxTimer_ == -1)
		{
			maxTimer_ = IFERand::GetRandF(0.25f, 1.5f);
		}
		timer_ += IFETime::sDeltaTime_;
		if (timer_ >= maxTimer_ && noiseMaxNum_ == noiseNum_)
		{
			noise_ = !noise_;
			timer_ = 0;
			if (noise_)
			{
				noiseMaxNum_ = uint8_t(IFERand::GetRand(1, 3));
				maxTimer_ = IFERand::GetRandF(0.05f, 0.15f);
				noiseNum_ = 0;
			}
			else
			{
				maxTimer_ = IFERand::GetRandF(0.25f, 1.5f);
				noiseNum_ = 0;
				noiseMaxNum_ = 0;
			}
		}
		static float pos = 0;
		static float pow = 0;
		static float wid = 0;
		if (noise_)
		{
			if (timer_ >= maxTimer_)
			{
				maxTimer_ = IFERand::GetRandF(0.05f, 0.15f);
				timer_ = 0;
				noiseNum_++;
				pos = IFERand::GetRandF(0, 1);
				pow = IFERand::GetRandF(0.025f, 0.15f);
				wid = IFERand::GetRandF(0.025f, 0.05f);
			}
		}
		else
		{
			pos = 0;
			pow = 0;
			wid = 0;
		}
		buff_->noisePosY = pos;
		buff_->noisePower = pow;
		buff_->noiseWidth = wid;
	}
	else
	{
		buff_->drone = false;
		noise_ = false;
		maxTimer_ = -1;
		timer_ = 0;
	}
	PostEffectDrawBefore();
	Sprite::DrawBefore();
	dp_->ForcedDraw();
	eh_->ForcedDraw();
	PostEffectDrawAfter();
}

void DronePostEffect::Draw()
{
	tex_[0]->SetTexture(0);
	tex_[1]->SetTexture(1);
	cb_->SetConstBuffView(2);
}

#ifdef EditorMode
#include "ImguiManager.h"
void DronePostEffect::PostEffectDebugGUI()
{
	auto im = ImguiManager::Instance();
	im->TextGUI(U8("ポストエフェクト確認用"));
	static float noiseWidth = 0;
	static float noisePower = 0;
	static float noisePosY = 0;
	static bool drone = false;
	im->DragFloatGUI(&noiseWidth, "noiseWidth", 0.01f, 0, 1);
	im->DragFloatGUI(&noisePower, "noisePower", 0.01f);
	im->DragFloatGUI(&noisePosY, "noisePosY", 0.01f, 0, 1);
	im->CheckBoxGUI(&drone, "drone flag");
	buff_->noiseWidth = noiseWidth;
	buff_->noisePower = noisePower;
	buff_->noisePosY = noisePosY;
	buff_->drone = drone;
}
#endif
