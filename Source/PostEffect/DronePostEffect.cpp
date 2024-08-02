#include "DronePostEffect.h"
#include "GraphicsPipelineManager.h"
#include "PostEffectManager.h"
#include "Rand.h"
#include "IFETime.h"
#include "TextureManager.h"

using namespace IFE;
using namespace std;

void DronePostEffect::Initialize()
{
	name_ = "DronePostEffect";
	gp_ = GraphicsPipelineManager::Instance()->GetGraphicsPipeline("DronePostEffect");
	cb_ = make_unique<IFE::ConstBuffer<DroneCameraPE>>();
	buff_ = cb_->GetCBMapObject();
	buff_->drone = false;
	updateFlag_ = true;
	droneFlag_ = false;
	PostEffectManager::Instance()->GetDefaultPE()->drawFlag_ = false;
}

void DronePostEffect::Update()
{
	if (droneFlag_)
	{
		buff_->drone = true;
	}
	else
	{
		buff_->drone = false;
	}
	buff_->time = IFETime::sTime_;
	PostEffectDrawBefore();
	Sprite::DrawBefore();
	PostEffectManager::Instance()->GetPostEffect("RadialBlurPE")->ForcedDraw();
	PostEffectDrawAfter();
}

void DronePostEffect::Draw()
{
	tex_[0]->SetTexture(0);
	TextureManager::Instance()->GetTexture("EnemyHighlighting_Render_0")->SetTexture(1);
	cb_->SetConstBuffView(2);
}

#ifdef EditorMode
#include "ImguiManager.h"
void DronePostEffect::PostEffectDebugGUI()
{
	auto im = ImguiManager::Instance();
	im->TextGUI(U8("ポストエフェクト確認用"));
	//static float noiseWidth = 0;
	//static float noisePower = 0;
	//static float noisePosY = 0;
	static bool drone = false;
	//im->DragFloatGUI(&noiseWidth, "noiseWidth", 0.01f, 0, 1);
	//im->DragFloatGUI(&noisePower, "noisePower", 0.01f);
	//im->DragFloatGUI(&noisePosY, "noisePosY", 0.01f, 0, 1);
	im->CheckBoxGUI(&drone, "drone flag");
	//buff_->noiseWidth = noiseWidth;
	//buff_->noisePower = noisePower;
	//buff_->noisePosY = noisePosY;
	buff_->drone = drone;
}
#endif
