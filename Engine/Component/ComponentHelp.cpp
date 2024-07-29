#include "ComponentHelp.h"
#include "StringUtil.h"
#include "Component.h"
#include "IPostEffect.h"
#include "DefaultPostEffect.h"
#include "RadialBlurPE.h"
#include "Transform.h"
#include "TransferGeometryBuffer.h"
#include "AnimationTexture.h"
#include "Material.h"
#include "CollisionPrimitive.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "ColorBuffer.h"
#include "RectTexture.h"
#include "Fog.h"
#include "Animator.h"
#include "DebugCamera.h"
#include <typeinfo>
#include"Player.h"
#include"PlayerAction.h"
#include"PlayerDrone.h"
#include "NormalEnemy.h"
#include "LaserWire.h"
#include "EnemyHp.h"
#include "StageCollideManageer.h"
#include "EnemyAttack.h"
#include "PlayerCommonCamera.h"
#include "EnemyManager.h"
#include "Boss.h"
#include "Title.h"
#include "Clear.h"
#include "Over.h"
#include"PlayerActionCamera.h"
#include"DroneRecoveryUI.h"
#include "DroneKeepoutZoneObject.h"
#include"CameraChange.h"
#include"PoseMenu.h"
#include"Config.h"
#include "MapUI.h"
#include"Attach3DModel.h"
#include "SwitchObject.h"
#include "GimmickTouch.h"
#include "DronePostEffect.h"
#include "EnemyHighlighting.h"
#include "Blood.h"

using namespace IFE;
using namespace std;

template <class T>
static Component* GetPtr(const string& s);

template<typename T>
static Component* CreateInstance();

void IFE::ComponentHelp::StaticHelpInitialize()
{
	//エンジン
	Register("Transform", &CreateInstance<Transform>);
	Register("Transform2D", &CreateInstance<Transform2D>);
	Register("TransformParticle", &CreateInstance<TransformParticle>);
	Register("TransferGeometryBuffer", &CreateInstance<TransferGeometryBuffer>);
	Register("ColorBuffer", &CreateInstance<ColorBuffer>);
	Register("Material", &CreateInstance<Material>);
	Register("Collider", &CreateInstance<Collider>);
	Register("Animator", &CreateInstance<Animator>);
	Register("TransformCamera", &CreateInstance<TransformCamera>);
	Register("RectTexture", &CreateInstance<RectTexture>);
	Register("AnimationTexture", &CreateInstance<AnimationTexture>);
	Register("Fog", &CreateInstance<Fog>);
	Register("DebugCamera", &CreateInstance<DebugCamera>);
	Register("Attach3DModel", &CreateInstance<Attach3DModel>);
	//プレイヤー
	Register("Player", &CreateInstance<Player>);
	Register("PlayerAction", &CreateInstance<PlayerAction>);
	Register("PlayerDrone", &CreateInstance<PlayerDrone>);
	Register("PlayerAttack", &CreateInstance<PlayerAttack>);
	Register("PlayerCommonCamera", &CreateInstance<PlayerCommonCamera>);
	Register("PlayerActionCamera", &CreateInstance<PlayerActionCamera>);
	//エネミー
	Register("NormalEnemy", &CreateInstance<NormalEnemy>);
	Register("EnemyHp", &CreateInstance<EnemyHp>);
	Register("EnemyAttack", &CreateInstance<EnemyAttack>);
	Register("EnemyManager", &CreateInstance<EnemyManager>);
	Register("Boss", &CreateInstance<Boss>);
	//ギミック
	Register("LaserWire", &CreateInstance<LaserWire>);
	Register("DroneKeepoutZoneObject", &CreateInstance<DroneKeepoutZoneObject>);
	Register("StageCollideManageer", &CreateInstance<StageCollideManageer>);
	Register("GimmickSwitchObject", &CreateInstance<SwitchObject>);
	Register("GimmickTouch", &CreateInstance<GimmickTouch>);
	//UI
	Register("PlayerHp", &CreateInstance<PlayerHp>);
	Register("DroneRecoveryUI", &CreateInstance<DroneRecoveryUI>);
	Register("Title", &CreateInstance<Title>);
	Register("Clear", &CreateInstance<Clear>);
	Register("Over", &CreateInstance<Over>);
	Register("CameraChange", &CreateInstance<CameraChange>);
	Register("MapUI", &CreateInstance<MapUI>);
	//ポーズ
	Register("PoseMenu", &CreateInstance<PoseMenu>);
	Register("Config", &CreateInstance<Config>);
	//エフェクト
	Register("Blood", &CreateInstance<Blood>);
}

template<typename T>
static IPostEffect* CreatePostEffectInstance()
{
	return new T();
}

void IFE::IPostEffectHelp::StaticHelpInitialize()
{
	Register("DefaultPostEffect", &CreatePostEffectInstance<DefaultPostEffect>);
	Register("RadialBlurPE", &CreatePostEffectInstance<RadialBlurPE>);
	Register("DronePostEffect", &CreatePostEffectInstance<DronePostEffect>);
	Register("EnemyHighlighting", &CreatePostEffectInstance<EnemyHighlighting>);
}

Component* IFE::ComponentHelp::StringToComponent(const std::string& str)
{
	auto it = creators_.find(str);
	if (it != creators_.end())
	{
		auto ptr = (it->second)();
		ptr->SetComponentName(str);
		return ptr;
	}
	return nullptr;
}

#ifdef EditorMode
#include "ImguiManager.h"

std::vector<std::string> IFE::ComponentHelp::GetRegisteredClasses()
{
	// Collect all keys (class names) from the unordered_map
	std::vector<std::string> classNames;
	for (const auto& pair : creators_) {
		classNames.push_back(pair.first);
	}

	// Sort the class names
	std::sort(classNames.begin(), classNames.end());

	return classNames;
}

std::vector<std::string> IFE::ComponentHelp::SearchClasses(const std::string& partialName)
{
	std::vector<std::string> matches;
	std::string lowerPartialName = ToLower(partialName);
	for (const auto& pair : creators_) {
		if (ToLower(pair.first).find(lowerPartialName) != std::string::npos) {
			matches.push_back(pair.first);
		}
	}
	std::sort(matches.begin(), matches.end());
	return matches;
}

string IFE::ComponentHelp::GetComponentList()
{
	static std::string search;
	ImguiManager::Instance()->InputTextGUI(U8("検索"), search);
	std::vector<std::string>items = ComponentHelp::SearchClasses(search);

	static int32_t returnNum = 0;
	ImguiManager::Instance()->Combo("component name", returnNum, items);
	return items[returnNum];
}

std::vector<std::string> IFE::IPostEffectHelp::GetRegisteredClasses()
{
	// Collect all keys (class names) from the unordered_map
	std::vector<std::string> classNames;
	for (const auto& pair : creators_) {
		classNames.push_back(pair.first);
	}

	// Sort the class names
	std::sort(classNames.begin(), classNames.end());

	return classNames;
}

std::vector<std::string> IFE::IPostEffectHelp::SearchClasses(const std::string& partialName)
{
	std::vector<std::string> matches;
	std::string lowerPartialName = ToLower(partialName);
	for (const auto& pair : creators_) {
		if (ToLower(pair.first).find(lowerPartialName) != std::string::npos) {
			matches.push_back(pair.first);
		}
	}
	std::sort(matches.begin(), matches.end());
	return matches;
}

std::string IFE::IPostEffectHelp::GetComponentList()
{
	static std::string search;
	ImguiManager::Instance()->InputTextGUI(U8("検索"), search);
	std::vector<std::string>items = IPostEffectHelp::SearchClasses(search);

	static int32_t returnNum = 0;
	ImguiManager::Instance()->Combo("posteffect name", returnNum, items);
	return items[returnNum];
}
#endif

template <class T>
static Component* GetPtr(const string& s)
{
	string n = typeid(T).name();
	if (n.starts_with("class "))
	{
		n.replace(0, 6, "");
	}
	if (n.starts_with("struct "))
	{
		n.replace(0, 7, "");
	}
	if (n.starts_with("IFE::"))
	{
		n.replace(0, 5, "");
	}
	if (s == n)
	{
		T* r = new T;
		r->SetComponentName(n);
		return r;
	}
	return nullptr;
}

void IFE::ComponentHelp::Register(const std::string& className, CreateFunc func)
{
	creators_[className] = func;
}

void IFE::IPostEffectHelp::Register(const std::string& className, CreateFunc func)
{
	creators_[className] = func;
}

template<typename T>
Component* CreateInstance() {
	return new T();
}

IPostEffect* IFE::IPostEffectHelp::StringToPostEffect(const std::string& componentName)
{
	auto it = creators_.find(componentName);
	if (it != creators_.end())
	{
		auto ptr = (it->second)();
		return ptr;
	}
	return nullptr;
}
