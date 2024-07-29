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
	float dethaAnimationTime_ = 0.0f;
	bool deathFadeAnimationFlag_ = false;
	float deathFadeAnimationTime_ = 0.0f;
	const float maxDeathFadeAnimationTime_ = 1.0f;

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
};