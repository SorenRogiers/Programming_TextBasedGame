#include "stdafx.h"
#include "Room.h"

#include "EntryPoint.h"
#include "JsonEntity.h"
#include "Inventory.h"
#include "GameEventHandler.h"

#include <vector>
#include <string>
#include <JsonBox.h>
#include "LivingEntity.h"

Room::Room(std::string id, GameEventHandler textChoices, Inventory items, std::vector<LivingEntity*> enemies):JsonEntity(id),m_TextChoices(textChoices),m_ItemsInRoom(items)
{
	for(auto enemy : enemies)
	{
		m_EnemiesInRoom.push_back(*enemy);
	}
}

Room::Room(std::string id, JsonBox::Value& v, JsonEntityManager* mgr) :JsonEntity(id)
{
	Load(v, mgr);
}

Room::~Room()
{
	for(size_t i=0;i<m_ObjectsInRoom.size();++i)
	{
		m_ObjectsInRoom[i] = nullptr;
	}
	m_ObjectsInRoom.clear();

	for(size_t i=0;i<m_EntryPoints.size();++i)
	{
		m_EntryPoints[i] = nullptr;
	}
	m_EntryPoints.clear();
}

void Room::Load(JsonBox::Value& v, JsonEntityManager* mgr)
{
	JsonBox::Object jsonObject = v.getObject();
	m_Name = jsonObject["name"].getString();
	m_Story = jsonObject["story"].getString();
	m_Description = jsonObject["description"].getString();

	if(jsonObject.find("dialogue") != jsonObject.end())
	{
		m_TextChoices = GameEventHandler(jsonObject["dialogue"]);
	}

	m_ItemsInRoom = Inventory(jsonObject["inventory"],mgr);
	//Find the enemiess in a room
	for(auto enemy : jsonObject["livingentities"].getArray())
	{
		LivingEntity e(*mgr->GetEntity<LivingEntity>(enemy.getString()));
		m_EnemiesInRoom.push_back(e);
	}

	//Find all the the possible connections in a room
	if(jsonObject.find("doors") != jsonObject.end())
	{
		m_EntryPoints.clear();
		for(auto entry : jsonObject["doors"].getArray())
		{
			EntryPoint* entryPoint = nullptr;
			if(entry.isString())
			{
				entryPoint = mgr->GetEntity<EntryPoint>(entry.getString());
			} 
			else
			{
				entryPoint = mgr->GetEntity<EntryPoint>(entry.getArray()[0].getString());
				entryPoint->m_Locked = entry.getArray()[1].getInteger();
			}

			m_EntryPoints.push_back(entryPoint);
		}
	}
	
	//Find all the objects in the room
	if (jsonObject.find("objects") != jsonObject.end())
	{
		m_ObjectsInRoom.clear();
		for (auto obj : jsonObject["objects"].getArray())
		{
			Objects* object = nullptr;
			if (obj.isString())
			{
				object = mgr->GetEntity<Objects>(obj.getString());
			}
			else
			{
				object = mgr->GetEntity<Objects>(obj.getArray()[0].getString());
			}

			m_ObjectsInRoom.push_back(object);
		}
	}
}

JsonBox::Object Room::GetJsonObject()
{
	JsonBox::Object jsonObject;

	jsonObject["inventory"] = m_ItemsInRoom.GetJson();

	JsonBox::Array jsonArray;

	for(auto livingEntity : m_EnemiesInRoom)
	{
		jsonArray.push_back(JsonBox::Value(livingEntity.m_ID));
	}
	jsonObject["livingentities"] = jsonArray;

	jsonArray.clear();
	for (auto entry : m_EntryPoints)
	{
		JsonBox::Array d;
		d.push_back(entry->m_ID);
		d.push_back(entry->m_Locked);
		jsonArray.push_back(d);
	}
	jsonObject["doors"] = jsonArray;

	//TODO still need to test this. 
	jsonArray.clear();
	for (auto obj : m_ObjectsInRoom)
	{
		JsonBox::Array d;
		d.push_back(obj->m_ID);
		d.push_back(obj->m_Name);
		jsonArray.push_back(d);
	}
	jsonObject["objects"] = jsonArray;

	return jsonObject;
}
