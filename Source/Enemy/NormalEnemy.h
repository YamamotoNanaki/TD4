#pragma once
#include "BaseEnemy.h"
#include "PlayerAction.h"

namespace IFE {
	//---�߂����ᕁ�ʂ̓G---//
	class NormalEnemy : public IFE::BaseEnemy
	{
	//�����o�ϐ�
	private:
		//�ҋ@�^�C�}�[
		int waitTimer;
		//�o�R�n�_
		std::vector<Float3> points;
		//�o�R�n�_��
		int nowPoints;
		//lerp�p
		float moveTime;

	//�����o�֐�
	public:
		/// <summary>
		/// ���W�w��Ȃ�������
		/// </summary>
		void Initialize();

		/// <summary>
		/// ���W�w�肠�菉����
		/// </summary>
		void Initialize(const Vector3& pos_, const std::vector<Float3>& points);

		/// <summary>
		/// ��ԕω�
		/// </summary>
		void ChangeState();

		/// <summary>
		/// �X�V
		/// </summary>
		void Update();

		/// <summary>
		/// �T��
		/// </summary>
		void Search();

		/// <summary>
		/// �ǂ�
		/// </summary>
		void Chase();

		/// <summary>
		/// �`��
		/// </summary>
		void Draw();

		/// <summary>
		/// �q�b�g��
		/// </summary>
		/// <param name="collider"></param>
		void OnColliderHit(IFE::Collider collider);

		/// <summary>
		/// �I��
		/// </summary>
		void Finalize();

		/// <summary>
		/// �f�o�b�N���ɒ����ł���炵��
		/// </summary>
		void ComponentDebugGUI();
	};
}