#include "EventFactory.h"
#include "DamageEvent.h"
#include "DoorEvent.h"
#include "DroneKeepoutZoneEvent.h"
#include "EnemySpawnEvent.h"
#include "GimmickSwitchEvent.h"


//ÉCÉxÉìÉgÇÃéÌóﬁí«â¡ÇµÇΩÇ»ÇÁÇ±Ç±Ç…ëùÇ‚Ç∑

IFE::EventFactory::EventFactory()
{
}

IFE::EventFactory::~EventFactory()
{
}

IFE::EventFactory* IFE::EventFactory::Instance()
{
	static EventFactory inst;
	return &inst;
}

std::unique_ptr<IFE::IEvent> IFE::EventFactory::CreateEventClass(const std::string& EventTypeName)
{

	std::unique_ptr<IFE::IEvent> newEvent = nullptr;

	if (EventTypeName == "DamageEvent")
	{
		newEvent = std::make_unique<DamageEvent>();
		
	}
	else if (EventTypeName == "DoorEvent")
	{
		newEvent = std::make_unique<DoorEvent>();

	}
	else if (EventTypeName == "DroneKeepoutZoneEvent")
	{
		newEvent = std::make_unique<DroneKeepoutZoneEvent>();

	}
	else if (EventTypeName == "EnemySpawnEvent")
	{
		newEvent = std::make_unique<EnemySpawnEvent>();

	}
	else if (EventTypeName == "GimmickSwitchEvent")
	{
		newEvent = std::make_unique<GimmickSwitchEvent>();

	}
	else if (EventTypeName == "")
	{
		newEvent = std::make_unique<DamageEvent>();
	}
	else
	{
		newEvent = std::make_unique<DamageEvent>();
	}

	return std::move(newEvent);

}