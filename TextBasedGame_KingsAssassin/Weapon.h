#pragma once

#include <string>
#include <JsonBox.h>
#include "Item.h"

class JsonEntityManager;

class Weapon : public Item
{
public:
	Weapon(std::string id,std::string name,std::string descriptiom,int damage);
	Weapon(std::string id, JsonBox::Value &value, JsonEntityManager* manager);
	~Weapon();

	void Load(JsonBox::Value &value, JsonEntityManager * manager) override;

	int m_Damage = 0;
};

