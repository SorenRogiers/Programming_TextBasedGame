#include "stdafx.h"
#include "Objects.h"

#include "Inventory.h"
#include <vector>
#include <string>
#include <JsonBox.h>
#include "LivingEntity.h"

Objects::Objects(std::string id,std::string name, Inventory items):JsonEntity(id),m_Name(name),m_ItemsInObject(items)
{

}

Objects::Objects(std::string id, JsonBox::Value& v, JsonEntityManager* mgr):JsonEntity(id)
{
	Load(v, mgr);
}

Objects::~Objects()
{
}

void Objects::Load(JsonBox::Value& v, JsonEntityManager* mgr)
{
	JsonBox::Object jsonObject = v.getObject();
	m_Name = jsonObject["name"].getString();
	m_Description = jsonObject["description"].getString();
	m_ItemsInObject = Inventory(jsonObject["inventory"], mgr);

}

JsonBox::Object Objects::GetJsonObject()
{
	JsonBox::Object jsonObject;
	jsonObject["inventory"] = m_ItemsInObject.GetJson();

	return jsonObject;
}