#include "stdafx.h"
#include "Player.h"
#include "Room.h"

Player::Player(std::string name, int hp, int attackPower, int agility, float dodgeChance, std::string className):LivingEntity("player",name,hp,attackPower,agility,dodgeChance),m_ClassName(className)
{
}

Player::Player(JsonBox::Value& savePlayerData, JsonBox::Value& saveRoomData, JsonEntityManager* mgr) : Player()
{
	Load(savePlayerData, mgr);
	LoadRoom(saveRoomData, mgr);
}
Player::Player() : Player("",0,0,0,0.0f,"null")
{
	
}

Player::~Player()
{
}

JsonBox::Object Player::ToJsonObject()
{
	JsonBox::Object jsonObject = LivingEntity::ToJsonObject();
	jsonObject["className"] = JsonBox::Value(m_ClassName);
	
	return jsonObject;
}

void Player::Save(JsonEntityManager* mgr)
{
	JsonBox::Value playerValues = (this->ToJsonObject());
	playerValues.writeToFile("Resources/" +m_Name + ".json");

	JsonBox::Object jsonObject;
	/*for(auto room : m_RoomsVisited)
	{
		jsonObject[room] = mgr->GetEntity<Room>(room)->GetJsonObject();

	}
	JsonBox::Value playerRoomValues(jsonObject);
	playerRoomValues.writeToFile("Resources/" + m_Name + "_rooms.json");*/
}

void Player::Load(JsonBox::Value& savePlayerData, JsonEntityManager* mgr)
{
	LivingEntity::Load(savePlayerData, mgr);
	JsonBox::Object jsonObject = savePlayerData.getObject();

	m_ClassName = jsonObject["className"].getString();
}

void Player::LoadRoom(JsonBox::Value& saveRoomData, JsonEntityManager* mgr)
{
	JsonBox::Object jsonObject = saveRoomData.getObject();

	for(auto room : jsonObject)
	{
		std::string key = room.first;
		mgr->GetEntity<Room>(key)->Load(room.second, mgr);
		m_RoomsVisited.insert(key);
	}
}

bool Player::PlayerVisted(Room* pRoom)
{
	for(auto s : m_RoomsVisited)
	{
		if (pRoom->m_ID == s)
			return true;
	}

	return false;
}

bool Player::CheckPlayerHP() const
{
	//If the players hp reaches zero he dies.
	if (m_CurrentHp <= 0)
	{
		std::cout << "You died!";
		return true;
	}
	return false;
}

