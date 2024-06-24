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

		/// <summary>
		/// �C�x���g�̃N���X����
		/// </summary>
		/// <param name="EventTypeName">�����������C�x���g���</param>
		/// <returns>���������C�x���g</returns>
		static IEvent* CreateEventClass(const std::string& EventTypeName);

	private:

	};

	
}