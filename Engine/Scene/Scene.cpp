#include "Scene.h"
#include "Debug.h"
#include "Transform.h"
#include "Compute.h"
#include "IFETime.h"
#include "ColorBuffer.h"
#include "Ease.h"
#include "WindowsAPI.h"
#include "StructuredBuffer.h"
#include "Compute.h"
#include "CollideManager.h"

using namespace IFE;

void Test()
{

}

#ifdef NDEBUG
void IFE::Scene::Initialize()
{
	Sprite::StaticInitialize();
	Transform2D::StaticInitialize();
	gp_->CreateBasicGraphicsPipeLine();
	gp_->CreateAnimGraphicsPipeLine();
	gp_->CreateBasicParticleGraphicsPipeLine();
	gp_->CreateTransparentParticleGraphicsPipeLine();
	gp_->CreateSubParticleGraphicsPipeLine();
	Emitter::StaticInitialize();
	tex_->Initialize();
	objM_->OBJInitialize();
	spriteM_->SPRITEInitialize();
	lightM_->Initialize();
	sound_->Initialize();
	cameraM_->Initialize();
	oldPostEffect = std::make_unique<OldPostEffect>();
	oldPostEffect->Initialize();
	oldPostEffect->SetBrightness(0);
	oldPostEffect->SetContrast(1.25f);
	SceneInit();

	particleM->Initialize();
}

void IFE::Scene::Update()
{
	SceneChange();
	if (loadEnd_)
	{
		CollideManager::Instance()->Reset();
		objM_->Update();
		spriteM_->Update();
		particleM->Update();
		cameraM_->Update();
		lightM_->Update();
		CollideManager::Instance()->CollidersUpdate();
	}
	else
	{
		LoadUpdate();
	}
	PostEffectDraw();
}

void IFE::Scene::PostEffectDraw()
{
	oldPostEffect->DrawBefore();
	if (loadEnd_)
	{
		objM_->Draw();
		particleM->Draw();
	}
	oldPostEffect->DrawAfter();
}

void IFE::Scene::Draw()
{
	if (loadEnd_)
	{
		Sprite::DrawBefore();
		spriteM_->BackDraw();
		oldPostEffect->Draw();
		spriteM_->ForeDraw();
	}
	else
	{
		LoadDraw();
	}
}
#else
void IFE::Scene::Initialize()
{
	Test();
	Sprite::StaticInitialize();
	Transform2D::StaticInitialize();
	gp_->CreateBasicGraphicsPipeLine();
	gp_->CreateAnimGraphicsPipeLine();
	gp_->CreateBasicParticleGraphicsPipeLine();
	gp_->CreateSubParticleGraphicsPipeLine();
	gp_->CreateTransparentParticleGraphicsPipeLine();
	Emitter::StaticInitialize();
	tex_->Initialize();
	objM_->OBJInitialize();
	spriteM_->SPRITEInitialize();
	lightM_->Initialize();
	sound_->Initialize();
	gui_.Initialize();
	cameraM_->Initialize();
	oldPostEffect = std::make_unique<OldPostEffect>();
	oldPostEffect->Initialize();
	oldPostEffect->SetBrightness(0);
	oldPostEffect->SetContrast(1.25f);
	SceneInit();
	particleM->Initialize();
}

void IFE::Scene::Update()
{
	SceneChange();
	if (loadEnd_)
	{
		gui_.StartNewFrame();
		DebugGUI();
		gui_.Update();

		if (debug_ && !stop_)
		{
			CollideManager::Instance()->Reset();
			objM_->Update();
			spriteM_->Update();
			particleM->Update();
			cameraM_->Update();
			CollideManager::Instance()->CollidersUpdate();
		}
		else
		{
			objM_->DebugUpdate();
			spriteM_->DebugUpdate();
			particleM->DebugUpdate();
			cameraM_->DebugUpdate();
		}
		lightM_->Update();
	}
	else
	{
		LoadUpdate();
	}
	PostEffectDraw();
}

void IFE::Scene::PostEffectDraw()
{
	oldPostEffect->DrawBefore();
	if (loadEnd_)
	{
		objM_->Draw();
		particleM->Draw();
	}
	oldPostEffect->DrawAfter();
}

void IFE::Scene::Draw()
{
	if (loadEnd_)
	{
		Sprite::DrawBefore();
		spriteM_->BackDraw();
		oldPostEffect->Draw();
		spriteM_->ForeDraw();
		gui_.Draw();
	}
	else
	{
		LoadDraw();
	}
}
#endif

void IFE::Scene::Finalize()
{
	ModelManager::Finalize();
	ParticleManager::Finalize();
	Sound::Finalize();
#ifdef NDEBUG
#else
	JsonManager::Instance()->SetDebugScene();
#endif
}

Scene* IFE::Scene::Instance()
{
	static Scene inst;
	return &inst;
}

void IFE::Scene::SetNextScene(const std::string& n)
{
	nextScene_ = n;
	nextFlag_ = true;
}

#include "JsonManager.h"
void IFE::Scene::SceneChange()
{
	if (nextFlag_)
	{
		AsyncLoad();
	}
}

void IFE::Scene::SceneInit()
{
	nowScene_ = JsonManager::Instance()->SceneInit();
	if (nowScene_ == "")nowScene_ = "test";
	JsonManager::Instance()->SetSceneName(nowScene_);
	tex_->TexReset();
	objM_->Reset();
	modelM_->Reset();
	spriteM_->Reset();
	particleM->Reset();
	cameraM_->Reset();
	lightM_->Reset();
	tex_->LoadingScene();
	modelM_->LoadingScene();
	objM_->LoadingScene();
	spriteM_->LoadingScene();
	particleM->LoadingScene();
	cameraM_->LoadingScene();
	lightM_->LoadingScene();
	objM_->Initialize();
	spriteM_->Initialize();
	particleM->Initialize();
	oldPostEffect->LoadingScene();
}

void IFE::Scene::SceneTransitionIn()
{
	if (!isOut_)
	{
		transitionTimer_ += IFETime::sDeltaTime_;
		if (transitionTimer_ < maxTransitionTime_)
		{
			//�J�ڂ̉��o

			float alpha = InOutQuad(0, 1, maxTransitionTime_, transitionTimer_);
			//float y = EaseOutBounce(-(float)WindowsAPI::Instance()->winHeight_ / 2, (float)WindowsAPI::Instance()->winHeight_ / 2, maxTransitionTime_, transitionTimer_);
			auto* fade = spriteM_->GetSpritePtr("fade");
			if (!fade)
			{
				spriteM_->AddInitialize("fade", "white");
				fade = spriteM_->GetSpritePtr("fade");
				fade->GetComponent<Transform2D>()->position2D_ = Float2((float)WindowsAPI::Instance()->winWidth_ / 2, (float)WindowsAPI::Instance()->winHeight_ / 2);
				fade->GetComponent<Transform2D>()->scale2D_ = Float2(16, 9.1f);
			}
			//fade->transform_->position2D_.y = y;
			//fade->GetComponent<ColorBuffer>()->SetColor(0.7f, 0.7f, 0.7f, 1);
			fade->GetComponent<ColorBuffer>()->SetColor(0, 0, 0, alpha);
		}
		else
		{
			transitionTimer_ = 0;
			isOut_ = true;
			loadEnd_ = false;
			sound_->AllStop();
			sceneInitialize_ = std::async(std::launch::async, [this] {return LoadingScene(); });
		}
	}
}

void IFE::Scene::SceneTransitionOut()
{
	if (isOut_ && loadEnd_)
	{
		transitionTimer_ += IFETime::sDeltaTime_;
		if (transitionTimer_ < maxTransitionTime_)
		{
			//�J�ڂ̉��o

			float alpha = InOutQuad(1, 0, maxTransitionTime_, transitionTimer_);
			auto* fade = spriteM_->GetSpritePtr("fade");
			//float y = EaseInBounce((float)WindowsAPI::Instance()->winHeight_ / 2, 3 * (float)WindowsAPI::Instance()->winHeight_ / 2, maxTransitionTime_, transitionTimer_);
			if (!fade)
			{
				spriteM_->AddInitialize("fade", "white");
				fade = spriteM_->GetSpritePtr("fade");
				fade->GetComponent<Transform2D>()->position2D_ = Float2((float)WindowsAPI::Instance()->winWidth_ / 2, (float)WindowsAPI::Instance()->winHeight_ / 2);
				fade->GetComponent<Transform2D>()->scale2D_ = Float2(16, 9.1f);
			}
			//fade->transform_->position2D_.y = y;
			//fade->GetComponent<ColorBuffer>()->SetColor(0.7f, 0.7f, 0.7f, 1);
			fade->GetComponent<ColorBuffer>()->SetColor(0, 0, 0, alpha);
		}
		else
		{
			auto* fade = spriteM_->GetSpritePtr("fade");
			fade->spritePtr_->Destroy();
			transitionTimer_ = 0;
			isOut_ = false;
			nextFlag_ = false;
		}
	}
}

void IFE::Scene::LoadUpdate()
{
}

void IFE::Scene::LoadDraw()
{
}

void IFE::Scene::LoadingScene()
{
	JsonManager::Instance()->SetSceneName(nextScene_);
	objM_->Reset();
	spriteM_->Reset();
	particleM->Reset();
	cameraM_->Reset();
	lightM_->Reset();
	objM_->LoadingScene();
	spriteM_->LoadingScene();
	particleM->LoadingScene();
	lightM_->LoadingScene();
	objM_->Initialize();
	spriteM_->Initialize();
	particleM->Initialize();
	cameraM_->LoadingScene();
	oldPostEffect->LoadingScene();
	nowScene_ = nextScene_;
	loadEnd_ = true;
}

void IFE::Scene::AsyncLoad()
{
	SceneTransitionIn();
	SceneTransitionOut();
}

#ifdef NDEBUG
#else
void IFE::Scene::OutputScene()
{
	if (debug_)return;
	JsonManager::Instance()->MakeSceneDirectry();
	tex_->OutputScene();
	modelM_->OutputScene();
	objM_->OutputScene();
	spriteM_->OutputScene();
	particleM->OutputScene();
	cameraM_->OutputScene();
	oldPostEffect->OutputScene();
	lightM_->OutputScene();
}
#include "imgui.h"
void IFE::Scene::DebugGUI()
{
	gui_.NewGUI("SceneManager", 1024);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("scene save"))
		{
			bool s = false;
			ImGui::MenuItem("save", "", &s);
			if (s == true)
			{
				OutputScene();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	if (debug_)
	{
		if (stop_)
		{
			if (gui_.ButtonGUI("start"))
			{
				stop_ = false;
			}
		}
		else
		{
			if (gui_.ButtonGUI("Stop"))
			{
				stop_ = true;
			}
		}
		if (gui_.ButtonGUI("Debug End"))
		{
			debug_ = false;
			stop_ = false;
			nextScene_ = nowScene_;
			sound_->AllStop();
			LoadingScene();
			objM_->Initialize();
		}
	}
	else
	{
		if (gui_.ButtonGUI("Debug Start"))
		{
			OutputScene();
			debug_ = true;
			objM_->Initialize();
		}
	}
	static char name[256];
	static uint8_t errorNum = 0;
	ImGui::InputText("scene change name", name, sizeof(name));
	std::string sname = name;
	if (ImGui::Button("SceneChange"))
	{
		if (sname == "")
		{
			errorNum = 0b1;
		}
		else if (SceneCheck(sname))
		{
			errorNum = 0b10;
		}
		else
		{
			errorNum = 0b0;
			SetNextScene(sname);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Make this scene the initial scene"))
	{
		JsonManager::Instance()->SetInitScene();
	}
	if (errorNum & 0b1)
	{
		ImGui::Text("error : not entered");
	}
	if (errorNum & 0b10)
	{
		ImGui::Text("error : not found");
	}
	gui_.EndGUI();
	objM_->DebugGUI();
	spriteM_->DebugGUI();
	modelM_->DebugGUI();
	tex_->DebugGUI();
	cameraM_->DebugGUI();
	particleM->DebugGUI();
	lightM_->DebugGUI();
	oldPostEffect->DebugGUI();
}

bool IFE::Scene::SceneCheck(const std::string& sceneName)
{
	return JsonManager::Instance()->SceneCheck(sceneName);
}
#endif