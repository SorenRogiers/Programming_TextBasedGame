#include "stdafx.h"
#include <cstdlib>
#include <ctime>
#include <JsonBox.h>
#include "Windows.h"
#include "Game.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Game* pTextBasedGame = new Game();
	pTextBasedGame->Start();
	pTextBasedGame->Update();
}

////Intro
//void PrintIntro();
////Game
//Player StartGame();
//void LoadJsonFiles();
//
//JsonEntityManager jsonEntityManager;
//Player player;
//HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//
//int main()
//{
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//
//	LoadJsonFiles();
//	std::srand(static_cast<unsigned int>(std::time(nullptr)));
//
//	PrintIntro();
//
//	player = StartGame();
//	Room* pRoom = nullptr;
//	player.m_CurrentPosition = "room_cell";
//
//	//Game Loop
//	while(true)
//	{
//		pRoom = player.GetCurrentPosition(&jsonEntityManager);
//		GameEventHandler roomDialogueOptions = pRoom->m_TextChoices;
//
//		//Print the story that belongs to the room if you never entered the room before
//		if(!player.PlayerVisted(pRoom))
//		{
//			roomDialogueOptions.SlowPrint(pRoom->m_Story, 15);
//		}
//
//		//Add the current position to the rooms visited.
//		player.m_RoomsVisited.insert(player.m_CurrentPosition);
//		
//		//Get the text options for the room in the json file.
//
//		//For every door we find we add an option.
//		for(auto entry : pRoom->m_EntryPoints)
//		{
//			roomDialogueOptions.AddChoice("OPEN " + entry->m_Description);
//		}
//
//		//For every object we find in the room we add an option
//		for (auto object : pRoom->m_ObjectsInRoom)
//		{
//			roomDialogueOptions.AddChoice("EXAMINE " + object->m_Name);
//		}
//
//		//Attack the player when he enters a room if there are enemies in it
//		roomDialogueOptions.AttackPlayer(pRoom, player);
//
//		//Process the input with the options.
//		std::string command;
//		std::cin.clear();
//		SetConsoleTextAttribute(hConsole, 10);
//		std::getline(std::cin, command);
//		SetConsoleTextAttribute(hConsole, 7);
//		std::cout << std::endl;
//		if(command != "")
//		{
//			roomDialogueOptions.ProcessCommand(command, pRoom, player);
//		}
//	}
//
//}
//
//
//
//Player StartGame()
//{
//	std::cout << "What's your name?" << std::endl;
//	std::string name;
//	std::cin.clear();
//	SetConsoleTextAttribute(hConsole, 10);
//	std::getline(std::cin, name);
//	SetConsoleTextAttribute(hConsole, 7);
//
//	std::cout << std::endl;
//	//std::cin >> name;
//
//	std::ifstream f((name + ".json").c_str());
//	if(f.good())
//	{
//		f.close();
//
//		JsonBox::Value savedPlayerData;
//		JsonBox::Value savedRoomData;
//		savedPlayerData.loadFromFile(name + ".json");
//		savedRoomData.loadFromFile(name + "_rooms.json");
//		Player player = Player(savedPlayerData, savedRoomData, &jsonEntityManager);
//		
//		return player;
//	}
//	else
//	{
//		f.close();
//		int result = GameEventHandler("Choose your class", { "Warrior","Rogue" }, true).PickOption();
//		switch (result)
//		{
//		case 1: 
//			return Player(name, 100,7, 3, 1.0f / 64.0f, "Warrior");
//		case 2:
//			return Player(name, 100, 5, 5, 1.0f / 64.0f, "Rogue");
//		default:
//			return Player(name, 15, 0, 0, 1.0f / 64.0f, "Invalid");
//		}
//	}
//
//}
//
//void LoadJsonFiles()
//{
//	//Load Files
//	jsonEntityManager.LoadJson<Item>("Resources/items.json");
//	jsonEntityManager.LoadJson<Objects>("Resources/objects.json");
//	jsonEntityManager.LoadJson<Weapon>("Resources/weapons.json");
//	jsonEntityManager.LoadJson<Armor>("Resources/armor.json");
//	jsonEntityManager.LoadJson<LivingEntity>("Resources/livingentities.json");
//	jsonEntityManager.LoadJson<EntryPoint>("Resources/entrypoints.json");
//	jsonEntityManager.LoadJson<Room>("Resources/rooms.json");
//}
//void PrintIntro()
//{
//	SetConsoleTextAttribute(hConsole, 8);
//
//	std::vector<std::string> art =
//	{
//		R"(/////////////////////////////////////////////////////////////////////)",
//		R"(//  _  ___                      _                          _       //)",
//		R"(// | |/ (_)_ __   __ _ ___     / \   ___ ___  __ _ ___ ___(_)_ __  //)",
//		R"(// | ' /| | '_ \ / _` / __|   / _ \ / __/ __|/ _` / __/ __| | '_ \ //)",
//		R"(// | . \| | | | | (_| \__ \  / ___ \\__ \__ \ (_| \__ \__ \ | | | |//)",
//		R"(// |_|\_\_|_| |_|\__, |___/ /_/   \_\___/___/\__,_|___/___/_|_| |_|//)",
//		R"(//               |___/                                             //)",
//		R"(/////////////////////////////////////////////////////////////////////)",
//	};
//	SetConsoleTextAttribute(hConsole, 7);
//	for (const auto& line : art) std::cout << '\t'<< '\t' << '\t' << line << '\n';
//	std::cout << '\n';
//
//	std::cout << "All available commands are:\n"
//		"HELP: prints all the available commands and how to use them.\n"
//		"STORY: prints the story that belongs to the room again.\n"
//		"WHERE: tells you in which room you are currently in.\n"
//		"LOOK: tells you every item you see in the room.\n"
//		"SEARCH: you can search an object for items.\n"
//		"OPEN: used to open doors.\n"
//		"Menu: opens the menu. you can see your inventory and switch from weapons,armor."
//		"Command example: 'Search Dead guard'."
//		<< std::endl << std::endl;
//}
