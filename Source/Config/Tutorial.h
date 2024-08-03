#pragma once
#include"Component.h"
#include"IFEMath.h"
#include"SpriteManager.h"
#include"Transform.h"
#include"Ease.h"

namespace IFE {
	class Tutorial :public IFE::Component
	{
	private:

		bool isShowText = false;
		float tutoTime;
		std::string nowText;
		enum Step {
			CAMERA,
			MOVE,
			BACK,
			ENEMY,
			DRONE,
			SCAN,
			COMEBACK,
			END,
			FOUND
		};
		Step step;
		Step oldStep;
		//�����ς݃t���O
		bool isFound;
		//�`���[�g���A�����܂���
		bool isPlayTutorial = false;

	public:
		/// <summary>
		/// ������
		/// </summary>
		void Initialize();

		/// <summary>
		/// �X�V
		/// </summary>
		void Update();

		void ChangeStep();

		void CameraText();

		void MoveText();

		void KillText();

		void KilledText();

		void DroneText();

		void ScanText();

		void ComeBackText();

		void FoundText();

		void CutInStep();

		void NextText(const std::string& str);

		void HideUI();

		/// <summary>
		/// �I��
		/// </summary>
		void Finalize();

		void Reset();

		bool GetIsShowText() { return isShowText; }

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

