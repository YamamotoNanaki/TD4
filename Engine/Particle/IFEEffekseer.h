#pragma once
#include <list>
#include <string>
#include "IFEMath.h"
#include "EditorMode.h"

#pragma warning(push, 0)
#include <Effekseer.h>
#include <EffekseerRendererDX12.h>
#pragma warning(pop)

namespace IFE
{
	class IFEEffekseer
	{
		Effekseer::EffectRef effect_;
		std::list<Effekseer::Handle>* handles_;
		std::list<Effekseer::Handle> playList_;
		::Effekseer::ManagerRef manager_;
		bool deleteFlag_ = false;
		bool destroyFlag_ = false;

	public:
		std::string name_;
		bool timeScaleFlag_ = true;
		//デフォルト1(1倍)、0にすると止まる
		//タイムスケールがかかるときはタイムスケール×オフセット
		float timeOffset_ = 1;

	public:
		/// <summary>
		/// エフェクトを再生する関数
		/// </summary>
		/// <param name="position">ポジション</param>
		/// <param name="rotation">回転、度数法</param>
		/// <param name="scale">スケール</param>
		void Play(IFE::Float3 position = { 0,0,0 }, IFE::Float3 rotation = { 0,0,0 }, IFE::Float3 scale = { 1,1,1 });
		void Update();
		void Draw();
		void Initiaize(::Effekseer::ManagerRef manager, Effekseer::EffectRef effect, std::list<Effekseer::Handle>* handle);
		void Destroy();
		inline bool GetDeleteFlag() { return deleteFlag_; };

#ifdef EditorMode
		void DebugGUI();
#endif
	};
}
