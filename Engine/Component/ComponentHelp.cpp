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

using namespace IFE;
using namespace std;

template <class T>
static Component* GetPtr(const string& s);

/*�����R�s�y
tmp = std::move(GetT<�N���X��>(str));
if (tmp != nullptr)return tmp;
*/

Component* IFE::StringToComponent(const std::string& str)
{
	Component* tmp = nullptr;
	//���ɃR�s�y

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

	//��������Q�[���̂��
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

	//�X�e�[�W�M�~�b�N
	tmp = std::move(GetPtr<LaserWire>(str));
	if (tmp != nullptr)return tmp;

	//�ǊǗ�
	tmp = std::move(GetPtr<StageCollideManageer>(str));
	if (tmp != nullptr)return tmp;

	//UI
	tmp = std::move(GetPtr<PlayerHp>(str));
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