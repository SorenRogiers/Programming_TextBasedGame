#pragma once

#include <vector>
#include <string>
#include <JsonBox.h>

#include "JsonEntity.h"
#include "Inventory.h"
#include "GameEventHandler.h"

class JsonEntityManager;

class Objects: public JsonEntity
{
public:
	
	Objects(std::string id,std::string name,Inventory items);
	Objects(std::string id, JsonBox::Value& v, JsonEntityManager *mgr);
	~Objects();

	void Load(JsonBox::Value& v, JsonEntityManager* mgr)override;
	JsonBox::Object GetJsonObject();

	Inventory m_ItemsInObject;
	std::string m_Description;
	std::string m_Name;
};

