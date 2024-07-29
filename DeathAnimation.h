#pragma once
#include"Component.h"
#include"IFEMath.h"
#include"SpriteManager.h"
#include"Transform.h"
#include"Ease.h"

class DeathAnimation :public IFE::Component
{
private:

	//���S�֘A
	bool deathAnimationFlag_ = false;
	bool deathAnimationEndFlag_ = false;
	float dethaAnimationTime_ = 0.0f;
	bool deathFadeAnimationFlag_ = false;
	float deathFadeAnimationTime_ = 0.0f;
	const float maxDeathFadeAnimationTime_ = 2.0f;

	//�I��
	bool isContinue_ = true;

	const uint16_t controllerRange_ = 10000;
	float oldLAnalog_ = 0.0f;

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

	void SetDeathAnimationFlag(bool flag);

private:

	void Fade();

	void Select();
};