#include "EventFactory.h"
#include "DamageEvent.h"


//ƒCƒxƒ“ƒg‚Ìí—Ş’Ç‰Á‚µ‚½‚È‚ç‚±‚±‚É‘‚â‚·

IFE::EventFactory::EventFactory()
{
}

IFE::EventFactory::~EventFactory()
{
}

IFE::IEvent* IFE::EventFactory::CreateEventClass(const std::string& EventTypeName)
{

	IFE::IEvent* newEvent = nullptr;

	if (EventTypeName == "DamageEvent")
	{
		DamageEvent* addevent = nullptr;
		newEvent = addevent;
	}
	else if (EventTypeName == "")
	{
		DamageEvent* addevent = nullptr;
		newEvent = addevent;
	}
	else
	{
		DamageEvent* addevent = nullptr;
		newEvent = addevent;
	}

	return newEvent;

}