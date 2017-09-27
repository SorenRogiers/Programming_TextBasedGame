#include "stdafx.h"
#include "LivingEntity.h"

#include <string>
#include <cstdlib>
#include <JsonBox.h>
#include "JsonEntity.h"
#include "Inventory.h"
#include "EntryPoint.h"
#include "Room.h"
#include "JsonEntity.h"
#include "Weapon.h"
#include "Armor.h"


LivingEntity::LivingEntity(std::string id, std::string name, int hp,int attackPower,int agility,float dodgeChance):JsonEntity(id),m_Name(name),m_CurrentHp(hp),m_MaxHP(hp),m_AttackPower(attackPower),m_Agility(agility),m_DodgeChance(dodgeChance)
{
	
}

LivingEntity::LivingEntity(std::string id, JsonBox::Value& v, JsonEntityManager* mgr):LivingEntity(m_ID,"",0,0,0,0)
{
	this->Load(v, mgr);
}

LivingEntity::~LivingEntity()
{
	m_pCurrentWeapon = nullptr;
	m_pCurrentArmor = nullptr;

	delete m_pCurrentWeapon;
	delete m_pCurrentArmor;
}

void LivingEntity::EquipArmor(Armor* armor)
{
	m_pCurrentArmor = armor;
}

void LivingEntity::EquipWeapon(Weapon* weapon)
{
	m_pCurrentWeapon = weapon;
}

Room* LivingEntity::GetCurrentPosition(JsonEntityManager* mgr) const
{
	return mgr->GetEntity<Room>(this->m_CurrentPosition);
}
//traverse requires a Door that the entity is attempting to travel through.
//First it checks the locked property of the door.If the door is
//closed but unlocked or locked but the Creature has the key then it is opened.Once the door is opened
//the Creature is moved to the other Area linked to by the door.
int LivingEntity::Traverse(EntryPoint* entry) 
{
	auto flag = 2;

	if(entry->m_Locked ==0)
	{
		entry->m_Locked = -1;
		flag = 2;
	} 
	else if (entry->m_Locked>0)
	{
		if(m_inventory.Count(entry->m_Key))
		{
			entry->m_Locked = -1;
			flag = 1;
		} else
		{
			return 0;
		}
	}

	if(entry->m_Rooms.first == this->m_CurrentPosition)
	{
		m_CurrentPosition = entry->m_Rooms.second;
	} 
	else if(entry->m_Rooms.second == m_CurrentPosition)
	{
		m_CurrentPosition = entry->m_Rooms.first;
	}

	return flag;
}

JsonBox::Object LivingEntity::ToJsonObject()
{
	JsonBox::Object jsonObject;
	jsonObject["name"] = JsonBox::Value(m_Name);
	jsonObject["hp"] = JsonBox::Value(m_CurrentHp);	
	jsonObject["max_hp"] = JsonBox::Value(m_MaxHP);
	jsonObject["friendly"] = JsonBox::Value(m_IsFriendly);
	jsonObject["text"] = JsonBox::Value(m_EntityText);
	jsonObject["attackpower"] = JsonBox::Value(m_AttackPower);
	jsonObject["agility"] = JsonBox::Value(m_Agility);
	jsonObject["dodgechance"] = JsonBox::Value(m_DodgeChance);
	jsonObject["inventory"] = JsonBox::Value(m_inventory.GetJson());
	jsonObject["currentweapon"] = JsonBox::Value(m_pCurrentWeapon == nullptr ? "nullptr" : m_pCurrentWeapon->m_ID);
	jsonObject["currentarmor"] = JsonBox::Value(m_pCurrentArmor == nullptr ? "nullptr" : m_pCurrentArmor->m_ID);

	return jsonObject;
}

void LivingEntity::Load(JsonBox::Value& v, JsonEntityManager* mgr)
{
	JsonBox::Object jsonObject = v.getObject();
	m_Name = jsonObject["name"].getString();
	m_CurrentHp = jsonObject["hp"].getInteger();
	
	if(jsonObject.find("max_hp") != jsonObject.end())
	{
		m_MaxHP = jsonObject["max_hp"].getInteger();
	} else
	{
		m_MaxHP = m_CurrentHp;
	}
	int friendly = 0;
	friendly = jsonObject["friendly"].getInteger();

	if (friendly == 0)
		m_IsFriendly = false;
	else
		m_IsFriendly = true;

	m_EntityText = jsonObject["text"].getString();
	m_AttackPower = jsonObject["attackpower"].getInteger();
	m_Agility = jsonObject["agility"].getInteger();
	m_DodgeChance = jsonObject["dodgechance"].getFloat();

	if(jsonObject.find("inventory") != jsonObject.end())
	{
		m_inventory = Inventory(jsonObject["inventory"], mgr);
	}

	if (jsonObject.find("currentweapon") != jsonObject.end())
	{
		std::string currentWeaponName = jsonObject["currentweapon"].getString();
		m_pCurrentWeapon = currentWeaponName == "nullptr" ? nullptr : mgr->GetEntity<Weapon>(currentWeaponName);

	}

	if (jsonObject.find("currentarmor") != jsonObject.end())
	{
		std::string currentArmorName = jsonObject["currentArmor"].getString();
		m_pCurrentArmor = currentArmorName == "nullptr" ? nullptr : mgr->GetEntity<Armor>(currentArmorName	);
	}
}
int LivingEntity::Attack(LivingEntity* target) const
{
	// Damage done
	int damage = 0;

	if (double(std::rand()) / RAND_MAX  > target->m_DodgeChance)
	{
		// Calculate attack based on strength and weapon damage
		int attack = m_AttackPower + (m_pCurrentWeapon == nullptr ? 0 : m_pCurrentWeapon->m_Damage);
		// Calculate defense based on agility and armor defense
		int defense = target->m_Agility + (target->m_pCurrentArmor == nullptr ? 0 : target->m_pCurrentArmor->m_Defense);
		// 1/32 chance of a critical hit
		if (std::rand() % 32 == 0)
		{
			// Ignore defense and do damage in range [attack/2, attack]
			damage = attack / 2 + std::rand() % (attack / 2 + 1);
		}
		else
		{
			// Normal hit so factor in defense
			int baseDamage = attack - defense / 2;
			// Do damage in range [baseDamage/4, baseDamage/2]
			damage = baseDamage / 4 + std::rand() % (baseDamage / 4 + 1);
			// If the damage is zero then have a 50% chance to do 1 damage
			if (damage < 1)
			{
				damage = std::rand() % 2;
			}
		}
		// Damage the target
		target->m_CurrentHp -= damage;
	}

	return damage;
}

int LivingEntity::UseItem()
{
	int healAmount = 0;

	std::vector<Item*> edibleItems = m_inventory.FilterEdibleItems();
	std::vector<std::string> itemNames;
	
	for(auto i : edibleItems)
	{
		itemNames.push_back(i->m_Name);
	}

	int pickedItem = 0;
	
	if(edibleItems.size()>0)
	{
		pickedItem = GameEventHandler("Which item do you want to use?", itemNames, true).PickOption();
		
		healAmount = edibleItems[pickedItem - 1]->m_HealthRestore;
		
		m_CurrentHp += healAmount;

		if(m_CurrentHp >=m_MaxHP)
		{
			int overheal = m_CurrentHp - m_MaxHP;
			healAmount -= overheal;
			m_CurrentHp = m_MaxHP;
		}
		m_inventory.Remove(edibleItems[pickedItem - 1], 1);
	}
	else
	{
		std::cout << "No items for you to use during battle." << std::endl;
	}
	
	return healAmount;
}
