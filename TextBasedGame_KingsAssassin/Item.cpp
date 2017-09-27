#include "stdafx.h"
#include <string>
#include <JsonBox.h>

#include "JsonEntityManager.h"
#include "Item.h"

Item::Item(std::string id, std::string name, std::string description) : JsonEntity(id)
{
	m_Name = name;
	m_Description = description;
}

Item::Item(std::string id, JsonBox::Value& v, JsonEntityManager* mgr) : JsonEntity(id)
{
	Load(v, mgr);
}

void Item::Load(JsonBox::Value& v, JsonEntityManager* mgr)
{
	JsonBox::Object jsonObject = v.getObject();
	m_Name = jsonObject["name"].getString();
	m_Description = jsonObject["description"].getString();
	m_EdibleString = jsonObject["edible"].getString();
	
	if(m_EdibleString == "true")
	{
		m_IsEdible = true;
	}
	m_HealthRestore = jsonObject["healthrestore"].getInteger();

	m_Inventory = Inventory(jsonObject["inventory"], mgr);
}
