#include "Tutorial.h"
#include "Input.h"
#include"SpriteManager.h"
#include"ColorBuffer.h"
#include"Scene.h"

void IFE::Tutorial::Initialize()
{
	isShowText = false;
	IFE::SpriteManager::Instance()->GetSpritePtr("BlackBack")->drawFlag_ = false;
	step = CAMERA;
	nowText;
}

void IFE::Tutorial::Update()
{
	switch (step)
	{		  
	case CAMERA:
		CameraText();
		break;
	case MOVE:
		
		break;
	case BACK:
		break;
	case ENEMY:
		break;
	}
	//ŽžŠÔŽ~‚ß
	if (isShowText) {
		IFE::IFETime::sTimeScale_ = 0.0f;
	}
	IFE::SpriteManager::Instance()->GetSpritePtr("BlackBack")->drawFlag_ = isShowText;
}

void IFE::Tutorial::CameraText()
{
	if (IFE::Input::GetKeyTrigger(IFE::Key::Space) || IFE::Input::PadTrigger(IFE::PADCODE::A)) {
		if (isShowText) {
			isShowText = false;
			IFE::IFETime::sTimeScale_ = 1.0f;
		}
		else {
			isShowText = true;
		}
	}
	/*if (isShowText) {
		IFE::SpriteManager::Instance()->GetSpritePtr(nowText)->drawFlag_ = false;
		NextText();
	}*/
}

void IFE::Tutorial::MoveText()
{
	  /*if (isShowText) {
		IFE::SpriteManager::Instance()->GetSpritePtr(nowText)->drawFlag_ = false;
		NextText();
	}*/
}

void IFE::Tutorial::NextText(const std::string& str)
{
	if (IFE::Input::GetKeyTrigger(IFE::Key::Space) || IFE::Input::PadTrigger(IFE::PADCODE::A)) {
		IFE::SpriteManager::Instance()->GetSpritePtr(nowText)->drawFlag_ = false;
		nowText = str;
		uint32_t tmp = 0;
		tmp = static_cast<uint32_t>(step);
		tmp++;
		step = static_cast<Step>(tmp);
	}
}

void IFE::Tutorial::Finalize()
{
}

void IFE::Tutorial::Reset()
{
}
