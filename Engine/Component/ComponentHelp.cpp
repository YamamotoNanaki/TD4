#include "ComponentHelp.h"
#include "Component.h"
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
	//UI
	Register("PlayerHp", &CreateInstance<PlayerHp>);
	Register("DroneRecoveryUI", &CreateInstance<DroneRecoveryUI>);
	Register("Title", &CreateInstance<Title>);
	Register("Clear", &CreateInstance<Clear>);
	Register("Over", &CreateInstance<Over>);
	Register("CameraChange", &CreateInstance<CameraChange>);
}

Component* IFE::ComponentHelp::StringToComponent(const std::string& str)
{
	//auto it = creators_.find(str);
	//if (it != creators_.end()) {
	//	return (it->second)();
	//}
	//return nullptr;

	Component* tmp = nullptr;
	//engine
	tmp = std::move(GetPtr<Transform>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<Transform2D>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<TransformParticle>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<TransferGeometryBuffer>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<ColorBuffer>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<Material>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<Collider>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<Animator>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<TransformCamera>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<RectTexture>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<AnimationTexture>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<Fog>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<DebugCamera>(str));
	if (tmp != nullptr)return tmp;

	//player
	tmp = std::move(GetPtr<Player>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<PlayerAction>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<PlayerDrone>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<PlayerAttack>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<PlayerCommonCamera>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<PlayerActionCamera>(str));
	if (tmp != nullptr)return tmp;

	//enemy
	tmp = std::move(GetPtr<NormalEnemy>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<EnemyHp>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<EnemyAttack>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<EnemyManager>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<Boss>(str));
	if (tmp != nullptr)return tmp;

	//Gimmick
	tmp = std::move(GetPtr<LaserWire>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<DroneKeepoutZoneObject>(str));
	if (tmp != nullptr)return tmp;

	tmp = std::move(GetPtr<StageCollideManageer>(str));
	if (tmp != nullptr)return tmp;
	//ui
	tmp = std::move(GetPtr<PlayerHp>(str));
	if(tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<DroneRecoveryUI>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<Title>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<Clear>(str));
	if (tmp != nullptr)return tmp;
	tmp = std::move(GetPtr<Over>(str));
	if (tmp != nullptr)return tmp;

	//Effect
	tmp = std::move(GetPtr<CameraChange>(str));
	if (tmp != nullptr)return tmp;

	return nullptr;
}

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

template<typename T>
Component* CreateInstance() {
	return new T();
}