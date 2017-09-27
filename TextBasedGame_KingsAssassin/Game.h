#pragma once
#include "Windows.h"
#include "Player.h"

class Game
{
public:
	Game();
	~Game();
	void Start();
	void Update();
private:
	Player CreatePlayer();
	void Intro() const;
	void LoadJsonFiles();

private:
	HANDLE m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	Player m_pPlayer;
	Room* m_pRoom = nullptr;
	JsonEntityManager m_pJsonEntityManager;

};

