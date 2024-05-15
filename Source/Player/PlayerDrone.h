#pragma once
#include <COmponent.h>
#include"IFEMath.h"

class PlayerDrone :public IFE::Component
{
private:

	IFE::Vector3 speed_ = {};

	IFE::Vector3 front_ = { 0,0,1 };
	IFE::Vector3 pos_ = { 0,0,0 };
	IFE::Vector3 frontVec_ = {};

	IFE::Camera* droneCamera_ = nullptr;
	bool cameraMode_ = false;

	const uint16_t controllerRange_ = 10000;

	/// <summary>
	/// �i�Xspeed��0�ɂ��鏈��
	/// </summary>
	/// <param name="speed"></param>
	void SpeedZero(float& speed);

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
};