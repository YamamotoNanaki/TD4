#include "EventFactory.h"
#include "DamageEvent.h"


//ƒCƒxƒ“ƒg‚Ìí—Ş’Ç‰Á‚µ‚½‚È‚ç‚±‚±‚É‘‚â‚·

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