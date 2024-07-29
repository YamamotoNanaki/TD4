#pragma once
#include <list>
#include <string>
#include <utility>
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
		struct TransformEffek
		{
			Float3* pos = nullptr;
			Float3* rot = nullptr;
			Float3* sca = nullptr;
			inline bool GetMoveFlag()
			{
				return pos || rot || sca;
			}
		};
		Effekseer::EffectRef effect_;
		std::list<Effekseer::Handle>* handles_;
		std::list<std::pair<Effekseer::Handle, TransformEffek>> playList_;
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
		/// <summary>
		/// 追従するエフェクトを再生する関数
		/// </summary>
		/// <param name="position">追従するポジション</param>
		/// <param name="rotation">追従する回転、度数法</param>
		/// <param name="scale">追従するスケール</param>
		/// <param name="startpos">posがnullの時のpos</param>
		/// <param name="startrot">rotがnullの時のrot</param>
		/// <param name="startsca">scaがnullの時のsca</param>
		void Play(IFE::Float3* position, IFE::Float3* rotation, IFE::Float3* scale, Float3 startpos = {0,0,0},Float3 startrot = { 0,0,0 },Float3 startsca = { 1,1,1 });
		void Update();
		void Initiaize(::Effekseer::ManagerRef manager, Effekseer::EffectRef effect, std::list<Effekseer::Handle>* handle);
		void Destroy();
		inline bool GetDeleteFlag() { return deleteFlag_; };
		uint32_t GetPlayNum() { return uint32_t(playList_.size()); };
		void SetPosition(uint32_t num, IFE::Float3 position);

#ifdef EditorMode
		void DebugGUI();
#endif

	private:
		void SetRota(Effekseer::Handle h, const Float3& rota);
	};
}
