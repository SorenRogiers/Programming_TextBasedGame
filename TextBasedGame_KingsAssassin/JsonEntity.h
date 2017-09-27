#pragma once

#include <JsonBox.h>
#include <string>

class JsonEntityManager;

class JsonEntity
{
public:

	JsonEntity(std::string id)
	{
		this->m_ID = id;
	}
	virtual ~JsonEntity() {}

	virtual void Load(JsonBox::Value& v, JsonEntityManager* mgr) = 0;

	std::string m_ID;
};
