#pragma once
#include"Component.h"
#include"IFEMath.h"
#include"CameraManager.h"
#include"PlayerAttack.h"
#include"PlayerHp.h"
#include"SpriteManager.h"
#include"PlayerActionCamera.h"
#include"EnemyManager.h"
#include"Animator.h"

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

	uint8_t hp_ = 10;
	bool isHit_ = false;
	float hitTime_;
	const int8_t HIT_COOLTIME = 1;

	//PlayerHp
	IFE::PlayerHp* playerHp_ = nullptr;

	//�U������N���X
	PlayerAttack* playerAttack_ = nullptr;

#pragma region �J����

	PlayerActionCamera* camera_ = nullptr;
	
#pragma endregion �J����

	IFE::Vector3 camerafrontVec_ = {};

	const float normalMoveSpeed_ = 10.0f;
	const float crouchMoveSpeed_ = 5.0f;
	float moveSpeed_ = 0.0f;

	bool attackFlag_ = false;
	bool isAttack_ = false;
	const float maxAttackTime_ = 0.8f;
	const float maxAttackAnimationTime_ = 1.0f;
	float attackTimer_ = 0;

	const uint16_t controllerRange_ = 10000;

	//���ۂ̕����x�N�g��
	IFE::Vector3 actualFrontVec_ = { 0,0,1.0f };
	//���͂���Ă������
	IFE::Vector3 targetVec_ = { 0,0,0 };

	IFE::EnemyManager* enemyManager_ = nullptr;
	IFE::BaseEnemy* closestEnemy = nullptr;
	bool isAttackUI_ = false;

	IFE::Animator* ani_ = nullptr;
	bool isWalk_ = false;
	bool oldIsWalk_ = false;

	bool crouchFlag_ = false;
	bool crouchAnimationFlag_ = false;
	float crouchAnimationTimer_ = 0.0f;
	const float maxCrouchTime_ = 0.2f;


	//�X���[�֘A

	const float minSlowSpeed_ = 0.3f;
	float nowGameTimeScale_;
	float slowEaseTime_;
	bool slowFlag_ = false;

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

	/// <summary>
	/// �̗͌���
	/// </summary>
	void DecHp(bool isBack_);

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

	const IFE::Vector3 GetRot();

	const float GetRotY();

	//Getter
	bool GetIsHit() { return isHit_; }

	void SetAnimation(std::string name);

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
	/// �U������
	/// </summary>
	void Attack();

	void SlowMotion();

	void AttackUI();

	// �ڕW�l�ɋ߂Â���֐�
	void ApproachTarget(float& current, float target, float step);

	void AutoAim();

	void IsWalk();

	void Crouch();
	void CrouchAnimation();

#ifdef EditorMode
	bool cheatFlag_ = false;
	void ComponentDebugGUI()override;
#endif
};