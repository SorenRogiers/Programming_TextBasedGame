#pragma once

#include <string>
#include <JsonBox.h>

#include "JsonEntity.h"
#include <list>
#include "Inventory.h"

class JsonEntityManager;

class Item : public JsonEntity
{
public:
	// Constructors
	Item(std::string id, std::string name, std::string description);
	Item(std::string id, JsonBox::Value& v, JsonEntityManager* mgr);

	// Load the item information from the JSON value
	virtual void Load(JsonBox::Value& v, JsonEntityManager* mgr)override;

	// Name and description of the item
	std::string m_Name;
	std::string m_Description;
	std::list<Item> m_Items;
	bool m_IsEdible = false;
	int m_HealthRestore = 0;
	std::string m_EdibleString;
	Inventory m_Inventory;

};
