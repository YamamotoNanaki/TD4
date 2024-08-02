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
		std::string nowText;
		enum Step {
			CAMERA,
			MOVE,
			BACK,
			ENEMY
		};
		Step step;

	public:
		/// <summary>
		/// èâä˙âª
		/// </summary>
		void Initialize();

		/// <summary>
		/// çXêV
		/// </summary>
		void Update();

		void CameraText();

		void MoveText();

		void NextText(const std::string& str);

		/// <summary>
		/// èIóπ
		/// </summary>
		void Finalize();

		void Reset();

		bool GetIsShowText() { return isShowText; }
	};
}

