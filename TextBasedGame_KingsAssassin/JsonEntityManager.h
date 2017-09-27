#pragma once
#include <string>
#include <map>

#include "JsonEntity.h"

class JsonEntityManager
{

public:
	// Constructor
	JsonEntityManager();

	// Destructor
	~JsonEntityManager();
	
	template<typename T>
	void LoadJson(std::string filename);

	template<typename T>
	T* GetEntity(std::string id);

private:
	std::map<std::string, JsonEntity*> m_Data;
};

template <typename T>
std::string EntityToString();
