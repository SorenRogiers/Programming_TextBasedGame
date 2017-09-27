#include "stdafx.h"
#include "Game.h"
#include <ctime>
#include <JsonBox.h>

#include "EntryPoint.h"
#include "Room.h"
#include "Item.h"
#include "JsonEntityManager.h"
#include "Player.h"
#include <iostream>
#include <fstream>
#include "GameEventHandler.h"
#include "Weapon.h"
#include "Armor.h"
#include "Windows.h"

Game::Game()
{
}


Game::~Game()
{
	m_pRoom = nullptr;
	delete m_pRoom;
}

void Game::Start()
{
	LoadJsonFiles();
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	Intro();

	m_pPlayer = CreatePlayer();
	m_pPlayer.m_CurrentPosition = "room_cell";
}


void Game::Update()
{
	//Game Loop
	while (true)
	{
		m_pRoom = m_pPlayer.GetCurrentPosition(&m_pJsonEntityManager);
		GameEventHandler roomDialogueOptions = m_pRoom->m_TextChoices;

		//Attack the player when he enters a room if there are enemies in it
		roomDialogueOptions.AttackPlayer(m_pRoom, m_pPlayer);

		//Print the story that belongs to the room if you never entered the room before
		if (!m_pPlayer.PlayerVisted(m_pRoom))
		{
			roomDialogueOptions.SlowPrint(m_pRoom->m_Story, 15);
		}

		//Add the current position to the rooms visited.
		m_pPlayer.m_RoomsVisited.insert(m_pPlayer.m_CurrentPosition);

		//Get the text options for the room in the json file.

		//For every door we find we add an option.
		for (auto entry : m_pRoom->m_EntryPoints)
		{
			roomDialogueOptions.AddChoice("OPEN " + entry->m_Description);
		}

		//For every object we find in the room we add an option
		for (auto object : m_pRoom->m_ObjectsInRoom)
		{
			roomDialogueOptions.AddChoice("EXAMINE " + object->m_Name);
		}
		
		if (m_pPlayer.m_inventory.CheckForWinningItem())
			std::cout << "Congratulations you finished the game, have fun running around!" << std::endl;

		//Process the input with the options.
		std::string command;
		std::cin.clear();
		SetConsoleTextAttribute(m_hConsole, 10);
		std::getline(std::cin, command);
		SetConsoleTextAttribute(m_hConsole, 7);
		std::cout << std::endl;
		if (command != "")
		{
			roomDialogueOptions.ProcessCommand(command, m_pRoom, m_pPlayer);
		}
	}
}

void Game::Intro() const
{
	SetConsoleTextAttribute(m_hConsole, 8);

	std::vector<std::string> art =
	{
		R"(/////////////////////////////////////////////////////////////////////)",
		R"(//  _  ___                      _                          _       //)",
		R"(// | |/ (_)_ __   __ _ ___     / \   ___ ___  __ _ ___ ___(_)_ __  //)",
		R"(// | ' /| | '_ \ / _` / __|   / _ \ / __/ __|/ _` / __/ __| | '_ \ //)",
		R"(// | . \| | | | | (_| \__ \  / ___ \\__ \__ \ (_| \__ \__ \ | | | |//)",
		R"(// |_|\_\_|_| |_|\__, |___/ /_/   \_\___/___/\__,_|___/___/_|_| |_|//)",
		R"(//               |___/                                             //)",
		R"(/////////////////////////////////////////////////////////////////////)",
	};
	SetConsoleTextAttribute(m_hConsole, 7);
	for (const auto& line : art) std::cout << '\t' << '\t' << '\t' << line << '\n';
	std::cout << '\n';

	std::cout << "All available commands are:\n"
		"HELP: prints all the available commands and how to use them.\n"
		"STORY: prints the story that belongs to the room again.\n"
		"WHERE: tells you in which room you are currently in.\n"
		"LOOK: tells you every item you see in the room.\n"
		"SEARCH: you can search an object for items.\n"
		"OPEN: used to open doors.\n"
		"Menu: opens the menu. you can see your inventory and switch from weapons,armor.\n"
		"Command example: 'Search Dead guard'."
		<< std::endl << std::endl;
}

void Game::LoadJsonFiles()
{
	//Load Files
	m_pJsonEntityManager.LoadJson<Item>("Resources/items.json");
	m_pJsonEntityManager.LoadJson<Weapon>("Resources/weapons.json");
	m_pJsonEntityManager.LoadJson<Armor>("Resources/armor.json");
	m_pJsonEntityManager.LoadJson<Objects>("Resources/objects.json");
	m_pJsonEntityManager.LoadJson<LivingEntity>("Resources/livingentities.json");
	m_pJsonEntityManager.LoadJson<EntryPoint>("Resources/entrypoints.json");
	m_pJsonEntityManager.LoadJson<Room>("Resources/rooms.json");
}

Player Game::CreatePlayer()
{
	std::cout << "What's your name?" << std::endl;
	std::string name;
	std::cin.clear();
	SetConsoleTextAttribute(m_hConsole, 10);
	std::getline(std::cin, name);
	SetConsoleTextAttribute(m_hConsole, 7);

	std::cout << std::endl;

	std::ifstream f((name + ".json").c_str());
	if (f.good())
	{
		f.close();

		JsonBox::Value savedPlayerData;
		JsonBox::Value savedRoomData;
		savedPlayerData.loadFromFile(name + ".json");
		savedRoomData.loadFromFile(name + "_rooms.json");
		Player player = Player(savedPlayerData, savedRoomData, &m_pJsonEntityManager);

		return player;
	}
	else
	{
		f.close();
		int result = GameEventHandler("Choose your class", { "Warrior","Rogue" }, true).PickOption();
		switch (result)
		{
		case 1:
			return Player(name, 100, 7, 3, 1.0f / 64.0f, "Warrior");
		case 2:
			return Player(name, 100, 5, 5, 1.0f / 64.0f, "Rogue");
		default:
			return Player(name, 15, 0, 0, 1.0f / 64.0f, "Invalid");
		}
	}
}
