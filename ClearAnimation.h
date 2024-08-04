#pragma once
#include"Component.h"
#include"IFEMath.h"
#include"SpriteManager.h"
#include"Transform.h"
#include"Ease.h"

class ClearAnimation :public IFE::Component
{
private:

	//�A�j���[�V�����֘A
	bool animationFlag_ = false;
	float animationTimer_ = 0.0f;
	const float maxAnimationTimer_ = 5.0f;

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

	void SetAnimationFlag(bool flag);
};