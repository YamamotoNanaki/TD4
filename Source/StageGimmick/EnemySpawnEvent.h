#pragma once
#include "IEvent.h"
#include "IFEMath.h"
#include "Object3D.h"

class EnemySpawnEvent :public IFE::IEvent
{
public:
	EnemySpawnEvent();
	~EnemySpawnEvent();

	//������
	void Initialize()override;

	//���t���[���X�V
	void Update()override;

	//�f�o�b�N�X�V
	void DebugGUI()override;

	//�g���M�~�b�N���Ńf�[�^���o�������Ƃ�
	void OutputData(nlohmann::json& json)override;

	//�g���M�~�b�N���Ńf�[�^���󂯎�肽���Ƃ�
	void InputData(nlohmann::json& json)override;

	//�J�n���̏�����
	void StartInitialize()override;

	//�M�~�b�N���̈ʒu���~�����Ƃ�
	void InputTransform(const IFE::Transform& transform)override;

private:

	//�N������ʒu
	IFE::Float3 spawnPos_;

	//�g���b�v�̈ʒu
	IFE::Float3 trapPos_;

	//�N�����G�̐�
	int32_t enemySpawnNum_ = 1;

	//�N�������G�̐�
	int32_t enemyNum_ = 0;

	//�G�̏��
	int32_t MaxEnemyNum_ = 10;

	float enemySpawnCT_ = 0;
	float enemySpawnMaxCT_ = 10;

	IFE::Object3D* EnemyManagerObject_ = nullptr;


};