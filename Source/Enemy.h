#pragma once
#include"Component.h"
#include"IFMath.h"

class Enemy :public IFE::Component
{
private:




public:

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �I��
	/// </summary>
	void Finalize();
};