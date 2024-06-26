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
		/// イベントのクラス生成
		/// </summary>
		/// <param name="EventTypeName">生成したいイベント種類</param>
		/// <returns>生成したイベント</returns>
		std::unique_ptr<IEvent> CreateEventClass(const std::string& EventTypeName);

	private:

	};

	
}