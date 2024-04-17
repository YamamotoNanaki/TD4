#include "ParticleManager.h"
#include "Transform.h"
#include "TextureManager.h"

using namespace IFE;
using namespace std;

ParticleManager* IFE::ParticleManager::Instance()
{
	static ParticleManager* inst = new ParticleManager;
	return inst;
}

void IFE::ParticleManager::Finalize()
{
	delete Instance();
}

void IFE::ParticleManager::Initialize()
{
	for (unique_ptr<Emitter>& itr : emitterList)
	{
		itr->Initialize();
	}
}

void IFE::ParticleManager::Update()
{
	for (unique_ptr<Emitter>& itr : emitterList)
	{
		itr->Update();
	}
}

void IFE::ParticleManager::Draw()
{
	//Emitter::DrawBefore();
	for (unique_ptr<Emitter>& itr : emitterList)
	{
		itr->Draw();
	}
}

void IFE::ParticleManager::AddInitialize(std::string emitterName, std::string textureName)
{
	std::unique_ptr<Emitter> ptr = make_unique<Emitter>();
	ptr->Initialize();
	ptr->emitterName = emitterName;
	ptr->SetTexture(textureName);
	emitterList.push_back(std::move(ptr));
}

Emitter* IFE::ParticleManager::Add(std::string emitterName)
{
	std::unique_ptr<Emitter> ptr = make_unique<Emitter>();
	ptr->emitterName = emitterName;
	emitterList.push_back(std::move(ptr));
	return emitterList.back().get();
}

Emitter* IFE::ParticleManager::GetEmitterPtr(std::string emitterName)
{
	for (unique_ptr<Emitter>& itr : emitterList)
	{
		if (itr->emitterName == emitterName)
		{
			return itr.get();
		}
	}
	return nullptr;
}

Emitter* IFE::ParticleManager::Instantiate(Emitter* gameObject, Float3 position, float rotation)
{
	std::unique_ptr<Emitter> ptr = make_unique<Emitter>();
	ptr->deleteFlag = gameObject->deleteFlag;
	ptr->emitterDeleteFlag = gameObject->emitterDeleteFlag;
	ptr->transform = gameObject->transform;
	ptr->isActive = gameObject->isActive;
	ptr->DrawFlag = gameObject->DrawFlag;
	ptr->tex = gameObject->tex;
	string objectName = GetNewName(gameObject->emitterName);
	ptr->emitterName = objectName;
	emitterList.push_back(std::move(ptr));
	Emitter* obj = emitterList.back().get();
	obj->transform->position = position;
	obj->transform->rotation = rotation;
	return obj;
}

Emitter* IFE::ParticleManager::Instantiate(Emitter* gameObject, Float3 position)
{
	position;
	std::unique_ptr<Emitter> ptr = make_unique<Emitter>();
	//*ptr = *gameObject;
	ptr->deleteFlag = gameObject->deleteFlag;
	ptr->emitterDeleteFlag = gameObject->emitterDeleteFlag;
	ptr->transform = gameObject->transform;
	ptr->isActive = gameObject->isActive;
	ptr->DrawFlag = gameObject->DrawFlag;
	ptr->tex = gameObject->tex;
	std::string emitterName;
	string objectName = GetNewName(gameObject->emitterName);
	ptr->emitterName = objectName;
	emitterList.push_back(std::move(ptr));
	Emitter* obj = emitterList.back().get();
	obj->transform->position = position;
	return obj;
}

Emitter* IFE::ParticleManager::Instantiate(Emitter* gameObject)
{
	std::unique_ptr<Emitter> ptr = make_unique<Emitter>();
	ptr->deleteFlag = gameObject->deleteFlag;
	ptr->emitterDeleteFlag = gameObject->emitterDeleteFlag;
	ptr->transform = gameObject->transform;
	ptr->isActive = gameObject->isActive;
	ptr->DrawFlag = gameObject->DrawFlag;
	ptr->tex = gameObject->tex;
	string objectName = GetNewName(gameObject->emitterName);
	ptr->emitterName = objectName;
	emitterList.push_back(std::move(ptr));
	Emitter* obj = emitterList.back().get();
	return obj;
}

void IFE::ParticleManager::SetTexture(std::string emitterName, std::string texName)
{
	texName;
	for (unique_ptr<Emitter>& itr : emitterList)
	{
		if (itr->emitterName == emitterName)
		{
			itr->SetTexture(texName);
			return;
		}
	}
}

std::list<std::unique_ptr<Emitter>>* IFE::ParticleManager::GetEmitterList()
{
	return &emitterList;
}

void IFE::ParticleManager::Reset()
{
	emitterList.clear();
}

bool IFE::ParticleManager::SearchName(std::string name)
{
	for (unique_ptr<Emitter>& itr : emitterList)
	{
		if (itr->emitterName == name)return true;
	}
	return false;
}

#include "JsonManager.h"
#ifdef _DEBUG
#include "ImguiManager.h"
#include "Imgui.h"
void IFE::ParticleManager::DebugGUI()
{
	static bool add = false;
	static bool fdelete = false;
	static bool fmove = false;
	ImGui::Begin("EmitterManager", (bool*)false, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("add"))
		{
			ImGui::MenuItem("Add", "", &add);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("delete"))
		{
			ImGui::MenuItem("delete", "", &fdelete);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	if (add)
	{
		if (ImGui::CollapsingHeader("New Emitter"))
		{
			static char name[256];
			static char tex[256];
			ImGui::InputText("New Emitter Name", name, sizeof(name));
			ImGui::InputText("Set Texture Name", tex, sizeof(tex));
			if (ImGui::Button("Add Emitter"))
			{
				if (SearchName(name))
				{
					ImGui::Text("error : an object with the same name exists");
				}
				else
				{
					AddInitialize(name, tex);
					ImGui::Text("success");
				}
			}
		}
	}
	for (unique_ptr<Emitter>& itr : emitterList)
	{
		itr->DebugGUI(fdelete);
	}
	ImGui::End();
	for (unique_ptr<Emitter>& itr : emitterList)
	{
		itr->ComponentGUI();
	}
}

void IFE::ParticleManager::OutputScene()
{
	JsonManager* jm = JsonManager::Instance();
	int i = 0;
	for (unique_ptr<Emitter>& itr : emitterList)
	{
		nlohmann::json& js = jm->GetJsonData();
		js[i] = itr->emitterName;
		i++;
	}
	jm->OutputAndMakeDirectry("EmitterManager", "Emitter");
	for (unique_ptr<Emitter>& itr : emitterList)
	{
		nlohmann::json& js = jm->GetJsonData();
		vector<string>names = itr->GetAllComponentName();
		for (int j = 0; j < names.size(); j++)
		{
			js["com"][j] = names[j];
		}
		js["tex"] = itr->tex->texName;
		jm->OutputAndMakeDirectry(itr->emitterName, "Emitter");
	}
	for (unique_ptr<Emitter>& itr : emitterList)
	{
		itr->OutputScene();
	}
}

void IFE::ParticleManager::DebugUpdate()
{
	emitterList.remove_if([](unique_ptr<Emitter>& obj) {return obj->deleteFlag; });
	for (unique_ptr<Emitter>& itr : emitterList)
	{
		itr->DebugUpdate();
	}
}
#endif
#include "ComponentHelp.h"

void IFE::ParticleManager::LoadingScene()
{
	JsonManager* jm = JsonManager::Instance();
	jm->Input("Emitter/EmitterManager");
	nlohmann::json js = jm->GetJsonData();
	for (auto itr : js)
	{
		Add(itr);
	}

	jm->JsonReset();
	for (unique_ptr<Emitter>& itr : emitterList)
	{
		string s = "Emitter/" + itr->emitterName;
		jm->Input(s);
		nlohmann::json js2 = jm->GetJsonData();
		vector<string>names;
		for (auto itr2 : js2["com"])
		{
			names.push_back(itr2);
		}
		itr->tex = TextureManager::Instance()->GetTexture(js2["tex"]);
		jm->JsonReset();
		for (int i = 0; i < names.size(); i++)
		{
			//‚ ‚Æ‚Å•Ê‚Ìcpp‚É
			Component* base = nullptr;

			base = StringToComponent(names[i]);
			base->LoadingScene(itr->emitterName, names[i]);
			itr->SetComponent(base);
		}
		itr->Initialize();
	}
}

std::string IFE::ParticleManager::GetNewName(std::string emitterName)
{
	int i = 0;
	string r;
	while (true)
	{
		string s = emitterName + to_string(i);
		if (SearchName(s))
		{
			i++;
		}
		else
		{
			r = s;
			break;
		}
	}
	return r;
}
