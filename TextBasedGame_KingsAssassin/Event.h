//*******************************************************************
//Event class to assist our combat system
//Who started the event, who is the target of said event and
//The event type. The player has the options to attack or use an item
//The enemies attack every turn
//*******************************************************************
#pragma once

class LivingEntity;

enum class EventType {ATTACK,USE};

class Event
{
public:
	Event(LivingEntity* starter,LivingEntity* target,EventType type);
	~Event();

	int Start() const;

	LivingEntity* m_pEventStarter;
	LivingEntity* m_pEventTarget;
	EventType m_EventType;

};

