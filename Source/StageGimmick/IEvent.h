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

	};


}
