#include "stdafx.h"
#include "Event.h"
#include "LivingEntity.h"
Event::Event(LivingEntity* starter, LivingEntity* target, EventType type):m_pEventStarter(starter),m_pEventTarget(target),m_EventType(type)
{

}

Event::~Event()
{
	m_pEventStarter = nullptr;
	m_pEventTarget = nullptr;

	delete m_pEventStarter;
	delete m_pEventTarget;
}

int Event::Start() const
{
	switch (m_EventType)
	{
	case EventType::ATTACK:
		return m_pEventStarter->Attack(m_pEventTarget);
	case EventType::USE:
		return m_pEventStarter->UseItem();
	default:
		return 0;

	}
}
