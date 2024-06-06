#pragma once
#include"Component.h"
#include"IFEMath.h"
#include"CameraManager.h"
#include"PlayerAttack.h"
#include"PlayerHp.h"
#include"SpriteManager.h"

class PlayerAction : public IFE::Component
{
private:

	IFE::Vector3 front_ = { 0,0,1 };
	IFE::Vector3 pos_ = { 0,0,0 };

	//���ʃx�N�g���̍쐬
	IFE::Vector3 frontVec_ = {};
	//���x�N�g��
	IFE::Vector3 temporaryVec_ = { 0,1,0 };
	//�E�x�N�g���̍쐬
	IFE::Vector3 rightVec_ = {};

	float rotY_ = 0.0f;

	uint8_t hp = 10;

	//PlayerHp
	IFE::PlayerHp* playerHp_ = nullptr;

	//�U������N���X
	PlayerAttack* playerAttack_ = nullptr;

#pragma region �J����
	IFE::Vector3 cameraPos_ = {};
	IFE::Vector2 cameraAngle_ = {};

	IFE::Camera* actionCamera_ = nullptr;
#pragma endregion �J����

	bool attackFlag_ = false;
	const uint8_t attackTime_ = 10;
	uint8_t attackTimer_ = 0;

	const uint16_t controllerRange_ = 10000;

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

	void OnColliderHit(IFE::ColliderCore* myCollider, IFE::ColliderCore* hitCollider)override;


	/// <summary>
	/// Player�N���X�ŌĂԑ���S�ʍX�V����
	/// </summary>
	void MoveUpdate();

	/// <summary>
	/// position�̎擾
	/// </summary>
	/// <returns></returns>
	const IFE::Vector3 GetPos();

	/// <summary>
	/// �A�N�V�����J�����擾
	/// </summary>
	/// <returns></returns>
	IFE::Camera* GetActionCamera();

	/// <summary>
	/// �U���t���O�̎擾
	/// </summary>
	/// <returns></returns>
	const bool GetAttackFlag();

	const IFE::Vector3 GetFrontVec();

	const float GetRotY();

private:

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

	/// <summary>
	/// �U������
	/// </summary>
	void Attack();

	/// <summary>
	/// �J�������
	/// </summary>
	void CameraComplement();

	/// <summary>
	/// �J�����̉�]����
	/// </summary>
	void CameraRot();
};