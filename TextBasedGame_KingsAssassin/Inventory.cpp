#include "stdafx.h"
#include "Inventory.h"
#include "Item.h"
#include "JsonBox.h"

#include <list>
#include <string>
#include <utility>
#include <iostream>
#include "Weapon.h"
#include "Armor.h"

Inventory::Inventory(JsonBox::Value& v, JsonEntityManager* mgr)
{
	JsonBox::Object o = v.getObject();
	Load<Item>(o["items"], mgr);
	Load<Weapon>(o["weapons"], mgr);
	Load<Armor>(o["armor"], mgr);

}

Inventory::Inventory()
{

}


Inventory::~Inventory()
{
	m_Items.clear();
}
//Add takes a pointer to an item and a quantity and checks to see if the inventory
//already contains an item with the same id as the given item pointer
//if it does it increases the quantity of the item otherwise the inventory doesn't contain
//the item yet and creates a pair
void Inventory::Add(Item* item, int count)
{
	for(auto& it : this->m_Items)
	{
		if(it.first->m_ID == item->m_ID)
		{
			it.second += count;
			return;
		}
	}
	this->m_Items.push_back(std::make_pair(item, count));
}

//Remove: in order to remove an item from a list we need to use the erase function
//and this takes an iterator
void Inventory::Remove(Item* item, int count)
{
	for(auto it = this->m_Items.begin(); it != this->m_Items.end();++it)
	{
		if((*it).first->m_ID == item->m_ID)
		{
			(*it).second -= count;

			if ((*it).second < 1) 
				this->m_Items.erase(it);

			return;
		}
	}
}

//This count returns the quantity of the item that the inventory contains by finding the first pair
//whose ID's match
int Inventory::Count(Item* item)
{
	for(auto it : this->m_Items)
	{
		if(it.first->m_ID == item->m_ID)
		{
			return it.second;
		}
	}
	return 0;
}

std::vector<Item*> Inventory::FilterEdibleItems()
{
	std::vector<Item*> edibleItems;
	for(auto it : m_Items)
	{
		if(it.first->m_IsEdible == 1)
		{
			edibleItems.push_back(it.first);
		}
	}
	return edibleItems;
}

//This count function uses the GetItemByIndex function to find a pointer of type T* to the n item, which is then
//converted to an Item* pointer before being sent to the first count function.
template <typename T>
int Inventory::Count(unsigned int index)
{
	return Count(GetItemByIndex<T>(index));
}

//The GetItemByIndex function returns a T* pointer to the x'th element in the item list that is of the same type as T
//std::list doesn't have an element access unlike a vector which does.
template <typename T>
T* Inventory::GetItemByIndex(unsigned int index)
{
	unsigned int i = 0;
	auto it = m_Items.begin();
	for(;it != m_Items.end();++it)
	{
		if((*it).first->m_ID.substr(0,EntityToString<T>().size()) != EntityToString<T>())
		{
			continue;
		}

		if (i++ == index)
			break;
	}

	if (it != this->m_Items.end())
	{
		return dynamic_cast<T*>((*it).first);
	}
	else
	{
		return nullptr;
	}
}

template <typename T>
int Inventory::Print(bool labeled)
{
	unsigned int i = 1;

	for(auto it : this->m_Items)
	{
		if (it.first->m_ID.substr(0, EntityToString<T>().size()) != EntityToString<T>())
			continue;

		if(labeled)
		{
			std::cout << i++ << ": ";
		}

		std::cout << it.first->m_Name << " (" << it.second << ") - ";
		std::cout << it.first->m_Description << std::endl;
	}
	return this->m_Items.size();
}

template <typename T>
void Inventory::Load(JsonBox::Value& v, JsonEntityManager* mgr)
{
	for(auto item : v.getArray())
	{
		std::string itemID = item.getArray()[0].getString();
		int quantity = item.getArray()[1].getInteger();
		m_Items.push_back(std::make_pair(mgr->GetEntity<T>(itemID), quantity));
	}
}

template <typename T>
JsonBox::Array Inventory::ToJsonArray()
{
	JsonBox::Array jsonArray;
	for(auto item : this->m_Items)
	{
		if (item.first->m_ID.substr(0, EntityToString<T>().size()) != EntityToString<T>())
			continue;

		JsonBox::Array pair;
		pair.push_back(JsonBox::Value(item.first->m_ID));
		pair.push_back(JsonBox::Value(item.second));

		jsonArray.push_back(JsonBox::Value(pair));
	}

	return jsonArray;
}

void Inventory::Clear()
{
	m_Items.clear();
}

void Inventory::MergeInventories(Inventory* inventory)
{
	if (inventory == this)
		return;

	for(auto it : inventory->m_Items)
	{
		this->Add(it.first, it.second);
	}
	
}

int Inventory::Print(bool label)
{
	unsigned int i = 0;
	if(m_Items.empty())
	{
		std::cout << "Nothing to be found." << std::endl;
	} else
	{
		i += Print<Item>(label);
		i += Print<Weapon>(label);
		i += Print<Armor>(label);
	}

	return i;
}

JsonBox::Object Inventory::GetJson()
{
	JsonBox::Object jsonObject;

	jsonObject["items"] = JsonBox::Value(ToJsonArray<Item>());
	jsonObject["weapons"] = JsonBox::Value(ToJsonArray<Weapon>());
	jsonObject["armor"] = JsonBox::Value(ToJsonArray<Armor>());

	return jsonObject;
}

bool Inventory::CheckForWinningItem()
{
	for(auto it : m_Items)
	{
		if (it.first->m_Name == "Magical letter")
			return true;
	}

	return false;
}

template void Inventory::Load<Item>(JsonBox::Value&, JsonEntityManager*);
template void Inventory::Load<Weapon>(JsonBox::Value&, JsonEntityManager*);
template void Inventory::Load<Armor>(JsonBox::Value&, JsonEntityManager*);

template JsonBox::Array Inventory::ToJsonArray<Item>();
template JsonBox::Array Inventory::ToJsonArray<Weapon>();
template JsonBox::Array Inventory::ToJsonArray<Armor>();

template int Inventory::Count<Item>(unsigned int);
template int Inventory::Count<Weapon>(unsigned int);
template int Inventory::Count<Armor>(unsigned int);

template Item* Inventory::GetItemByIndex<Item>(unsigned int);
template Weapon* Inventory::GetItemByIndex<Weapon>(unsigned int);
template Armor* Inventory::GetItemByIndex<Armor>(unsigned int);

template int Inventory::Print<Item>(bool);
template int Inventory::Print<Weapon>(bool);
template int Inventory::Print<Armor>(bool);

