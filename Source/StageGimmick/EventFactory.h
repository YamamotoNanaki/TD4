#pragma once

#include "IFEMath.h"
#include "IEvent.h"
#include "IFETime.h"

namespace IFE
{
	class EventFactory
	{
	public:
		EventFactory();
		~EventFactory();

		static EventFactory* Instance();

		/// <summary>
		/// �C�x���g�̃N���X����
		/// </summary>
		/// <param name="EventTypeName">�����������C�x���g���</param>
		/// <returns>���������C�x���g</returns>
		std::unique_ptr<IEvent> CreateEventClass(const std::string& EventTypeName);

	private:

	};

	
}