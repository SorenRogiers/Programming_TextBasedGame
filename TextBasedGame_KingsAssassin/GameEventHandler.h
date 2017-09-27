//*************************************************************************************
//Class used to process the inputs of the player.
//Help prints every available command
//Story prints the story related to the room again.
//Where tells you which room you are currently in
//Look tells you which items you see in the room
//Search allows you to search an object and puts all the foud objects in your inventory
//Open is used to open doors (doors are always placed last in the list "look" gives you
//Invalid is returned when the player inputs a unknown command
//Menu opens the menu to equip gear,view inventory or check your hp.
//Examine prints you more information about an object.
//*************************************************************************************
#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <JsonBox.h>
#include "Player.h"
#include "Windows.h"

class Room;
class Player;

class GameEventHandler
{
public:

	enum class Commands { HELP,STORY,WHERE,LOOK,SEARCH,OPEN,INVALID,MENU,EXAMINE };

	GameEventHandler(std::string description,std::vector<std::string> choices,bool printChoices=false);
	GameEventHandler(JsonBox::Value &v);
	GameEventHandler();
	~GameEventHandler();

	int PickOption();
	unsigned int GetSize() const;

	void AddChoice(std::string choice);
	void SetPrintChoices(bool printChoices);
	void ProcessCommand(std::string command,Room* pRoom,Player& pPlayer);
	void AttackPlayer(Room* pRoom,Player &player) const;
	
	static void SlowPrint(const std::string& stringToPrint,int delay);
	static void ShowMenu(Player& player);
private:
	std::string m_Description;
	std::vector<std::string> m_Choices;

	bool m_printChoices = false;

	Commands m_CurrentCommand = Commands::INVALID;
	HANDLE m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

private:
	void Search(Room* pRoom,Player& player, const std::string objectString) const;
	static void Examine(Room* pRoom, const std::string objectString);
	void Open(Room* pRoom,Player& player ,const std::string objectString) const;
	void Look(Room* pRoom)const;
	void ExecuteCommand(Room* pRoom, Player& player, const std::string objectString)const;
};

