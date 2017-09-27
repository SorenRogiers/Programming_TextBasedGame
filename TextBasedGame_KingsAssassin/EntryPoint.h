//******************************************************************
//Class to keep track of connections between rooms
//Load the values from the json file
//Each room has a name - description - description2
//wether it's locked or not - which key we need to unlock it
//The descriptions are used depending on which side you are standing
//******************************************************************
#pragma once
#include "JsonEntity.h"
#include <string>
#include <utility>

class Item;
class JsonEntityManager;

class EntryPoint: public JsonEntity
{
public:
	EntryPoint(std::string id , std::string description, std::pair<std::string,std::string> rooms,int locked, Item* key=nullptr);
	EntryPoint(std::string id, JsonBox::Value& v, JsonEntityManager* mgr);
	~EntryPoint();

	void Load(JsonBox::Value &v, JsonEntityManager* mgr) override;

	std::string m_Name;
	std::string m_Description;
	std::string m_Description2;
	int m_Locked = 0;
	Item* m_Key = nullptr;
	std::pair<std::string, std::string> m_Rooms;
	bool m_FlipDescription = false;
};

