#include "IEvent.h"

std::string EventTypeToString(EventType eventType)
{
	switch (eventType) {
	case EventType::damage:   return "damage";
	case EventType::door: return "door";
	case EventType::DroneKeepoutZone: return "droneKeepoutZone";
	case EventType::EnemySpawn: return "EnemySpawn";
	case EventType::GimmickSwitch: return "GimmickSwitchEvent";
	default:    return "UNKNOWN";
	}
}