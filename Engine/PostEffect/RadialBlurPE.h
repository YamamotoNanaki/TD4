#pragma once
#include "IPostEffect.h"

namespace IFE
{
	class RadialBlurPE : public IPostEffect
	{
		struct RadialCB {
			Float2 center = { 0.5,0.5 }; // 中心点 (0.5, 0.5 など)
			float blurAmount; // ブラーの強さ
			Float2 pad;
		};
		RadialCB* buff_;
		std::unique_ptr<IFE::ConstBuffer<RadialCB>>cb_;
	public:
		void SetCenter(Float2 center);
		void SetBlurAmount(float blurAmount);
		void Draw();
		void Update();
		void Initialize();

#ifdef EditorMode
		void PostEffectDebugGUI()override;
#endif
	};
}
