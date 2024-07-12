#pragma once
#include "Component.h"
#include "IFEMath.h"
#include "Object3D.h"
#include "Sprite.h"
#include "utility"

class MapUI:public IFE::Component
{
public:
	MapUI();
	~MapUI();

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider);

#ifdef EditorMode

	void ComponentDebugGUI()override;
	void OutputComponent(nlohmann::json& json)override;
#endif
	void LoadingComponent(nlohmann::json& json)override;

private:
	//ゲーム開始時のみの初期化
	void StartInit();

private:

	IFE::Object3D* playerObj_ = nullptr;

	std::vector<IFE::Float2> objectPos_;
	std::vector<float> objectRot_;

	std::vector<IFE::Sprite*> mapSprite_;
	std::vector<std::pair<IFE::Sprite*,std::string>> enemySprite_;

	

	IFE::Sprite* playerSprite_;

	//画像の生成などゲーム開始時に起きてほしいことのためのフラグ(Initializeでやらないのはエディタで生成したときにInitializeが呼ばれるため)
	bool IsInit_ = false;

};

