#pragma once
#include "BaseEnemy.h"
#include "PlayerAction.h"

namespace IFE {
	//---�߂����ᕁ�ʂ̓G---//
	class NormalEnemy : public IFE::BaseEnemy
	{
	private:
		const float LERP_TIME = 200.f;
		//��������n������
		const int WAIT_TIME = 200;
		//�T�����ړ����x 
		const float MOVE_SPEED = 2.0f;
	//�����o�ϐ�
	private:
		//�ҋ@�^�C�}�[
		int waitTimer;
		//�o�R�n�_
		std::vector<Float3> points;
		//�o�R�n�_
		int pointA;
		int pointB;
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
		/// �҂�
		/// </summary>
		void Wait();

		/// <summary>
		/// �x��
		/// </summary>
		void Warning();

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
		void OnColliderHit(IFE::Collider* collider)override;

		/// <summary>
		/// �I��
		/// </summary>
		void Finalize();


#ifdef EditorMode
		/// <summary>
		/// �f�o�b�N���ɒ����ł���炵��
		/// </summary>
		void ComponentDebugGUI()override;
		void OutputComponent(nlohmann::json& json)override;
#endif
		void LoadingComponent(nlohmann::json& json)override;
	};
}