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
		//�f�t�H���g1(1�{)�A0�ɂ���Ǝ~�܂�
		//�^�C���X�P�[����������Ƃ��̓^�C���X�P�[���~�I�t�Z�b�g
		float timeOffset_ = 1;

	public:
		/// <summary>
		/// �G�t�F�N�g���Đ�����֐�
		/// </summary>
		/// <param name="position">�|�W�V����</param>
		/// <param name="rotation">��]�A�x���@</param>
		/// <param name="scale">�X�P�[��</param>
		void Play(IFE::Float3 position = { 0,0,0 }, IFE::Float3 rotation = { 0,0,0 }, IFE::Float3 scale = { 1,1,1 });
		/// <summary>
		/// �Ǐ]����G�t�F�N�g���Đ�����֐�
		/// </summary>
		/// <param name="position">�Ǐ]����|�W�V����</param>
		/// <param name="rotation">�Ǐ]�����]�A�x���@</param>
		/// <param name="scale">�Ǐ]����X�P�[��</param>
		/// <param name="startpos">pos��null�̎���pos</param>
		/// <param name="startrot">rot��null�̎���rot</param>
		/// <param name="startsca">sca��null�̎���sca</param>
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
