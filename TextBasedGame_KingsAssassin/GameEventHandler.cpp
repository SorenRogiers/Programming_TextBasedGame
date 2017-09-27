#include "stdafx.h"
#include "GameEventHandler.h"
#include <algorithm>
#include "Room.h"
#include "EntryPoint.h"
#include <string>
#include "Player.h"
#include "Armor.h"
#include "Weapon.h"
#include "Combat.h"

GameEventHandler::GameEventHandler(std::string description,std::vector<std::string> choices,bool printChoices): m_Description(description),m_Choices(choices),m_printChoices(printChoices)
{
}

GameEventHandler::GameEventHandler(JsonBox::Value& v)
{
	JsonBox::Object o = v.getObject();

	for (auto choice : o["choices"].getArray())
		m_Choices.push_back(choice.getString());

}

GameEventHandler::GameEventHandler()
{
}


GameEventHandler::~GameEventHandler()
{
}

//Prints the choices available to the player.
//the options can be selected with a number
int GameEventHandler::PickOption()
{
	if (m_Description != "")
		std::cout << m_Description << std::endl;

	if (m_printChoices)
	{
		int choice = 1;
		std::for_each(m_Choices.begin(), m_Choices.end(), [&choice](const std::string& s) {std::cout << choice++ << ": " << s << std::endl; });
	}

	int userInput = -1;
	while (true)
	{
		SetConsoleTextAttribute(m_hConsole, 10);
		std::cin >> userInput;
		SetConsoleTextAttribute(m_hConsole, 7);

		if (!std::cin || userInput > static_cast<int>(m_Choices.size()))
		{
			std::cout << "Pick a number please in the list please" << std::endl;
			std::cin.clear();
			std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		}

		if (userInput >= 0 && userInput <= static_cast<int>(m_Choices.size()))
		{
			SetConsoleTextAttribute(m_hConsole, 14);
			std::cout << "You picked: " << m_Choices[userInput - 1] << std::endl;
			SetConsoleTextAttribute(m_hConsole, 7);

			return userInput;
		}
	}
}

void GameEventHandler::AddChoice(std::string choice)
{
	m_Choices.push_back(choice);
}

void GameEventHandler::SetPrintChoices(bool printChoices)
{
	m_printChoices = printChoices;
}

void GameEventHandler::ProcessCommand(std::string command, Room* pRoom,Player& player)
{
	std::string commandString;
	std::string objectString;

	if(command.find(" ") == std::string::npos)
	{
		commandString = command;
	} 
	else
	{
		size_t pos = 0;
		pos = command.find(" ");
		commandString = command.substr(0, pos);
		objectString = command.substr(pos+1);
	}

	std::for_each(commandString.begin(), commandString.end(), [](char &s) {s = toupper(s); });
	std::for_each(objectString.begin(), objectString.end(), [](char &s) {s = toupper(s); });

	if(commandString == "LOOK")
	{
		m_CurrentCommand = Commands::LOOK;
	} 
	else if(commandString=="SEARCH")
	{
		m_CurrentCommand = Commands::SEARCH;
	} 
	else if (commandString == "OPEN")
	{
		m_CurrentCommand = Commands::OPEN;
	} 
	else if(commandString == "EXAMINE")
	{
		m_CurrentCommand = Commands::EXAMINE;
	}
	else if(commandString=="MENU")
	{
		m_CurrentCommand = Commands::MENU;
	}
	else if(commandString =="STORY")
	{
		m_CurrentCommand = Commands::STORY;
	} else if(commandString =="HELP")
	{
		m_CurrentCommand = Commands::HELP;
	} 
	else if(commandString == "WHERE")
	{
		m_CurrentCommand = Commands::WHERE;
	}
	else
	{
		m_CurrentCommand = Commands::INVALID;
	}
	
	ExecuteCommand(pRoom, player, objectString);
}

void GameEventHandler::ShowMenu(Player& player)
{
	int result = GameEventHandler("==========\n== Menu ==\n==========", { "Inventory","Gear","Character Info" },true).PickOption();

	switch (result)
	{
	case 1:
	{
		std::cout << "===============\n== Inventory ==\n===============" << std::endl;
		player.m_inventory.Print();
		break;
	}
	case 2:
	{
		std::cout << "==========\n== Gear ==\n==========" << std::endl;
		std::cout << "Armor: " << (player.m_pCurrentArmor != nullptr ? player.m_pCurrentArmor->m_Name : "no armor currently equipped") << std::endl;
		std::cout << "Weapon: " << (player.m_pCurrentWeapon != nullptr ? player.m_pCurrentWeapon->m_Name : "no weapon currently equipped") << std::endl;

		int gearOptions = GameEventHandler("", { "Equip armor","Equip Weapon","Close" },true).PickOption();

		if (gearOptions == 1)
		{
			int userInput = 0;

			int numItems = player.m_inventory.Print<Armor>(true);

			if (numItems == 0)
			{
				std::cout << "no armor found." << std::endl;
				break;
			}

			while (!userInput)
			{
				std::cout << "Which item do you want to equip?" << std::endl;
				std::cin >> userInput;

				if (userInput >= 1 && userInput <= numItems)
				{
					player.EquipArmor(player.m_inventory.GetItemByIndex<Armor>(userInput - 1));
					std::cout << player.m_inventory.GetItemByIndex<Armor>(userInput - 1)->m_Name << " equipped." << std::endl;
				}
			}
		}
		else if (gearOptions == 2)
		{
			int userInput = 0;

			int numItems = player.m_inventory.Print<Weapon>(true);

			if (numItems == 0)
			{
				std::cout << "no weapon found" << std::endl;
				break;
			}

			while (!userInput)
			{
				std::cout << "Which weapon do you want to equip?" << std::endl;
				std::cin >> userInput;

				if (userInput >= 1 && userInput <= numItems)
				{
					player.EquipWeapon(player.m_inventory.GetItemByIndex<Weapon>(userInput - 1));
					std::cout << player.m_inventory.GetItemByIndex<Weapon>(userInput - 1)->m_Name << " equipped." << std::endl;

				}
			}
		}
		break;
	}
	case 3:
	{
		std::cout << "====================\n== Character info ==\n====================" << std::endl;
		std::cout << player.m_Name;

		if (player.m_ClassName != "Invalid")
		{
			std::cout << " the " << player.m_ClassName;
		}
		std::cout << std::endl;

		std::cout << "Health: " << player.m_CurrentHp << " / " << player.m_MaxHP << std::endl;
		break;
	}
	default:
		break;
	}
}

unsigned GameEventHandler::GetSize() const
{
	return m_Choices.size();
}

//When an non friendly entity is in the room and the player enters
//he attacks the player immediately
//Gathers all the entities in the room, places them in a particpants list along with the player and start a combat event.
void GameEventHandler::AttackPlayer(Room* pRoom, Player &player) const
{
	std::vector<LivingEntity*> participants;
	
	if(pRoom->m_EnemiesInRoom.size()>0)
	{
		SetConsoleTextAttribute(m_hConsole, 12);
		std::cout << "You're being attacked by a " << std::endl;
		
		for (auto s : pRoom->m_EnemiesInRoom)
			std::cout << "-" << s.m_Name << std::endl;

		SetConsoleTextAttribute(m_hConsole, 7);

		for (size_t i = 0; i<pRoom->m_EnemiesInRoom.size(); ++i)
		{
			if (pRoom->m_EnemiesInRoom[i].m_IsFriendly)
			{

			}
			else
			{
				LivingEntity* enemy = &(pRoom->m_EnemiesInRoom[i]);
				participants.push_back(enemy);
			}
		}
		
		participants.push_back(&player);
		Combat combat(participants);
		combat.Start();

		if (player.m_CurrentHp > 0)
		{
			pRoom->m_EnemiesInRoom.clear();
		}
		else
		{
			std::cout << "YOU DIED!\n Game over!" << std::endl;
		}

	}
	
}

void GameEventHandler::SlowPrint(const std::string& stringToPrint, int delay)
{
	for (size_t i = 0; i != stringToPrint.size(); ++i) {
		std::cout << stringToPrint[i];
		Sleep(delay);
	}
	std::cout << std::endl;
}
//Search an object
//The player has an inventory and an object has an inventory as well if the object has items
//We merge both inventories together and clear the inventory of the object
void GameEventHandler::Search(Room* pRoom,Player& player ,const std::string objectString) const
{
	bool IsObjectFound = false;
	for (size_t i = 0; i<pRoom->m_ObjectsInRoom.size(); ++i)
	{
		std::string upperCaseObject = pRoom->m_ObjectsInRoom[i]->m_Name;

		std::for_each(upperCaseObject.begin(), upperCaseObject.end(), [](char &s) {s = toupper(s); });

		if (upperCaseObject == objectString)
		{
			IsObjectFound = true;
			std::cout << "You found:" << std::endl;
			SetConsoleTextAttribute(m_hConsole, 14);
			player .m_inventory.MergeInventories(&(pRoom->m_ObjectsInRoom[i]->m_ItemsInObject));
			pRoom->m_ObjectsInRoom[i]->m_ItemsInObject.Print();
			pRoom->m_ObjectsInRoom[i]->m_ItemsInObject.Clear();
			SetConsoleTextAttribute(m_hConsole, 7);
		}
	}

	if (!IsObjectFound)
	{
		std::cout << "The object your looking for is not here or cannot be searched." << std::endl;
	}

}

void GameEventHandler::Examine(Room* pRoom, const std::string objectString)
{
	std::string examinedObject;

	for (size_t i = 0; i<pRoom->m_ObjectsInRoom.size(); ++i)
	{
		std::string upperCaseObject = pRoom->m_ObjectsInRoom[i]->m_Name;
		std::for_each(upperCaseObject.begin(), upperCaseObject.end(), [](char &s) {s = toupper(s); });

		if (upperCaseObject == objectString)
		{
			examinedObject = pRoom->m_ObjectsInRoom[i]->m_Description;
		}
	}

	for (size_t i = 0; i<pRoom->m_EntryPoints.size(); ++i)
	{
		std::string upperCaseEntry = pRoom->m_EntryPoints[i]->m_Name;
		std::for_each(upperCaseEntry.begin(), upperCaseEntry.end(), [](char &s) {s = toupper(s); });

		if (upperCaseEntry == objectString)
		{
			if(pRoom->m_EntryPoints[i]->m_FlipDescription)
			{
				examinedObject = pRoom->m_EntryPoints[i]->m_Description2;
			}
			else
			{
				examinedObject = pRoom->m_EntryPoints[i]->m_Description;
			}
		}
	}
	if (examinedObject == "")
	{
		std::cout << "The object your looking for is not here." << std::endl;
	} else
	{
		std::cout << examinedObject << std::endl;
	}
}

//Opens the door if the door is unlocked or if the player has the required key and go through it
void GameEventHandler::Open(Room* pRoom, Player& player, const std::string objectString) const
{
	bool IsDoorFound = false;

	for (size_t i = 0; i<pRoom->m_EntryPoints.size(); ++i)
	{
		std::string upperCaseEntrypoint = pRoom->m_EntryPoints[i]->m_Name;
		std::for_each(upperCaseEntrypoint.begin(), upperCaseEntrypoint.end(), [](char &s) {s = toupper(s); });

		if (upperCaseEntrypoint == objectString)
		{
			IsDoorFound = true;
			EntryPoint* entry = pRoom->m_EntryPoints.at(i);
			entry->m_FlipDescription = !entry->m_FlipDescription;
			int flag = player.Traverse(entry);
			SetConsoleTextAttribute(m_hConsole, 14);

			switch (flag)
			{
			default:
			case 0:
				std::cout << "The " << entry->m_Name << " is locked." << std::endl;
				break;
			case 1:
				std::cout << "You unlock the " << entry->m_Name << " and go through it." << std::endl;
				break;
			case 2:
				std::cout << "You go through the " << entry->m_Name << "." << std::endl;
				break;
			}
			SetConsoleTextAttribute(m_hConsole, 7);
		}
	}

	if(!IsDoorFound)
		std::cout << "The door you're looking for is not here." << std::endl;

}

void GameEventHandler::Look(Room* pRoom) const
{
	std::cout << "You look around and see: " << std::endl;
	SetConsoleTextAttribute(m_hConsole, 14);
	std::for_each(pRoom->m_ObjectsInRoom.begin(), pRoom->m_ObjectsInRoom.end(), [](const Objects* object) {std::cout << "* " << object->m_Name << std::endl; });
	std::for_each(pRoom->m_EntryPoints.begin(), pRoom->m_EntryPoints.end(), [](const EntryPoint* entry) {std::cout << "* " << entry->m_Name << std::endl; });
	SetConsoleTextAttribute(m_hConsole, 7);
}

void GameEventHandler::ExecuteCommand(Room* pRoom, Player& player, const std::string objectString) const
{
	switch (m_CurrentCommand)
	{
	case Commands::LOOK:
	{
		Look(pRoom);
		break;
	}
	case Commands::OPEN:
	{
		Open(pRoom, player, objectString);
		break;
	}
	case Commands::SEARCH:
	{
		Search(pRoom, player, objectString);
		break;
	}
	case Commands::EXAMINE:
	{
		Examine(pRoom, objectString);
		break;
	}
	case Commands::MENU:
	{
		ShowMenu(player);
		break;
	}
	case Commands::STORY:
	{
		std::cout << pRoom->m_Story << std::endl;
		break;
	}
	case Commands::HELP:
	{
		std::cout << "All available commands are:\n"
			"HELP: prints all the available commands and how to use them.\n"
			"STORY: prints the story that belongs to the room again.\n"
			"WHERE: tells you in which room you are currently in.\n"
			"LOOK: tells you every item you see in the room.\n"
			"SEARCH: you can search an object for items.\n"
			"OPEN: used to open doors.\n"
			"Menu: opens the menu. you can see your inventory and switch from weapons,armor."
			<< std::endl;
		break;
	}
	case Commands::WHERE:
	{
		std::cout << pRoom->m_Description << std::endl;
		break;
	}
	case Commands::INVALID:
	{
		std::cout << "Unknown command" << std::endl;
		break;
	}
	default:
		std::cout << "Unknown command" << std::endl;
		break;
	}
}
