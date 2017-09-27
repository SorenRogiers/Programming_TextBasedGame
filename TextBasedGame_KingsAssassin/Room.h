//**************************************
//Class to hold information about a room
//-Connections to other rooms
//-Entities in the room
//-Story that belongs to the room
//-Objects that are in the room
//**************************************
#pragma once
#include <vector>
#include <string>
#include <JsonBox.h>

#include "JsonEntity.h"
#include "Inventory.h"
#include "GameEventHandler.h"
#include "Objects.h"
#include "LivingEntity.h"

class JsonEntityManager;
class EntryPoint;

class Room :public JsonEntity
{
public:
	Room(std::string id, GameEventHandler textChoices, Inventory items,std::vector<LivingEntity*> enemies);
	Room(std::string id, JsonBox::Value& v, JsonEntityManager *mgr);
	~Room();

	void Load(JsonBox::Value& v, JsonEntityManager* mgr) override;
	JsonBox::Object GetJsonObject();

	GameEventHandler m_TextChoices;
	Inventory m_ItemsInRoom;
	std::string m_Name;
	std::string m_Description;
	std::string m_Story;
	std::vector<EntryPoint*> m_EntryPoints;
	std::vector<Objects*> m_ObjectsInRoom;

	std::vector<LivingEntity> m_EnemiesInRoom;
};

