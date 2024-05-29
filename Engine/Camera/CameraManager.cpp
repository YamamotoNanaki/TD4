#include "CameraManager.h"
#include "Debug.h"

using namespace IFE;
Camera* CameraManager::sActivCamera_ = nullptr;

IFE::CameraManager::~CameraManager()
{
	cameraList_.clear();
}

CameraManager* IFE::CameraManager::Instance()
{
	static CameraManager inst;
	return &inst;
}

void IFE::CameraManager::AllCameraInitialize()
{
	for (auto& itr : cameraList_)
	{
		itr->Initialize();
	}
}

void IFE::CameraManager::Initialize()
{
	auto c = new Camera;
	c->cameraName_ = "initCamera";
	cameraList_.push_back(std::unique_ptr<Camera>(c));
	sActivCamera_ = cameraList_.front().get();
	sActivCamera_->CameraInitialize();
}

void IFE::CameraManager::Update()
{
	cameraList_.remove_if([](std::unique_ptr<Camera>& camera) {return camera->deleteFlag_; });
	if (sNextCamera_)
	{
		sActivCamera_ = sNextCamera_;
		sNextCamera_ = nullptr;
	}
	if (!sActivCamera_)
	{
		sActivCamera_ = cameraList_.front().get();
	}
	sActivCamera_->CameraUpdate();
}

void IFE::CameraManager::Draw()
{
	sActivCamera_->Draw();
}

void IFE::CameraManager::Reset()
{
	sActivCamera_ = nullptr;
	cameraList_.clear();
}

Camera* IFE::CameraManager::AddInitialize(const std::string& cameraName)
{
	std::unique_ptr<Camera> ptr = std::make_unique<Camera>();
	ptr->cameraName_ = cameraName;
	ptr->CameraInitialize();
	cameraList_.push_back(std::move(ptr));
	return cameraList_.back().get();
}

bool IFE::CameraManager::SearchName(const std::string& name)
{
	for (auto& itr : cameraList_)
	{
		if (itr->cameraName_ == name)return true;
	}
	return false;
}

Camera* IFE::CameraManager::GetCamera(const std::string& name)
{
	for (auto& itr : cameraList_)
	{
		if (itr->cameraName_ == name)
		{
			return itr.get();
		}
	}
	return nullptr;
}

void IFE::CameraManager::SetActiveCamera(const std::string& name)
{
	sNextCamera_ = GetCamera(name);
}

Camera* IFE::CameraManager::Add(const std::string& cameraName)
{
	std::unique_ptr<Camera> ptr = std::make_unique<Camera>();
	ptr->cameraName_ = cameraName;
	cameraList_.push_back(std::move(ptr));
	return cameraList_.back().get();
}

#ifdef InverseEditorMode
#else
#include "ImguiManager.h"
#include "ImGui.h"
void IFE::CameraManager::DebugInitialize()
{
	//auto d = new Camera;
	//d->cameraName_ = "debugCamera";
	//cameraList_.push_back(std::unique_ptr<Camera>(d));
	//sDebugCamera_ = cameraList_.front().get();
	//sDebugCamera_->CameraInitialize();
	//sDebugCamera_->DebugCameraInitialize();
}
void IFE::CameraManager::DebugUpdate()
{
	cameraList_.remove_if([](std::unique_ptr<Camera>& camera) {return camera->deleteFlag_; });
	if (!sActivCamera_)
	{
		sActivCamera_ = cameraList_.front().get();
	}
	sActivCamera_->DebugUpdate();
}
void IFE::CameraManager::DebugGUI()
{
	static bool add = false;
	static bool fdelete = false;
	auto im = ImguiManager::Instance();
	im->NewGUI("CameraManager", (int32_t)ImGuiWindowFlags_MenuBar);
	if (im->NewMenuBar())
	{
		if (im->NewMenu("add"))
		{
			im->MenuItem("Add", &add);
			im->EndMenu();
		}
		if (im->NewMenu("delete"))
		{
			im->MenuItem("delete", &fdelete);
			im->EndMenu();
		}
		im->EndMenuBar();
	}

	im->TextGUI("CameraManager");

	if (add)
	{
		if (im->CollapsingHeaderGUI("New Camera"))
		{
			static std::string name;
			im->InputTextGUI("New Camera Name", name);
			if (im->ButtonGUI("Add Camera"))
			{
				if (name == "")
				{
					im->TextGUI("error : no name has been set");
				}
				else if (SearchName(name))
				{
					im->TextGUI("error : an camera with the same name exists");
				}
				else
				{
					AddInitialize(name);
					im->TextGUI("success");
				}
			}
		}
	}

	for (auto& itr : cameraList_)
	{
		itr->DebugGUI(fdelete);
	}

	im->EndGUI();

	for (auto& itr : cameraList_)
	{
		itr->ComponentGUI();
	}
}
void IFE::CameraManager::OutputScene()
{
	JsonManager* jm = JsonManager::Instance();
	nlohmann::json& j = jm->GetJsonData();
	uint32_t i = 0;
	for (auto& itr : cameraList_)
	{
		if (itr->cameraName_ == "debugCamera")continue;
		itr->OutputScene(j[i]);
		i++;
	}
	jm->Output("CameraManager");
}
#endif

void IFE::CameraManager::LoadingScene()
{
	JsonManager* jm = JsonManager::Instance();
	jm->Input("CameraManager");
	nlohmann::json js = jm->GetJsonData();
#ifdef InverseEditorMode
#else
	DebugInitialize();
#endif
	for (auto& j : js)
	{
		Camera* camera = nullptr;
		camera = Add(j["name"]);
		camera->LoadingScene(j);
		camera->Initialize();
	}
	if (cameraList_.size() == 0)
	{
		Initialize();
	}
}
