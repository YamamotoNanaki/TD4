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
		Effekseer::EffectRef effect_;
		std::list<Effekseer::Handle>* handles_;
		std::list<std::pair<Effekseer::Handle, Float3*>> playList_;
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
		/// �|�W�V�����ɒǏ]����G�t�F�N�g���Đ�����֐�
		/// </summary>
		/// <param name="position">�Ǐ]����|�W�V����</param>
		/// <param name="rotation">��]�A�x���@</param>
		/// <param name="scale">�X�P�[��</param>
		void Play(IFE::Float3* position, IFE::Float3 rotation = { 0,0,0 }, IFE::Float3 scale = { 1,1,1 });
		void Update();
		void Initiaize(::Effekseer::ManagerRef manager, Effekseer::EffectRef effect, std::list<Effekseer::Handle>* handle);
		void Destroy();
		inline bool GetDeleteFlag() { return deleteFlag_; };
		uint32_t GetPlayNum() { return uint32_t(playList_.size()); };
		void SetPosition(uint32_t num, IFE::Float3 position);

#ifdef EditorMode
		void DebugGUI();
#endif
	};
}
