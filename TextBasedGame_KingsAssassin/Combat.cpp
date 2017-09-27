#include "stdafx.h"
#include "Combat.h"
#include <algorithm>
#include <queue>
#include "Event.h"

//Give the player a couple of options to pick during the battle
//When we have enemies with the same name we adjust the with a number.
Combat::Combat(std::vector<LivingEntity*>& participants):m_Participants(participants)
{
	m_CombatChoices = GameEventHandler("You:", { "Attack","Use"},true);

	std::map<std::string, int> names;
	for(auto participant : m_Participants)
	{
		if (participant->m_ID == "Player") continue;

		if(names.count(participant->m_Name)==0)
		{
			names[participant->m_Name] = 0;

		}
		else if(names[participant->m_Name]==0)
		{
			names[participant->m_Name] = 1;
		}
	}

	for(auto &particpant :m_Participants)
	{
		std::string newName = particpant->m_Name;

		if(names.count(particpant->m_Name)>0 && names[particpant->m_Name]>0)
		{
			newName += " (" + std::to_string(names[particpant->m_Name]) + ")";
			names[particpant->m_Name] += 1;
		}

		particpant->m_Name = newName;
	}
}

Combat::~Combat()
{
	for(size_t i=0;i<m_Participants.size();++i)
	{
		m_Participants[i] = nullptr;
	}
	m_Participants.clear();
}

void Combat::Start()
{
	std::vector<LivingEntity*>::iterator player;
	std::vector<LivingEntity*>::iterator end;
	
	do
	{
		player = std::find_if(m_Participants.begin(), m_Participants.end(), [](LivingEntity* e) {return e->m_ID == "player"; });
		end = m_Participants.end();

		NextTurn();
	}
	while (player != end && m_Participants.size() > 1);

}
//When we defeated an enemy we remove it from the participants list
void Combat::Kill(LivingEntity* entity)
{
	auto position = std::find(m_Participants.begin(), m_Participants.end(), entity);

	if(position != m_Participants.end())
	{
		SetConsoleTextAttribute(m_hConsole, 12);
		std::cout << entity->m_Name << " has been defeated!" << std::endl;
		SetConsoleTextAttribute(m_hConsole, 7);

		entity->m_CurrentHp = 0;
		m_Participants.erase(position);
	}
}

//For every turn based on the choice of the player we generate an event. (Attack - Use)
//The players gets to choose which entity to attack.
void Combat::NextTurn()
{
	std::queue<Event> events;

	std::vector<LivingEntity*> participants = m_Participants;

	std::sort(participants.begin(), participants.end(), [](LivingEntity* e1, LivingEntity* e2) {return e1->m_Agility > e2->m_Agility; });
	//std::sort(m_Participants.begin(), m_Participants.end(), [](LivingEntity* e1, LivingEntity* e2) {return e1->m_Agility > e2->m_Agility; });

	for (auto participant : m_Participants)
	{
		if (participant->m_ID == "player")
		{
			GameEventHandler targetChoice = GameEventHandler("Who do you attack?", {}, true);
			for (auto target : m_Participants)
			{
				if (target->m_ID != "player")
				{
					targetChoice.AddChoice(target->m_Name);
				}
			}

			int pickedAction = m_CombatChoices.PickOption();

			switch (pickedAction)
			{
			case 1:
			{
				int position = targetChoice.PickOption();

				for (int i = 0; i < position; ++i)
				{
					if (m_Participants[i]->m_ID == "player") ++position;
				}
				LivingEntity* target = m_Participants[position - 1];
				events.push(Event(participant, target, EventType::ATTACK));
				break;
			}
			case 2:
			{
				events.push(Event(participant, nullptr, EventType::USE));
				break;
			}
			default:
				break;
			}
		}
		else
		{
			LivingEntity* pPlayer = *std::find_if(m_Participants.begin(), m_Participants.end(), [](LivingEntity* e) {return e->m_ID == "player"; });
			events.push(Event(participant, pPlayer, EventType::ATTACK));
		}
	}
	while (!events.empty())
	{
		Event event = events.front();
		switch (event.m_EventType)
		{
		case EventType::ATTACK:
		{
			auto start = m_Participants.begin();
			auto end = m_Participants.end();

			if (std::find(start, end, event.m_pEventStarter) == end || std::find(start, end, event.m_pEventTarget) == end)
			{
				break;
			}
			SetConsoleTextAttribute(m_hConsole, 12);
			std::cout << event.m_pEventStarter->m_Name << " attacks " << event.m_pEventTarget->m_Name << " for " << event.Start() << " damage! " << std::endl;
			SetConsoleTextAttribute(m_hConsole, 7);
			if (event.m_pEventTarget->m_CurrentHp <= 0)
			{
				Kill(event.m_pEventTarget);
			}
			break;
		}
		case EventType::USE:
		{
			std::string name = event.m_pEventStarter->m_Name;
			int healAmount = event.Start();

			if(healAmount >0)
			{
				SetConsoleTextAttribute(m_hConsole, 10);
				std::cout << name << " heals for " << healAmount << std::endl;
				SetConsoleTextAttribute(m_hConsole, 7);

			}
			
			break;
		}
		default:
			break;
		}
		events.pop();
	}
    participants.clear();
}

