#pragma once

#include <string>
#include <JsonBox.h>
#include "Item.h"

class JsonEntityManager;

class Armor : public Item
{
public:
	Armor(std::string id,std::string name,std::string description,int defense);
	Armor(std::string id, JsonBox::Value &value, JsonEntityManager* manager);
	~Armor();

	void Load(JsonBox::Value &value, JsonEntityManager* manager)override;

	int m_Defense = 0;
};

