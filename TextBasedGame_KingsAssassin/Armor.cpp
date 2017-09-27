#include "stdafx.h"
#include "Armor.h"

Armor::Armor(std::string id, std::string name, std::string description, int defense) : Item(id, name, description), m_Defense(defense)
{
}

Armor::Armor(std::string id, JsonBox::Value& value, JsonEntityManager* manager) : Item(id,value,manager)
{
	Load(value, manager);
}

Armor::~Armor()
{
}

void Armor::Load(JsonBox::Value& value, JsonEntityManager* manager)
{
	JsonBox::Object jsonObject = value.getObject();
	m_Defense = jsonObject["defense"].getInteger();
}
