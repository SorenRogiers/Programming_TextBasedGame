//************************************************************
//Class to simulate combat
//Combat consist of the entities who participate in the battle
//The combat system is turn based.
//************************************************************
#pragma once
#include <vector>
#include "LivingEntity.h"
#include "GameEventHandler.h"
#include "Windows.h"
class Combat
{
public:
	Combat(std::vector<LivingEntity*>& participants);
	~Combat();

	void Start();
private:
	void Kill(LivingEntity* entity);
	void NextTurn();
private:
	std::vector<LivingEntity*> m_Participants;
	GameEventHandler m_CombatChoices;

	HANDLE m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

};

