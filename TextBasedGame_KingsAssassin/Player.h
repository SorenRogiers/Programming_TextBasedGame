#pragma once
#include <unordered_set>
#include <string>
#include <JsonBox.h>

#include "LivingEntity.h"

class JsonEntityManager;
class Room;

//Using the std::unordered_set class to store the ids
//because it ensures that each id is only stored once, and it also doesn’t care about the order the ids are entered.
class Player : public LivingEntity
{
public:
	Player(std::string name,int hp,int attackPower,int agility,float dodgeChance,std::string className);
	Player();
	Player(JsonBox::Value& savePlayerData,JsonBox::Value& saveRoomData, JsonEntityManager* mgr);

	~Player();

	JsonBox::Object ToJsonObject() override;
	void Save(JsonEntityManager* mgr);
	void Load(JsonBox::Value& savePlayerData, JsonEntityManager* mgr) override;
	void LoadRoom(JsonBox::Value& saveRoomData, JsonEntityManager* mgr);
	bool PlayerVisted(Room* pRoom);
	bool CheckPlayerHP()const;

	std::string m_ClassName;
	std::unordered_set<std::string> m_RoomsVisited;

};

