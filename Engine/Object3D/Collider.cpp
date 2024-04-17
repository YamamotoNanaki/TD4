#include "Collider.h"
#include "Object3D.h"
#include "ImGuiManager.h"
#include "ComponentManager.h"
#include "Component.h"
#include "JsonManager.h"

void IFE::Collider::Update()
{
	ADXCollider::Update(objectPtr);
}

void IFE::Collider::Initialize()
{
	ADXCollider::Initialize(objectPtr);
}

void IFE::Collider::Copy(Component* c)
{
	Collider* col = dynamic_cast<Collider*>(c);
	if (col == nullptr)return;
	enabled = col->enabled;
	isTrigger = col->isTrigger;
	pushable_ = col->pushable_;
	radius_ = col->radius_;
	pos_ = col->pos_;
	scale_ = col->scale_;
	pushBackPriority = col->pushBackPriority;
	collideLayer = col->collideLayer;
	colType_ = col->colType_;
}

#ifdef _DEBUG
#include "ImguiManager.h"
void IFE::Collider::DebugGUI()
{
	ImguiManager* gui = ImguiManager::Instance();

	std::function<void(void)>f = [&]()
	{
		int coltype = (int)colType_;
		gui->CheckBoxGUI(&enabled, "enabled");
		gui->CheckBoxGUI(&isTrigger, "isTrigger");
		gui->CheckBoxGUI(&pushable_, "pushable");
		gui->RadioButtonGUI("coltype Box", &coltype, (int)box);
		gui->RadioButtonGUI("coltype Sphere", &coltype, (int)sphere);
		gui->RadioButtonGUI("coltype Plain", &coltype, (int)plain);
		gui->RadioButtonGUI("coltype Quad", &coltype, (int)quad);
		gui->DragFloatGUI(&radius_, "radius");
		gui->DragFloat3GUI(&pos_, "pos");
		gui->DragFloat3GUI(&scale_, "scale");
		gui->DragFloatGUI(&pushBackPriority, "pushBackPriority");
		gui->DragIntGUI(&collideLayer, "collideLayer");
		colType_ = (colType)coltype;
	};

	std::function<void(void)>df = [&]()
	{
		componentDeleteFlag = true;
	};

	gui->ComponentGUI(f, df, "Collider");
}

void IFE::Collider::OutputComponent()
{
	JsonManager* j = JsonManager::Instance();
	j->OutputBool("enabled", enabled);
	j->OutputBool("isTrigger", isTrigger);
	j->OutputBool("pushable", pushable_);
	j->OutputInt("colType", (int)colType_);
	j->OutputFloat("radius", radius_);
	j->OutputFloat3("pos", pos_);
	j->OutputFloat3("scale", scale_);
	j->OutputFloat("pushBackPriority", pushBackPriority);
	j->OutputInt("collideLayer", collideLayer);
}

#endif

void IFE::Collider::LoadingComponent()
{
	JsonManager* j = JsonManager::Instance();
	enabled = j->InputBool("enabled");
	isTrigger = j->InputBool("isTrigger");
	pushable_ = j->InputBool("pushable");
	colType_ = (colType)j->InputInt("colType");
	radius_ = j->InputFloat("radius");
	pos_ = SetVector3(j->InputFloat3("pos"));
	scale_ = SetVector3(j->InputFloat3("scale"));
	pushBackPriority = j->InputFloat("pushBackPriority");
	collideLayer = j->InputInt("collideLayer");
}

