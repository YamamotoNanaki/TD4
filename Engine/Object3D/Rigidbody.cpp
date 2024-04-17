#include "Rigidbody.h"
#include "ComponentManager.h"
#include "ImGuiManager.h"
#include "JsonManager.h"

void IFE::Rigidbody::VelocityMove()
{
	ADXRigidbody::VelocityMove();
}

void IFE::Rigidbody::Update()
{
	ADXRigidbody::Update(objectPtr);
}

void IFE::Rigidbody::Initialize()
{
	ADXRigidbody::Initialize(objectPtr);
}

void IFE::Rigidbody::Copy(Component* c)
{
	Rigidbody* r = dynamic_cast<Rigidbody*>(c);
	if (r == nullptr)return;
	drag = r->drag;
	gravity = r->gravity;
	gravityScale = r->gravityScale;
	dragAxis = r->dragAxis;
	velocity = r->velocity;
}

#ifdef _DEBUG
void IFE::Rigidbody::DebugGUI()
{
	ImguiManager* gui = ImguiManager::Instance();
	std::function<void(void)> f = [&]()
	{
		gui->DragFloatGUI(&drag, "drag");
		gui->DragFloat3GUI(&gravity, "gravity");
		gui->DragFloatGUI(&gravityScale, "gravityScale");
		gui->CheckBoxGUI(&dragAxis.x, "dragAxisFlag X");
		gui->CheckBoxGUI(&dragAxis.y, "dragAxisFlag Y");
		gui->CheckBoxGUI(&dragAxis.z, "dragAxisFlag Z");
		gui->DragFloat3GUI(&velocity, "velocity");
	};

	std::function<void(void)>df = [&]()
	{
		componentDeleteFlag = true;
	};

	gui->ComponentGUI(f, df, "Rigidbody");
}

void IFE::Rigidbody::OutputComponent()
{
	JsonManager* j = JsonManager::Instance();
	j->OutputFloat("drag", drag);
	j->OutputFloat3("gravity", gravity);
	j->OutputFloat("gravityScale", gravityScale);
	j->OutputBool("dragAxisX", dragAxis.x);
	j->OutputBool("dragAxisY", dragAxis.y);
	j->OutputBool("dragAxisZ", dragAxis.z);
	j->OutputFloat3("velocity", velocity);
}
#endif
void IFE::Rigidbody::LoadingComponent()
{
	JsonManager* j = JsonManager::Instance();
	drag = j->InputFloat("drag");
	gravity = SetVector3(j->InputFloat3("gravity"));
	gravityScale = j->InputFloat("gravityScale");
	dragAxis.x = j->InputBool("dragAxisX");
	dragAxis.y = j->InputBool("dragAxisY");
	dragAxis.z = j->InputBool("dragAxisZ");
	velocity = SetVector3(j->InputFloat3("velocity"));
}
