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

	void SetIsMapRot(bool flag) { isMapRot_ = flag; };

#ifdef EditorMode

	void ComponentDebugGUI()override;
	void OutputComponent(nlohmann::json& json)override;
#endif
	void LoadingComponent(nlohmann::json& json)override;

private:
	//ゲーム開始時のみの初期化
	void StartInit();

	//マップ回転時の更新
	void RotationMapUpdate();
	//マップ固定時の更新
	void FixedMapUpdate();

	//範囲内にいるか
	bool AABB(IFE::Float2 pos1, IFE::Float2 scale1, IFE::Float2 pos2, IFE::Float2 scale2);

	//AABBじゃ無理だった
	bool OBB(IFE::Vector2 pos1, IFE::Float2 scale1, IFE::Vector2 pos2, IFE::Float2 scale2,float angle1,float angle2);

	//OBBのおまけ
	float GetProjectionRadius(IFE::Float2 scale, IFE::Vector2 Axis, IFE::Vector2 AxisX, IFE::Vector2 AxisY);

private:

	IFE::Object3D* playerObj_ = nullptr;
	IFE::Object3D* playerActionObj_ = nullptr;
	IFE::Object3D* playerDroneObj_ = nullptr;

	std::vector<IFE::Float2> objectPos_;
	std::vector<float> objectRot_;

	std::vector<IFE::Sprite*> mapSprite_;
	std::vector<std::pair<IFE::Sprite*,std::string>> enemySprite_;

	

	IFE::Sprite* playerSprite_;

	//画像の生成などゲーム開始時に起きてほしいことのためのフラグ(Initializeでやらないのはエディタで生成したときにInitializeが呼ばれるため)
	bool IsInit_ = false;

	//マップが回転するか
	bool isMapRot_ = true;

};

