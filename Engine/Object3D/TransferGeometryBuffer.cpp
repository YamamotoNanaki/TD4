#include "TransferGeometryBuffer.h"
#include "ImguiManager.h"
#include "JsonManager.h"
#include "Material.h"

using namespace IFE;
using namespace std;

void IFE::TransferGeometryBuffer::Initialize()
{
	geometryBuffer_ = make_unique<ConstBuffer<ConstBufferModelGeometry>>();
	constMapGeometry_ = geometryBuffer_->GetCBMapObject();
}

void IFE::TransferGeometryBuffer::Update()
{
	constMapGeometry_->explosion = explosion_;
	constMapGeometry_->gravity = gravity_;
	constMapGeometry_->rotation = rotation_;
	constMapGeometry_->polygonSize = polygonSize_;
	constMapGeometry_->lightFlag = lightFlag_;
	constMapGeometry_->toonFlag = toonFlag_;
	constMapGeometry_->bllomFlag = bllomFlag_;
}

void IFE::TransferGeometryBuffer::Draw()
{
	geometryBuffer_->SetConstBuffView(1);
}

void IFE::TransferGeometryBuffer::Copy(Component* component)
{
	TransferGeometryBuffer* t = dynamic_cast<TransferGeometryBuffer*>(component);
	if (t == nullptr)return;
	explosion_ = t->explosion_;
	gravity_ = t->gravity_;
	rotation_ = t->rotation_;
	polygonSize_ = t->polygonSize_;
	lightFlag_ = t->lightFlag_;
	toonFlag_ = t->toonFlag_;
	bllomFlag_ = t->bllomFlag_;
}

#ifdef InverseEditorMode
#else

void IFE::TransferGeometryBuffer::ComponentDebugGUI()
{
	ImguiManager* im = ImguiManager::Instance();
	std::function<void(void)> guiFunc = [&]()
	{
		im->DragFloatGUI(&explosion_, "explosion");
		im->DragFloatGUI(&gravity_, "gravity");
		im->DragFloatGUI(&rotation_, "polygon rotation");
		im->DragFloatGUI(&polygonSize_, "polygon size");
		im->CheckBoxGUI(&lightFlag_, "lightFlag");
		im->CheckBoxGUI(&toonFlag_, "toonFlag");
	};
	std::function<void(void)> deleteFunc = [&]()
	{
		componentDeleteFlag_ = true;
	};
	im->ComponentGUI(guiFunc, deleteFunc);
}

void IFE::TransferGeometryBuffer::OutputComponent(nlohmann::json& json)
{
	json["explosion"] = explosion_;
	json["gravity"] = gravity_;
	json["rotation"] = rotation_;
	json["polygonSize"] = polygonSize_;
	json["lightFlag"] = lightFlag_;
	json["toonFlag"] = toonFlag_;
}

#endif

void IFE::TransferGeometryBuffer::LoadingComponent(nlohmann::json& json)
{
	explosion_ = json["explosion"];
	gravity_ = json["gravity"];
	rotation_ = json["rotation"];
	polygonSize_ = json["polygonSize"];
	lightFlag_ = json["lightFlag"];
	toonFlag_ = json["toonFlag"];
	geometryBuffer_ = make_unique<ConstBuffer<ConstBufferModelGeometry>>();
	constMapGeometry_ = geometryBuffer_->GetCBMapObject();
}
