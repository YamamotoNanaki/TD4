#pragma once
#include <COmponent.h>
#include"IFEMath.h"
#include "Propeller.h"
#include"ObjectManager.h"
#include "Propeller.h"
#include "ModelManager.h"

class PlayerDrone :public IFE::Component
{
private:

	bool isDroneSurvival_ = false;

	IFE::Vector3 moveValue_ = {};

	IFE::Vector3 front_ = { 0,0,1 };
	IFE::Vector3 pos_ = { 0,0,0 };
	IFE::Vector3 frontVec_ = {};

	IFE::Camera* droneCamera_ = nullptr;
	bool cameraMode_ = false;

	const uint16_t controllerRange_ = 10000;

	float time_ = 0;
	float dronePosY_ = 0.0f;

	bool slowSpeedMode_ = false;

	IFE::Propeller* propeller_[4];

	/// <summary>
	/// �i�Xspeed��0�ɂ��鏈��
	/// </summary>
	/// <param name="speed"></param>
	void SpeedZero(float& speed, float decelerationSpeed);

public:

	/// <summary>
	/// ����������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

	void OnColliderHit(IFE::ColliderCore collider);

	/// <summary>
	/// Player�N���X�ŌĂԑ���S�ʍX�V����
	/// </summary>
	void MoveUpdate();
	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();

	void MoveLimit();
	/// <summary>
	/// ��]����
	/// </summary>
	void Rotation();

	/// <summary>
	/// �J�����̍X�V����
	/// </summary>
	void CameraUpdate();

	bool GetDrawFlag();
	void SetDrawFlag(bool flag);

	void SetPos(const IFE::Vector3& pos);
	IFE::Vector3 GetPos();

	void SetRotY(const float rot);

	IFE::Camera* GetDroneCamera();

	void SetIsDroneSurvival(bool flag);
	bool GetIsDroneSurvival();
};