#pragma once

namespace IFE {
	class IEvent
	{
	public:
		virtual ~IEvent() {};

		//������
		virtual void Initialize() {};

		//�I������
		virtual void Finalize() {};

		//���t���[���X�V
		virtual void Update() {};

		//�I���t���O�l��
		virtual bool GetIsEnd() { return isEnd_; };

		//�ݒ���o��
		virtual void DebugGUI() {};

	protected:

		//���̃C�x���g���I����Ă��邩
		bool isEnd_ = false;

	};


}
