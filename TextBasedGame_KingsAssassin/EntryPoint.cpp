#include "stdafx.h"
#include "EntryPoint.h"

#include "Item.h"
#include "JsonEntity.h"
#include "JsonEntityManager.h"


EntryPoint::EntryPoint(std::string id, std::string description, std::pair<std::string, std::string> rooms, int locked, Item* key) : JsonEntity(id),
	m_Description(description), m_Rooms(rooms), m_Locked(locked), m_Key(key)
{

}

EntryPoint::EntryPoint(std::string id, JsonBox::Value& v, JsonEntityManager* mgr) :JsonEntity(id)
{
	Load(v, mgr);
}

EntryPoint::~EntryPoint()
{
	m_Key = nullptr;
	delete m_Key;
}

void EntryPoint::Load(JsonBox::Value& v, JsonEntityManager* mgr)
{
	JsonBox::Object o = v.getObject();
	m_Name = o["name"].getString();
	m_Description = o["description"].getString();
	m_Description2 = o["description2"].getString();
	m_Locked = o["locked"].getInteger();
	
	if(o.find("key") != o.end())
	{
		m_Key = mgr->GetEntity<Item>(o["key"].getString());
	}
	JsonBox::Array a = o["rooms"].getArray();
	if(a.size()==2)
	{
		m_Rooms.first = a[0].getString();
		m_Rooms.second = a[1].getString();
	}
}
