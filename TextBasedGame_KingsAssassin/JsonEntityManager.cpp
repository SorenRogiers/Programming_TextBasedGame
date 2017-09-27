#include "stdafx.h"
#include <string>
#include <map>

#include "JsonEntityManager.h"
#include "JsonEntity.h"
#include "Item.h"
#include "Room.h"
#include "EntryPoint.h"
#include "LivingEntity.h"
#include "Objects.h"
#include "Weapon.h"
#include "Armor.h"

JsonEntityManager::JsonEntityManager() {}

JsonEntityManager::~JsonEntityManager()
{
	for (auto& entity : m_Data)
	{
		delete entity.second;
	}
}

template <class T>
void JsonEntityManager::LoadJson(std::string filename)
{
	JsonBox::Value v;
	v.loadFromFile(filename);

	JsonBox::Object o = v.getObject();
	for (auto entity : o)
	{
		std::string key = entity.first;
		m_Data[key] = dynamic_cast<JsonEntity*>(new T(key, entity.second, this));
	}
}

template <class T>
T* JsonEntityManager::GetEntity(std::string id)
{
	// The id prefix should match to the type T, so take the
	// first characters of the id up to the length of the
	// prefix and compare the two
	if (id.substr(0, EntityToString<T>().size()) == EntityToString<T>())
		return dynamic_cast<T*>(m_Data.at(id));
	else
		return nullptr;
}

template <> std::string EntityToString<Item>() { return "item"; }
template <> std::string EntityToString<Weapon>() { return "weapon"; }
template <> std::string EntityToString<Armor>() { return "armor"; }
template <> std::string EntityToString<Objects>() { return "object"; }
template <> std::string EntityToString<LivingEntity>() { return "livingentity"; }
template <> std::string EntityToString<Room>() { return "room"; }
template <> std::string EntityToString<EntryPoint>() { return "entrypoint"; }

template void JsonEntityManager::LoadJson<Item>(std::string);
template void JsonEntityManager::LoadJson<Objects>(std::string);
template void JsonEntityManager::LoadJson<Weapon>(std::string);
template void JsonEntityManager::LoadJson<Armor>(std::string);
template void JsonEntityManager::LoadJson<LivingEntity>(std::string);
template void JsonEntityManager::LoadJson<Room>(std::string);
template void JsonEntityManager::LoadJson<EntryPoint>(std::string);

template Item* JsonEntityManager::GetEntity<Item>(std::string);
template Weapon* JsonEntityManager::GetEntity<Weapon>(std::string);
template Armor* JsonEntityManager::GetEntity<Armor>(std::string);
template Objects* JsonEntityManager::GetEntity<Objects>(std::string);
template LivingEntity* JsonEntityManager::GetEntity<LivingEntity>(std::string);
template Room* JsonEntityManager::GetEntity<Room>(std::string);
template EntryPoint* JsonEntityManager::GetEntity<EntryPoint>(std::string);