//*******************************************************************************************
//Class to hold information about livingEntities such as player and enemies
//We hold information such as name,currentposition,hp,attack power, agility, dodgechance etc
//An entity has the ability to equip armor and weapons
//There are methods to attack, use items and travel through the world
//*******************************************************************************************
#pragma once

#include <string>
#include <cstdlib>
#include <JsonBox.h>

#include "JsonEntity.h"
#include "Inventory.h"

class Room;
class JsonEntityManager;
class EntryPoint;
class Weapon;
class Armor;

class LivingEntity : public JsonEntity
{
public:
	
	LivingEntity(std::string id,std::string name,int hp,int attackPower,int agility,float dodgeChance);
	LivingEntity(std::string id, JsonBox::Value &v, JsonEntityManager* mgr);

	~LivingEntity();

	void EquipArmor(Armor* armor);
	void EquipWeapon(Weapon* weapon);

	Room* GetCurrentPosition(JsonEntityManager* mgr) const;
	int Traverse(EntryPoint* entry);
	int Attack(LivingEntity* target) const;
	int UseItem();

	virtual JsonBox::Object ToJsonObject();
	virtual void Load(JsonBox::Value& v, JsonEntityManager* mgr)override;

	//Members
	std::string m_Name;
	std::string m_CurrentPosition;
	std::string m_EntityText;

	bool m_IsFriendly = false;
	int m_MaxHP = 0;
	int m_CurrentHp = 0;
	int m_AttackPower = 0;
	int m_Agility = 0;
	float m_DodgeChance = 0.0f;

	Inventory m_inventory;

	Weapon* m_pCurrentWeapon = nullptr;
	Armor* m_pCurrentArmor = nullptr;
};

