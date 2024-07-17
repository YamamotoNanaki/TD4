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
	//�Q�[���J�n���݂̂̏�����
	void StartInit();

	//�}�b�v��]���̍X�V
	void RotationMapUpdate();
	//�}�b�v�Œ莞�̍X�V
	void FixedMapUpdate();

private:

	IFE::Object3D* playerObj_ = nullptr;

	std::vector<IFE::Float2> objectPos_;
	std::vector<float> objectRot_;

	std::vector<IFE::Sprite*> mapSprite_;
	std::vector<std::pair<IFE::Sprite*,std::string>> enemySprite_;

	

	IFE::Sprite* playerSprite_;

	//�摜�̐����ȂǃQ�[���J�n���ɋN���Ăق������Ƃ̂��߂̃t���O(Initialize�ł��Ȃ��̂̓G�f�B�^�Ő��������Ƃ���Initialize���Ă΂�邽��)
	bool IsInit_ = false;

	//�}�b�v����]���邩
	bool isMapRot_ = true;

};

