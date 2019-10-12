#pragma once
#include <Windows.h>
#include <string>
#include <exception>

class RegistryKey
{
private:
	RegistryKey();
public:

	static void create_if_not_exist(const HKEY hive, const char * keyPath);
	static std::string query_string_value(const HKEY hive, const char * keyPath, const char* valueName);
	static void set_string_value(const HKEY hive, const char * keyPath, const char* valueName, const char* value);
	static void delete_value(const HKEY hive, const char * keyPath, const char* valueName);

	~RegistryKey();
};

