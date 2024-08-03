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
			END,
			FOUND
		};
		Step step;
		Step oldStep;
		//説明済みフラグ
		bool isFound;

	public:
		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 更新
		/// </summary>
		void Update();

		void ChangeStep();

		void CameraText();

		void MoveText();

		void KillText();

		void KilledText();

		void DroneText();

		void FoundText();

		void CutInStep();

		void NextText(const std::string& str);

		void HideUI();

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize();

		void Reset();

		bool GetIsShowText() { return isShowText; }
	};
}

