#include "ImguiManager.h"
#ifdef InverseEditorMode
#else
#include "ObjectManager.h"

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"
#include "ImGuizmo.h"
#include <Windows.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include "GraphicsAPI.h"
#include "WindowsAPI.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "ComponentManager.h"
#include "Texture.h"
#include "ComponentHelp.h"
#include "Input.h"
#include "CameraManager.h"
#include "Transform.h"
#include <cuchar>

using namespace IFE;

std::string ImguiManager::sOpenComponentName_;

ImguiManager* IFE::ImguiManager::Instance()
{
	static ImguiManager inst;
	return &inst;
}

void IFE::ImguiManager::Initialize()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls


	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(*WindowsAPI::Instance()->GetHWnd());
	ID3D12DescriptorHeap* SrvDescHeap = TextureManager::Instance()->GetDescriptorHeap();
	ImGui_ImplDX12_Init(GraphicsAPI::Instance()->GetDevice(), sNUM_FRAMES_IN_FLIGHT_,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, SrvDescHeap,
		SrvDescHeap->GetCPUDescriptorHandleForHeapStart(),
		SrvDescHeap->GetGPUDescriptorHandleForHeapStart());
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontFromFileTTF("DebugData/Fonts/YuGothM.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
}

void IFE::ImguiManager::StartNewFrame()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
	ImGuizmo::Enable(true);
}

void IFE::ImguiManager::Update()
{
	//ペーストの処理
	if (copyObjects_.size() > 0 && (Input::GetKeyPush(Key::LCONTROL) || Input::GetKeyPush(Key::RCONTROL)) &&
		Input::GetKeyTrigger(Key::V))
	{
		auto om = ObjectManager::Instance();
		for (auto& itr : copyObjects_)
		{
			auto obj = om->Add(itr->GetObjectName());
			itr->CopyValue(obj);
		}
	}

	//オブジェクトのコピーの処理
	if (!openObj_)return;
	if ((Input::GetKeyPush(Key::LCONTROL) || Input::GetKeyPush(Key::RCONTROL)) &&
		(Input::GetKeyPush(Key::LSHIFT) || Input::GetKeyPush(Key::RSHIFT)) && Input::GetKeyPush(Key::C))
	{
		for (auto& itr : copyObjects_)
		{
			if (itr->GetObjectName() == openObj_->GetObjectName())
			{
				openObj_ = nullptr;
				return;
			}
		}
		copyObjects_.push_back(openObj_);
	}
	else if ((Input::GetKeyPush(Key::LCONTROL) || Input::GetKeyPush(Key::RCONTROL)) && Input::GetKeyPush(Key::C))
	{
		copyObjects_.clear();
		copyObjects_.push_back(openObj_);
	}
	openObj_ = nullptr;

	UpdateWindowSize();
}

void IFE::ImguiManager::UpdateWindowSize()
{
	if (WindowsAPI::Instance()->resize_)
	{
		RECT rect;
		GetClientRect(*WindowsAPI::Instance()->GetHWnd(), &rect);
		float width = static_cast<float>(rect.right - rect.left);
		float height = static_cast<float>(rect.bottom - rect.top);

		// ImGuiに新しいディスプレイサイズを設定
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(width, height);
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
	}
}

void IFE::ImguiManager::Draw()
{
	if (sOpenComponentName_ == "")
	{
		ImGui::Begin(U8("インスペクター"), (bool*)false, ImGuiWindowFlags_MenuBar);
		ImGui::End();
	}
	ImGui::Render();
	ID3D12GraphicsCommandList* commandList = GraphicsAPI::Instance()->GetCmdList();
	ID3D12DescriptorHeap* heaps[] = { TextureManager::Instance()->GetDescriptorHeap() };
	commandList->SetDescriptorHeaps(_countof(heaps), heaps);
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}

void IFE::ImguiManager::Demo()
{
	ImGui::ShowDemoWindow();
}

void IFE::ImguiManager::EndGUI()
{
	ImGui::End();
}

bool IFE::ImguiManager::ButtonGUI(const std::string& buttonName)
{
	return ImGui::Button(buttonName.c_str());
}

void IFE::ImguiManager::RadioButtonGUI(const std::string& buttonName, int32_t* num, int32_t buttonNum)
{
	ImGui::RadioButton(buttonName.c_str(), num, buttonNum);
}

void IFE::ImguiManager::ObjectManagerGUI(bool* add, bool* fdelete, bool* prefab, bool* fmove)
{
	ImGui::Begin(U8("ヒエラルキー(object)"), (bool*)false, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu(U8("追加")))
		{
			ImGui::MenuItem(U8("追加"), "", add);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("delete"))
		{
			ImGui::MenuItem("delete", "", fdelete);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("move"))
		{
			ImGui::MenuItem("Prefab and Object Switching", "", fmove);
			ImGui::EndMenu();
		}
		if (*prefab)
		{
			if (ImGui::BeginMenu("ObjectManager"))
			{
				ImGui::MenuItem("ObjectManager", "", prefab);
				ImGui::EndMenu();
			}
		}
		else
		{
			if (ImGui::BeginMenu("PrefabManager"))
			{
				ImGui::MenuItem("PrefabManager", "", prefab);
				ImGui::EndMenu();
			}
		}
		ImGui::EndMenuBar();
	}
}

void IFE::ImguiManager::ComponentGUI(const std::string& objectName, const std::function<void(void)>& ComponentFunc, const std::function<void(std::unique_ptr<Component>)>& addFunc, const  std::function<void(Component*)>& modelFunc)
{
	static bool cm = false;
	if (objectName == sOpenComponentName_)
	{
		ImGui::Begin(U8("インスペクター"), (bool*)false, ImGuiWindowFlags_MenuBar);
		openObj_ = ObjectManager::Instance()->GetObjectPtr(objectName);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu(U8("追加")))
			{
				ImGui::MenuItem(U8("追加"), "", &componentAddFlag_);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("delete"))
			{
				ImGui::MenuItem("Delete", "", &componentDeleteFlag_);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("change model"))
			{
				ImGui::MenuItem("Change Model", "", &cm);
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		if (componentAddFlag_)
		{
			if (ImGui::CollapsingHeader(U8("追加")))
			{
				static char name[256];
				ImGui::InputText("add Component", name, sizeof(name));
				if (ImGui::Button("Add"))
				{
					auto tmp = std::unique_ptr<Component>(std::move(StringToComponent(name)));
					if (tmp != nullptr)
					{
						addFunc(std::move(tmp));
					}
				}
			}
		}
		if (cm)
		{
			if (ImGui::CollapsingHeader("Change Model"))
			{
				auto name = ModelManager::Instance()->GetModelNameGUI();
				if (ImGui::Button("Change"))
				{
					auto m = ModelManager::Instance()->GetModel(name);
					if (m)
					{
						modelFunc(m);
					}
				}
			}
		}
		static int32_t guizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
		static int32_t guizmoMode = ImGuizmo::MODE::WORLD;
		static bool guizmoUseSnap = false;
		static float snapNum[3] = { 1,1,1 };
		if (ImGui::CollapsingHeader("Guizmo Settings"))
		{
			ImGui::RadioButton("translate", &guizmoOperation, ImGuizmo::OPERATION::TRANSLATE);
			ImGui::SameLine();
			ImGui::RadioButton("scale", &guizmoOperation, ImGuizmo::OPERATION::SCALE);
			ImGui::RadioButton("world", &guizmoMode, ImGuizmo::MODE::WORLD);
			ImGui::SameLine();
			ImGui::RadioButton("local", &guizmoMode, ImGuizmo::MODE::LOCAL);
			ImGui::Checkbox("use snap", &guizmoUseSnap);
			if (guizmoUseSnap)ImGui::InputFloat3("snap num", snapNum);
		}
		ComponentFunc();
		ImGui::End();

		auto cameraPtr = CameraManager::Instance()->sActivCamera_;
		if (!cameraPtr)return;
		float v[16];
		float p[16];
		float w[16];
		float oldw[16];

		GetArrayMatrix(cameraPtr->GetView()->Get(), v);
		GetArrayMatrix(cameraPtr->GetProjection()->Get(), p);
		GetArrayMatrix(openObj_->transform_->matWorld_, w);

		for (size_t i = 0; i < 16; i++)
		{
			oldw[i] = w[i];
		}

		ImGuizmo::SetRect(0, 0, (float)WindowsAPI::Instance()->winWidth_, (float)WindowsAPI::Instance()->winHeight_);
		ImGuizmo::Manipulate(v, p, (ImGuizmo::OPERATION)guizmoOperation, (ImGuizmo::MODE)guizmoMode, w, nullptr, guizmoUseSnap ? snapNum : nullptr);

		SetArrayMatrix(openObj_->transform_->matWorld_, w);

		openObj_->transform_->position_ = { w[12],w[13],w[14] };
		openObj_->transform_->scale_ = GetScale(openObj_->transform_->matWorld_);
		//openObj_->transform_->position_ = { w[12],w[13],w[14] };
	}
}

void IFE::ImguiManager::ComponentGUI2D(const std::string& objectName, const std::function<void(void)>& ComponentFunc, const std::function<void(std::unique_ptr<Component>)>& addFunc/*, const std::function<void(std::string)>& texFunc*/)
{
	if (objectName == sOpenComponentName_)
	{
		ImGui::Begin(U8("インスペクター"), (bool*)false, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu(U8("追加")))
			{
				ImGui::MenuItem(U8("追加"), "", &componentAddFlag_);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("delete"))
			{
				ImGui::MenuItem("Delete", "", &componentDeleteFlag_);
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		if (componentAddFlag_)
		{
			if (ImGui::CollapsingHeader(U8("追加")))
			{
				static char name[256];
				ImGui::InputText("add Component", name, sizeof(name));
				if (ImGui::Button("Add"))
				{
					auto tmp = std::unique_ptr<Component>(std::move(StringToComponent(name)));
					if (tmp != nullptr)
					{
						addFunc(std::move(tmp));
					}
				}
			}
		}
		ImGui::Text(objectName.c_str());
		ComponentFunc();
		ImGui::End();
	}
}

bool IFE::ImguiManager::ObjectGUI(const std::string& name, const bool& flagdelete, const  bool& m, bool* moveFlag, Object3D* obj)
{
	if (ImGui::TreeNode(name.c_str()))
	{
		if (ImGui::Button("Open Component"))
		{
			sOpenComponentName_ = name;
		}
		if (flagdelete)
		{
			if (ImGui::Button("Object Delete"))
			{
				ImGui::TreePop();
				return true;
			}
		}
		if (m)
		{
			if (ImGui::Button("Prefab and Object Switching"))
			{
				*moveFlag = true;
			}
		}
		if (obj->child_.size() != 0)
		{
			if (ImGui::TreeNode("child"))
			{
				for (int32_t i = 0; i < obj->childName_.size(); i++)
				{
					ImGui::Text(obj->childName_[i].c_str());
					std::string s = "delete " + obj->childName_[i];
					if (ImGui::Button(s.c_str()))
					{
						obj->DeleteChild(i);
					}
				}
				ImGui::TreePop();
			}
		}
		if (obj->parent_ != nullptr)
		{
			if (ImGui::TreeNode("parent"))
			{
				ImGui::Text(obj->parent_->GetObjectName().c_str());
				ImGui::TreePop();
			}
		}
		if (ImGui::TreeNode("Set Child"))
		{
			static char n[256];
			ImGui::InputText("child Name", n, sizeof(n));
			if (ImGui::Button("Set"))
			{
				Object3D* ptr = ObjectManager::Instance()->GetObjectPtr(n);
				if (ptr != nullptr)
				{
					obj->child_.push_back(ptr);
					obj->childName_.push_back(ptr->GetObjectName());
					ptr->parent_ = obj;
					ptr->parentName_ = obj->GetObjectName();
				}
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	return false;
}

bool IFE::ImguiManager::SpriteGUI(const std::string& name, const bool& flagdelete, const bool& m, bool* moveFlag)
{
	if (ImGui::TreeNode(name.c_str()))
	{
		if (ImGui::Button("Open Component"))
		{
			sOpenComponentName_ = name;
		}
		if (flagdelete)
		{
			if (ImGui::Button("Object Delete"))
			{
				ImGui::TreePop();
				return true;
			}
		}
		ImGui::TreePop();
	}
	if (m)
	{
		if (ImGui::Button("Prefab and Object Switching"))
		{
			*moveFlag = true;
		}
	}
	return false;
}

bool IFE::ImguiManager::EmitterGUI(const std::string& name, const bool& flagdelete)
{
	if (ImGui::TreeNode(name.c_str()))
	{
		if (ImGui::Button("Open Component"))
		{
			sOpenComponentName_ = name;
		}
		if (flagdelete)
		{
			if (ImGui::Button("Object Delete"))
			{
				ImGui::TreePop();
				return true;
			}
		}
		ImGui::TreePop();
	}
	return false;
}

bool IFE::ImguiManager::ObjectAddGUI(std::string* newObjectName, std::string* modelName, const std::function<std::string(void)>& guiFunc)
{
	if (ImGui::CollapsingHeader("New Object"))
	{
		static char name[256];
		ImGui::InputText("New Object Name", name, sizeof(name));
		*newObjectName = name;
		*modelName = guiFunc();
		return ImGui::Button("Add Object");
	}
	return false;
}

void IFE::ImguiManager::NewGUI(const std::string& guiName, int32_t flag)
{
	ImGui::Begin(guiName.c_str(), (bool*)false, (ImGuiWindowFlags)flag);
}

void IFE::ImguiManager::ModelManagerGUI(bool* add, bool* fdelete)
{
	ImGui::Begin("ModelManager", (bool*)false, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu(U8("追加")))
		{
			ImGui::MenuItem(U8("追加"), "", add);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("delete"))
		{
			ImGui::MenuItem("delete", "", fdelete);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

bool IFE::ImguiManager::ModelAddGUI(std::string* newObjectName, std::string* newFilename, uint16_t* settings, bool* smooth)
{
	if (ImGui::CollapsingHeader("New Model"))
	{
		static char name[256];
		ImGui::InputText("New Model Name", name, sizeof(name));
		*newObjectName = name;

		ImGui::Checkbox("smooth", smooth);

		int32_t num = (int32_t)*settings;
		ImGui::RadioButton("Load .obj", &num, (int32_t)AddModelSettings::LoadOBJ);
		ImGui::RadioButton("Load .gltf", &num, (int32_t)AddModelSettings::LoadGLTF);
		ImGui::RadioButton("Primitiv Cube", &num, (int32_t)AddModelSettings::CreateCube);
		ImGui::RadioButton("Primitiv Square", &num, (int32_t)AddModelSettings::CreateSquare);
		ImGui::RadioButton("Primitiv Triangle", &num, (int32_t)AddModelSettings::CreateTriangle);
		ImGui::RadioButton("Primitiv Circle", &num, (int32_t)AddModelSettings::CreateCircle);
		ImGui::RadioButton("Primitiv Sphere", &num, (int32_t)AddModelSettings::CreateSphere);

		*settings = (uint16_t)num;

		if (num < (int32_t)AddModelSettings::CreateCube)
		{
			static char filename[256];
			ImGui::InputText("File Name", filename, sizeof(filename));
			*newFilename = filename;
		}
		return ImGui::Button("Add Model");
	}
	return false;
}

int32_t IFE::ImguiManager::GetModelNameGUI(const std::vector<std::string>& names)
{
	static int32_t returnnum = 0;
	Combo("models", returnnum, names);
	return returnnum;
}

//void IFE::ImguiManager::ModelGUI(const std::string& modelName)
//{
//	if (ImGui::CollapsingHeader(modelName.c_str()))
//	{
//	}
//}

void IFE::ImguiManager::ComponentGUI(const std::string& name, const std::function<void(void)>& guiFunc, const std::function<void(void)>& deleteFunc)
{
	if (ImGui::CollapsingHeader(name.c_str()))
	{
		if (componentDeleteFlag_)
		{
			if (ImGui::Button("Remove"))
			{
				deleteFunc();
			}
		}
		guiFunc();
	}
}

void IFE::ImguiManager::ChangeTextureGUI(const std::function<void(std::string)>& guiFunc)
{
	if (ImGui::CollapsingHeader("change texture"))
	{
		static char texname[256];
		ImGui::InputText("texture name", texname, sizeof(texname));
		if (ImGui::Button("change"))
		{
			std::string s = texname;
			guiFunc(s);
		}
	}
}

void IFE::ImguiManager::TextureGUI(bool* load, bool* search)
{
	ImGui::Begin("TextureManager", (bool*)false, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("load"))
		{
			ImGui::MenuItem("load", "", load);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("search"))
		{
			ImGui::MenuItem("search", "", search);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void IFE::ImguiManager::TextureLoadGUI(const std::function<void(std::string)>& guiFunc)
{
	if (ImGui::CollapsingHeader("Texture Load"))
	{
		static char filename[256];
		ImGui::InputText("filename", filename, sizeof(filename));
		if (ImGui::Button("Load"))
		{
			std::string f = filename;
			guiFunc(f);
		}
	}
}

void IFE::ImguiManager::ShowTextureGUI(std::array<Texture, 1024>& tex)
{
	if (ImGui::CollapsingHeader("All Textures"))
	{
		Texture* t[5];
		int32_t j = 0;
		for (int32_t i = 1; i < 1024; i++)
		{
			if (!tex[i].free_)continue;
			t[j] = tex[i].GetPtr();
			j++;
			if (j % 5 == 0 && j != 0)
			{
				for (int32_t k = 0; k < 5; k++)
				{
					std::string s = std::to_string(k) + ". " + t[k]->texName_;
					ImGui::Text(s.c_str());
					if (k != 4)ImGui::SameLine();
				}
				for (int32_t k = 0; k < 5; k++)
				{
					ImGui::Image((ImTextureID)t[k]->GPUHandle_.ptr, { 96,96 });
					if (k != 4)ImGui::SameLine();
				}
				j = 0;
			}
		}

		for (int32_t k = 0; k < j; k++)
		{
			std::string s = std::to_string(k) + ". " + t[k]->texName_;
			ImGui::Text(s.c_str());
			if (k != j - 1)ImGui::SameLine();
		}
		for (int32_t k = 0; k < j; k++)
		{
			ImGui::Image((ImTextureID)t[k]->GPUHandle_.ptr, { 96,96 });
			if (k != j - 1)ImGui::SameLine();
		}
	}
}

void IFE::ImguiManager::SearchTextureGUI(const std::array<Texture, 1024>& texture)
{
	if (ImGui::CollapsingHeader("search"))
	{
		static bool flag = false;
		static std::string SearchName;
		static char s[256];
		ImGui::InputText("search texture name", s, sizeof(s));
		if (ImGui::Button("search texture"))
		{
			if (!flag)flag = true;
			SearchName = s;
		}
		if (flag)
		{
			for (int32_t i = 1; i < 1024; i++)
			{
				if (!texture[i].free_)continue;
				if (texture[i].texName_ == SearchName)
				{
					ImGui::Image((ImTextureID)texture[i].GPUHandle_.ptr, { 96,96 });
					break;
				}
			}
		}
	}
}

void IFE::ImguiManager::TextGUI(const std::string& text)
{
	ImGui::Text(text.c_str());
}

void IFE::ImguiManager::TextFloatGUI(const std::string& text, float number)
{
	std::string t = text + " : " + std::to_string(number);
	ImGui::Text(t.c_str());
}

void IFE::ImguiManager::TextFloat2GUI(const std::string& text, const Float2& number)
{
	if (Instance()->NewTreeNode(text))
	{
		Instance()->TextFloatGUI("x",number.x);
		Instance()->TextFloatGUI("y",number.y);
		Instance()->EndTreeNode();
	}
}

void IFE::ImguiManager::TextFloat3GUI(const std::string& text, const Float3& number)
{
	if (Instance()->NewTreeNode(text))
	{
		Instance()->TextFloatGUI("x", number.x);
		Instance()->TextFloatGUI("y", number.y);
		Instance()->TextFloatGUI("z", number.z);
		Instance()->EndTreeNode();
	}
}

void IFE::ImguiManager::TextFloat4GUI(const std::string& text, const Float4& number)
{
	if (Instance()->NewTreeNode(text))
	{
		Instance()->TextFloatGUI("x", number.x);
		Instance()->TextFloatGUI("y", number.y);
		Instance()->TextFloatGUI("z", number.z);
		Instance()->TextFloatGUI("w", number.w);
		Instance()->EndTreeNode();
	}
}

void IFE::ImguiManager::TextIntGUI(const std::string& text, int number)
{
	std::string t = text + " : " + std::to_string(number);
	ImGui::Text(t.c_str());
}

void IFE::ImguiManager::TextFloatGUI(float number)
{
	TextFloatGUI("float", number);
}

void IFE::ImguiManager::TextFloat2GUI(const Float2& number)
{
	TextFloat2GUI("float2", number);
}

void IFE::ImguiManager::TextFloat3GUI(const Float3& number)
{
	TextFloat3GUI("float3", number);
}

void IFE::ImguiManager::TextFloat4GUI(const Float4& number)
{
	TextFloat4GUI("float4", number);
}

void IFE::ImguiManager::TextIntGUI(int32_t number)
{
	TextIntGUI("int", number);
}

void IFE::ImguiManager::InputTextGUI(const std::string& label, std::string& text)
{
	static char t[256];
	ImGui::InputText(label.c_str(), t, sizeof(t));
	text = t;
}
void IFE::ImguiManager::DragIntGUI(int32_t* i, const std::string& label, float speed, int32_t min, int32_t max)
{
	ImGui::DragInt(label.c_str(), i, speed, min, max);
}

void IFE::ImguiManager::DragFloatGUI(float* f, const std::string& label, float speed, float min, float max)
{
	ImGui::DragFloat(label.c_str(), f, speed, min, max);
}

void IFE::ImguiManager::DragFloat2GUI(Float2* f, const std::string& label, float speed, float min, float max)
{
	float a[2] = { f->x,f->y };
	ImGui::DragFloat2(label.c_str(), a, speed, min, max);
	f->Set(a[0], a[1]);
}

void IFE::ImguiManager::DragInt2GUI(Int2* i, const std::string& label, float speed, int32_t min, int32_t max)
{
	int32_t a[2] = { i->x,i->y };
	ImGui::DragInt2(label.c_str(), a, speed, min, max);
	i->x = a[0];
	i->y = a[1];
}

void IFE::ImguiManager::DragFloat3GUI(Float3* f, const std::string& label, float speed, float min, float max)
{
	float a[3] = { f->x,f->y,f->z };
	ImGui::DragFloat3(label.c_str(), a, speed, min, max);
	f->Set(a[0], a[1], a[2]);
}

void IFE::ImguiManager::DragFloat4GUI(Float4* f, const std::string& label, float speed, float min, float max)
{
	float a[4] = { f->x,f->y,f->z,f->w };
	ImGui::DragFloat4(label.c_str(), a, speed, min, max);
	f->Set(a[0], a[1], a[2], a[3]);
}

void IFE::ImguiManager::ColorEdit4GUI(Float4* color, const std::string& label)
{
	float c[4] = { color->x,color->y,color->z,color->w };
	ImGui::ColorEdit4(label.c_str(), c);
	color->Set(c[0], c[1], c[2], c[3]);
}

void IFE::ImguiManager::ColorEdit3GUI(Float3* color, const std::string& label)
{
	float c[3] = { color->x,color->y,color->z };
	ImGui::ColorEdit3(label.c_str(), c);
	color->Set(c[0], c[1], c[2]);
}

void IFE::ImguiManager::CheckBoxGUI(bool* flag, const std::string& label)
{
	ImGui::Checkbox(label.c_str(), flag);
}

void IFE::ImguiManager::CollapsingHeaderGUI(const std::string& label, const std::function<void(void)>& guiFunc)
{
	if (ImGui::CollapsingHeader(label.c_str()))
	{
		guiFunc();
	}
}

bool IFE::ImguiManager::NewTreeNode(const std::string& label)
{
	return ImGui::TreeNode(label.c_str());
}

void IFE::ImguiManager::EndTreeNode()
{
	ImGui::TreePop();
}

Object3D* IFE::ImguiManager::SetObject(Object3D* obj, const std::string& label)
{
	static bool error = false;
	if (ImGui::TreeNode(label.c_str()))
	{
		if (obj != nullptr)ImGui::Text(obj->GetObjectName().c_str());
		else ImGui::Text("nullptr");
		if (ImGui::TreeNode("Change Ptr"))
		{
			static char name[256];
			ImGui::InputText("Object Name", name, sizeof(name));
			if (error)ImGui::Text("error : return nullptr");

			if (ImGui::Button("Change"))
			{
				Object3D* ptr = ObjectManager::Instance()->GetObjectPtr(name);
				if (ptr != nullptr)
				{
					error = false;
					obj = ptr;
				}
				else error = true;
			}
			if (ImGui::Button("Change nullptr"))
			{
				obj = nullptr;
			}

			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	return obj;
}

Component* IFE::ImguiManager::GetModelGUI()
{
	ModelManager* mm = ModelManager::Instance();
	std::vector<std::string>strs = mm->GetAllModelName();
	static int32_t num = 0;

	for (size_t i = 0; i < strs.size(); i++)
	{
		ImGui::RadioButton(strs[i].c_str(), &num, (int32_t)i);
	}

	if (ImGui::Button("Select"))
	{
		return mm->GetModel(strs[num]);
	}
	return nullptr;
}


void IFE::ImguiManager::DragVectorFloatGUI(std::vector<float>& f, const std::string& label, float speed, float min, float max)
{
	if (ImGui::TreeNode(label.c_str()))
	{

		for (uint32_t i = 0; i < f.size(); i++)
		{
			std::string num = "[" + std::to_string(i) + "]";
			ImGui::DragFloat(num.c_str(), &f[i], speed, min, max);
			ImGui::SameLine();
			if (ImGui::Button("↑"))
			{
				if (i != 0)
				{
					float n = f[i];
					f[i] = f[i - 1];
					f[i - 1] = n;
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("↓"))
			{
				if (i != f.size() - 1)
				{
					float n = f[i];
					f[i] = f[i + 1];
					f[i + 1] = n;
				}

			}
			ImGui::SameLine();
			if (ImGui::Button("×"))
			{
				f.erase(f.begin() + i);
			}
		}

		if (ImGui::Button(U8("追加")))
		{
			f.push_back(0);
		}

		ImGui::TreePop();
	}
}

void IFE::ImguiManager::DragVectorFloat2GUI(std::vector<Float2>& f, const std::string& label, float speed, float min, float max)
{
	if (ImGui::TreeNode(label.c_str()))
	{

		for (uint32_t i = 0; i < f.size(); i++)
		{
			std::string num = "[" + std::to_string(i) + "]";
			DragFloat2GUI(&f[i], num, speed, min, max);
			ImGui::SameLine();
			if (ImGui::Button("↑"))
			{
				if (i != 0)
				{
					Float2 n = f[i];
					f[i] = f[i - 1];
					f[i - 1] = n;
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("↓"))
			{
				if (i != f.size() - 1)
				{
					Float2 n = f[i];
					f[i] = f[i + 1];
					f[i + 1] = n;
				}

			}
			ImGui::SameLine();
			if (ImGui::Button("×"))
			{
				f.erase(f.begin() + i);
			}
		}

		if (ImGui::Button(U8("追加")))
		{
			f.push_back(0);
		}

		ImGui::TreePop();
	}
}

void IFE::ImguiManager::DragVectorFloat3GUI(std::vector<Float3>& f, const std::string& label, Float3 addValue, float speed, float min, float max)
{
	if (ImGui::TreeNode(label.c_str()))
	{

		for (uint32_t i = 0; i < f.size(); i++)
		{
			std::string num = "[" + std::to_string(i) + "]";
			DragFloat3GUI(&f[i], num, speed, min, max);
			ImGui::SameLine();
			std::string text = num + "UP";
			if (ImGui::Button(text.c_str()))
			{
				if (i != 0)
				{
					Float3 n = f[i];
					f[i] = f[i - 1];
					f[i - 1] = n;
				}
			}
			ImGui::SameLine();
			text = num + "DOWN";
			if (ImGui::Button(text.c_str()))
			{
				if (i != f.size() - 1)
				{
					Float3 n = f[i];
					f[i] = f[i + 1];
					f[i + 1] = n;
				}

			}
			ImGui::SameLine();
			text = num + "DELETE";
			if (ImGui::Button(text.c_str()))
			{
				f.erase(f.begin() + i);
			}
		}

		if (ImGui::Button(U8("追加")))
		{
			f.push_back(addValue);
		}

		ImGui::TreePop();
	}
}

void IFE::ImguiManager::DragVectorFloat4GUI(std::vector<Float4>& f, const std::string& label, float speed, float min, float max)
{
	if (ImGui::TreeNode(label.c_str()))
	{

		for (uint32_t i = 0; i < f.size(); i++)
		{
			std::string num = "[" + std::to_string(i) + "]";
			DragFloat4GUI(&f[i], num, speed, min, max);
			ImGui::SameLine();
			if (ImGui::Button("↑"))
			{
				if (i != 0)
				{
					Float4 n = f[i];
					f[i] = f[i - 1];
					f[i - 1] = n;
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("↓"))
			{
				if (i != f.size() - 1)
				{
					Float4 n = f[i];
					f[i] = f[i + 1];
					f[i + 1] = n;
				}

			}
			ImGui::SameLine();
			if (ImGui::Button("×"))
			{
				f.erase(f.begin() + i);
			}
		}

		if (ImGui::Button(U8("追加")))
		{
			f.push_back(0);
		}

		ImGui::TreePop();
	}
}

bool IFE::ImguiManager::NewMenuBar()
{
	return ImGui::BeginMenuBar();
}

void IFE::ImguiManager::EndMenuBar()
{
	ImGui::EndMenuBar();
}

bool IFE::ImguiManager::NewMenu(const std::string& menuName)
{
	return ImGui::BeginMenu(menuName.c_str());
}

void IFE::ImguiManager::EndMenu()
{
	ImGui::EndMenu();
}

void IFE::ImguiManager::MenuItem(const std::string& itemName, bool* flag)
{
	ImGui::MenuItem(itemName.c_str(), "", flag);
}

bool IFE::ImguiManager::CollapsingHeaderGUI(const std::string& headerName)
{
	return ImGui::CollapsingHeader(headerName.c_str());
}

bool IFE::ImguiManager::Combo(const std::string& comboName, int32_t& number, std::vector<std::string> items)
{
	std::vector<const char*> tmp;

	for (const auto& item : items)
	{
		tmp.push_back(item.c_str());
	}

	return ImGui::Combo(comboName.c_str(), &number, tmp.data(), int32_t(items.size()));
}



#endif