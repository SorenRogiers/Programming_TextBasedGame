//*************************************************************************************
//Class to hold the objects that the players finds during his adventure
//Methods to add or remove objects from the inventory
//We can merge inventories since rooms,objects or entities can have an inventory as well
//**************************************************************************************
#pragma once

#include <list>
#include <utility>
#include <JsonBox.h>

#include "JsonEntityManager.h"

class Item;
class Weapon;
class Armor;

class Inventory
{
public:
	Inventory(JsonBox::Value& v, JsonEntityManager* mgr);
	Inventory();
	~Inventory();

	void Add(Item* item, int count);
	void Remove(Item* item, int count);

	int Count(Item* item);

	std::vector<Item*> FilterEdibleItems();

	template <typename T>
	int Count(unsigned int index);

	template <typename T>
	T* GetItemByIndex(unsigned int index);

	template <typename T>
	int Print(bool labeled = false);

	void Clear();

	void MergeInventories(Inventory* inventory);

	int Print(bool label = false);

	JsonBox::Object GetJson();

	bool CheckForWinningItem();
private:
	template <typename T> 
	void Load(JsonBox::Value &v, JsonEntityManager* mgr);

	template <typename T>
	JsonBox::Array ToJsonArray();

private:
	//Using a list and not a vector as inventories are highly mutable.
	//The first element in the pair stores a pointer to the item in the JsonEntityManager
	//The second element stores the quantity of the item
	std::list<std::pair<Item*, int>> m_Items;
};

