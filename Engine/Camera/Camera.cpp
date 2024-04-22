#include "Camera.h"
#include "Debug.h"
#include "WindowsAPI.h"
#include "ComponentHelp.h"
#include "Transform.h"
#include "Fog.h"
#include "DebugCamera.h"

using namespace IFE;
using namespace std;

Camera::Camera()
{
	SetCameraPtr(this);
}

void IFE::Camera::CameraInitialize()
{
	AddComponent<TransformCamera>();
	AddComponent<Fog>();
	ComponentManager::Initialize();
	transform_ = GetComponent<TransformCamera>();
}

void IFE::Camera::CameraUpdate()
{
	projection_.Update();
	ComponentManager::Update();
}

View* IFE::Camera::GetView()
{
	return &view_;
}

Projection* IFE::Camera::GetProjection()
{
	return &projection_;
}

IFE::Camera::~Camera()
{
}

void IFE::Camera::Initialize()
{
	ComponentManager::Initialize();
	transform_ = GetComponent<TransformCamera>();
}

void IFE::Camera::SetComponent(std::unique_ptr<Component> component)
{
	AddComponentBack<Component>(std::move(component));
}
void IFE::Camera::SetComponentFront(std::unique_ptr<Component> component)
{
	AddComponent<Component>(std::move(component));
}

#ifdef InverseEditorMode
#else
#include "ImguiManager.h"
#include "ImGui.h"
void IFE::Camera::DebugGUI(bool fdelete)
{
	auto imgui = ImguiManager::Instance();

	if (imgui->NewTreeNode(cameraName_))
	{
		if (imgui->ButtonGUI("Open Component"))
		{
			imgui->sOpenComponentName_ = cameraName_;
		}
		if (fdelete)
		{
			if (imgui->ButtonGUI("Camera Delete"))
			{
				imgui->EndTreeNode();
				deleteFlag_ = true;
				return;
			}
		}
		imgui->EndTreeNode();
	}
}

void IFE::Camera::DebugUpdate()
{
	projection_.Update();
	transform_->Update();
	GetComponent<Fog>()->Update();
}

void IFE::Camera::DebugCameraInitialize()
{
	AddComponent<DebugCamera>();
	ComponentManager::Initialize();
}

void IFE::Camera::ComponentGUI()
{
	auto imgui = ImguiManager::Instance();

	if (cameraName_ == imgui->sOpenComponentName_)
	{
		imgui->NewGUI("Component List", (int32_t)ImGuiWindowFlags_MenuBar);
		if (imgui->NewMenuBar())
		{
			if (imgui->NewMenu("add"))
			{
				imgui->MenuItem("Add", &imgui->componentAddFlag_);
				imgui->EndMenu();
			}
			if (imgui->NewMenu("delete"))
			{
				imgui->MenuItem("Delete", &imgui->componentDeleteFlag_);
				imgui->EndMenu();
			}
			ImGui::EndMenuBar();
		}
		if (imgui->componentAddFlag_)
		{
			if (imgui->CollapsingHeaderGUI("add"))
			{
				std::string name;
				imgui->InputTextGUI("add Component", name);
				if (imgui->ButtonGUI("Add"))
				{
					auto tmp = std::unique_ptr<Component>(std::move(StringToComponent(name)));
					if (tmp != nullptr)
					{
						SetComponent(std::move(tmp));
					}
				}
			}
		}
		ComponentManager::DebugGUI();
		ImGui::End();
	}
}

void IFE::Camera::OutputScene(nlohmann::json& json)
{
	json["name"] = cameraName_;
	uint32_t i = 0;
	for (auto& com : componentList_)
	{
		json["component"][i] = com->componentName_;
		i++;
	}
	ComponentManager::OutputScene(json);
}
#endif

void IFE::Camera::LoadingScene(nlohmann::json& json)
{
	for (auto& com : json["component"])
	{
		ComponentManager::LoadingScene(json, com);
	}
}
