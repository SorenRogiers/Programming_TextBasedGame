#include "stdafx.h"
#include "Weapon.h"

Weapon::Weapon(std::string id, std::string name, std::string descriptiom, int damage) : Item(id,name,descriptiom),m_Damage(damage)
{

}

Weapon::Weapon(std::string id, JsonBox::Value& value, JsonEntityManager* manager):Item(id,value,manager)
{
	Load(value, manager);
}

Weapon::~Weapon()
{
}

void Weapon::Load(JsonBox::Value& value, JsonEntityManager* manager)
{
	JsonBox::Object jsonObject = value.getObject();
	m_Damage = jsonObject["damage"].getInteger();
	
	m_Description = jsonObject["description"].getString();
	m_EdibleString = jsonObject["edible"].getString();

	if (m_EdibleString == "true")
	{
		m_IsEdible = true;
	}
	m_HealthRestore = jsonObject["healthrestore"].getInteger();

}
