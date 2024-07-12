#include "SpriteManager.h"
#include "Transform.h"
#include "TextureManager.h"
#include "IFEList.h"
#include <regex>

using namespace std;
using namespace IFE;

SpriteManager* IFE::SpriteManager::Instance()
{
	static SpriteManager inst;
	return &inst;
}

void IFE::SpriteManager::SPRITEInitialize()
{
	for (unique_ptr<Sprite>& itr : backgroundList_)
	{
		itr->SPRITEInitialize();
	}
	for (unique_ptr<Sprite>& itr : foregroundList_)
	{
		itr->SPRITEInitialize();
	}
}

void IFE::SpriteManager::Initialize()
{
	for (unique_ptr<Sprite>& itr : backgroundList_)
	{
		itr->Initialize();
	}
	for (unique_ptr<Sprite>& itr : foregroundList_)
	{
		itr->Initialize();
	}
}

void IFE::SpriteManager::Update()
{
	backgroundList_.remove_if([](unique_ptr<Sprite>& spr) {return spr->GetDeleteFlag(); });
	foregroundList_.remove_if([](unique_ptr<Sprite>& spr) {return spr->GetDeleteFlag(); });
	for (unique_ptr<Sprite>& itr : backgroundList_)
	{
		itr->Update();
	}
	for (unique_ptr<Sprite>& itr : foregroundList_)
	{
		itr->Update();
	}
}

void IFE::SpriteManager::ForeDraw()
{
	Sprite::DrawBefore();

	list<Sprite*>sprList;
	for (unique_ptr<Sprite>& itr : foregroundList_)
	{
		if (!itr->isActive_)continue;
		if (!itr->drawFlag_)continue;
		sprList.push_back(itr.get());
	}

	sprList.sort([](const Sprite* sprA, const Sprite* sprB) {return sprA->order_ > sprB->order_; });
	for (auto& itr : sprList)
	{
		itr->Draw();
	}
}

void IFE::SpriteManager::BackDraw()
{
	Sprite::DrawBefore();

	list<Sprite*>sprList;
	for (unique_ptr<Sprite>& itr : backgroundList_)
	{
		if (!itr->isActive_)continue;
		if (!itr->drawFlag_)continue;
		sprList.push_back(itr.get());
	}

	sprList.sort([](const Sprite* sprA, const Sprite* sprB) {return sprA->order_ > sprB->order_; });
	for (auto& itr : sprList)
	{
		itr->Draw();
	}
}

Sprite* IFE::SpriteManager::AddInitialize(const std::string& spriteName, const std::string& textureName)
{
	std::unique_ptr<Sprite> ptr = make_unique<Sprite>();
	ptr->SPRITEInitialize();
	ptr->spriteName_ = SetNameNumber(spriteName);
	ptr->SetTexture(textureName);
	foregroundList_.push_back(std::move(ptr));
	return foregroundList_.back().get();
}

Sprite* IFE::SpriteManager::AddInitializePushFront(const std::string& spriteName, const std::string& textureName)
{
	std::unique_ptr<Sprite> ptr = make_unique<Sprite>();
	ptr->SPRITEInitialize();
	ptr->spriteName_ = SetNameNumber(spriteName);
	ptr->SetTexture(textureName);
	foregroundList_.push_front(std::move(ptr));
	return foregroundList_.front().get();
}

Sprite* IFE::SpriteManager::Add(const std::string& spriteName)
{
	std::unique_ptr<Sprite> ptr = make_unique<Sprite>();
	ptr->spriteName_ = SetNameNumber(spriteName);
	foregroundList_.push_back(std::move(ptr));
	return foregroundList_.back().get();
}

Sprite* IFE::SpriteManager::AddBackGround(const std::string& spriteName)
{
	std::unique_ptr<Sprite> ptr = make_unique<Sprite>();
	ptr->spriteName_ = SetNameNumber(spriteName);
	backgroundList_.push_back(std::move(ptr));
	return backgroundList_.back().get();
}

Sprite* IFE::SpriteManager::GetSpritePtr(const std::string& spriteName)
{
	for (unique_ptr<Sprite>& itr : foregroundList_)
	{
		if (itr->spriteName_ == spriteName)
		{
			return itr.get();
		}
	}
	for (unique_ptr<Sprite>& itr : backgroundList_)
	{
		if (itr->spriteName_ == spriteName)
		{
			return itr.get();
		}
	}
	return nullptr;
}
//
//Sprite* IFE::SpriteManager::Instantiate(Sprite* gameObject, Float2 position, float rotation)
//{
//	std::unique_ptr<Sprite> ptr = make_unique<Sprite>();
//	*ptr = *gameObject;
//	string objectName = GetNewName(gameObject->spriteName);
//	ptr->spriteName = objectName;
//	foregroundList.push_back(std::move(ptr));
//	Sprite* obj = foregroundList.back().get();
//	obj->transform->position = position;
//	obj->transform->rotation = rotation;
//	return obj;
//}
//
//Sprite* IFE::SpriteManager::Instantiate(Sprite* gameObject, Float2 position)
//{
//	std::unique_ptr<Sprite> ptr = make_unique<Sprite>();
//	*ptr = *gameObject;
//	string objectName = GetNewName(gameObject->spriteName);
//	ptr->spriteName = objectName;
//	foregroundList.push_back(std::move(ptr));
//	Sprite* obj = foregroundList.back().get();
//	obj->transform->position = position;
//	return obj;
//}
//
//Sprite* IFE::SpriteManager::Instantiate(Sprite* gameObject)
//{
//	std::unique_ptr<Sprite> ptr = make_unique<Sprite>();
//	*ptr = *gameObject;
//	string objectName = GetNewName(gameObject->spriteName);
//	ptr->spriteName = objectName;
//	foregroundList.push_back(std::move(ptr));
//	Sprite* obj = foregroundList.back().get();
//	return obj;
//}

void IFE::SpriteManager::SetTexture(const std::string& spriteName, const std::string& texName)
{
	for (unique_ptr<Sprite>& itr : foregroundList_)
	{
		if (itr->spriteName_ == spriteName)
		{
			itr->SetTexture(texName);
			return;
		}
	}
	for (unique_ptr<Sprite>& itr : backgroundList_)
	{
		if (itr->spriteName_ == spriteName)
		{
			itr->SetTexture(texName);
			return;
		}
	}
}

std::list<std::unique_ptr<Sprite>>* IFE::SpriteManager::GetBackgroundList()
{
	return &backgroundList_;
}

std::list<std::unique_ptr<Sprite>>* IFE::SpriteManager::GetForegroundList()
{
	return &foregroundList_;
}

void IFE::SpriteManager::Reset()
{
	foregroundList_.clear();
	backgroundList_.clear();
}

bool IFE::SpriteManager::SearchName(const std::string& name)
{
	for (unique_ptr<Sprite>& itr : foregroundList_)
	{
		if (itr->spriteName_ == name)return true;
	}
	for (unique_ptr<Sprite>& itr : backgroundList_)
	{
		if (itr->spriteName_ == name)return true;
	}
	return false;
}

void IFE::SpriteManager::SpriteMoveElementFor(int32_t num1, int32_t num2)
{
	List::MoveElement<unique_ptr<Sprite>>(foregroundList_, num1, num2);
}

void IFE::SpriteManager::SpriteMoveElementBack(int32_t num1, int32_t num2)
{
	List::MoveElement<unique_ptr<Sprite>>(backgroundList_, num1, num2);
}

std::string IFE::SpriteManager::SetNameNumber(std::string spriteName)
{
	string objectN = std::regex_replace(spriteName, regex("\\d"), "");
	string name = objectN;
	uint32_t num = 0;
	while (SearchName(name))
	{
		num++;
		name = objectN + to_string(num);
	}
	return name;
}

#include "JsonManager.h"
#ifdef InverseEditorMode
#else
#include "ImguiManager.h"
#include "Imgui.h"
void IFE::SpriteManager::DebugGUI()
{
	static bool add = false;
	static bool fdelete = false;
	static bool back = false;
	ImGui::Begin("SpriteManager", (bool*)false, ImGuiWindowFlags_MenuBar);
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
		if (back)
		{
			if (ImGui::BeginMenu("fore ground list"))
			{
				ImGui::MenuItem("fore ground list", "", &back);
				ImGui::EndMenu();
			}
		}
		else
		{
			if (ImGui::BeginMenu("back ground list"))
			{
				ImGui::MenuItem("back ground list", "", &back);
				ImGui::EndMenu();
			}
		}
		ImGui::EndMenuBar();
	}
	if (add)
	{
		if (ImGui::CollapsingHeader("New Sprite"))
		{
			static char name[256];
			ImGui::InputText("New Sprite Name", name, sizeof(name));
			auto tex = TextureManager::Instance()->GetTextureGUI();
			if (ImGui::Button("Add Sprite"))
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
	std::string str;
	if (back)
	{
		for (unique_ptr<Sprite>& itr : backgroundList_)
		{
			std::string s;
			itr->DebugGUI(fdelete, &s, back);
			if (s != "")
			{
				str = s;
			}
		}
		ImGui::End();
		for (unique_ptr<Sprite>& itr : backgroundList_)
		{
			itr->ComponentGUI();
		}
		for (unique_ptr<Sprite>& itr : backgroundList_)
		{
			if (itr->spriteName_ == str)
			{
				foregroundList_.push_back(std::move(itr));
				backgroundList_.remove(itr);
				break;
			}
		}
	}
	else
	{
		for (unique_ptr<Sprite>& itr : foregroundList_)
		{
			std::string s;
			itr->DebugGUI(fdelete, &s, back);
			if (s != "")
			{
				str = s;
			}
		}
		ImGui::End();
		for (unique_ptr<Sprite>& itr : foregroundList_)
		{
			itr->ComponentGUI();
		}
		for (unique_ptr<Sprite>& itr : foregroundList_)
		{
			if (itr->spriteName_ == str)
			{
				foregroundList_.push_back(std::move(itr));
				backgroundList_.remove(itr);
				break;
			}
		}
	}
}

void IFE::SpriteManager::OutputScene()
{
	JsonManager* jm = JsonManager::Instance();
	nlohmann::json& j = jm->GetJsonData();
	uint32_t i = 0;
	for (unique_ptr<Sprite>& itr : backgroundList_)
	{
		itr->OutputScene(j["back"][i]);
		i++;
	}
	i = 0;
	for (unique_ptr<Sprite>& itr : foregroundList_)
	{
		if (itr->spriteName_.find("line") != std::string::npos)continue;
		itr->OutputScene(j["fore"][i]);
		i++;
	}
	jm->Output("SpriteManager");
}

void IFE::SpriteManager::DebugUpdate()
{
	foregroundList_.remove_if([](unique_ptr<Sprite>& obj) {return obj->GetDeleteFlag(); });
	backgroundList_.remove_if([](unique_ptr<Sprite>& obj) {return obj->GetDeleteFlag(); });
	for (unique_ptr<Sprite>& itr : foregroundList_)
	{
		itr->DebugUpdate();
	}
	for (unique_ptr<Sprite>& itr : backgroundList_)
	{
		itr->DebugUpdate();
	}
}
#endif
#include "ComponentHelp.h"

void IFE::SpriteManager::LoadingScene()
{
	JsonManager* jm = JsonManager::Instance();
	jm->Input("SpriteManager");
	nlohmann::json& js = jm->GetJsonData();
	for (auto& j : js["back"])
	{
		auto spr = AddBackGround(j["name"]);
		spr->Initialize(j["texture"]);
		spr->LoadingScene(j);
	}
	for (auto& j : js["fore"])
	{
		auto spr = Add(j["name"]);
		spr->Initialize(j["texture"]);
		spr->LoadingScene(j);
	}
}

std::string IFE::SpriteManager::GetNewName(const std::string& spriteName)
{
	int32_t i = 0;
	string r;
	while (true)
	{
		string s = spriteName + to_string(i);
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
