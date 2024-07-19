#pragma once
#pragma once
#include "BaseEnemy.h"
#include "EnemyHp.h"
#include "IFEMath.h"

namespace IFE {
	class Boss : public IFE::BaseEnemy
	{
		using BaseEnemy::BaseEnemy;
	private:
		//hp
		EnemyHp* hpUI = nullptr;
		//�����o�֐�
	public:
		/// <summary>
		/// ������
		/// </summary>
		void Initialize();

		/// <summary>
		/// �X�V
		/// </summary>
		void EnemyUpdate()override;

		/// <summary>
		/// �q�b�g��
		/// </summary>
		/// <param name="collider"></param>
		void EnemyOnColliderHit(ColliderCore* myCollider, ColliderCore* hitCollider)override;

		void Killed();

		/// <summary>
		/// �`��
		/// </summary>
		void Draw();

		/// <summary>
		/// �I��
		/// </summary>
		void Finalize();

		bool GetBack();
		bool GetIsHit() { return isHit_; }

#ifdef EditorMode
		/// <summary>
		/// �f�o�b�N���ɒ����ł���炵��
		/// </summary>
		void ComponentDebugGUI()override;
#endif
	};
}

