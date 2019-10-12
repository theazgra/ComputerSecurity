#include "RegistryKey.h"


RegistryKey::RegistryKey()
{
}

static void open_key(const HKEY hive, const char * keyPath, PHKEY keyHandle)
{
	LSTATUS winApiResult = RegOpenKeyExA(hive, keyPath, 0, KEY_ALL_ACCESS, keyHandle);
	if (winApiResult != ERROR_SUCCESS)
	{
		throw std::runtime_error("Key with specified path doesn't exist.");
	}
}

void RegistryKey::create_if_not_exist(const HKEY hive, const char * keyPath)
{
	DWORD dwDisposition;
	HKEY handle;
	LSTATUS winApiResult = RegCreateKeyExA(hive, keyPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &handle, &dwDisposition);
	if (winApiResult == ERROR_SUCCESS)
	{
		RegCloseKey(handle);
	}
	else
	{
		throw std::runtime_error("Error creating key with specified path.");
	}
}

std::string RegistryKey::query_string_value(const HKEY hive, const char * keyPath, const char * valueName)
{
	HKEY keyHandle;
	open_key(hive, keyPath, &keyHandle);
	char data[512];
	DWORD size;
	DWORD type;
	LSTATUS queryResult = RegQueryValueExA(keyHandle, valueName, NULL, &type, (LPBYTE)data, &size);
	if (queryResult != ERROR_SUCCESS)
	{
		throw std::runtime_error("Failed to query key value.");
	}
	if (type != REG_SZ)
	{
		throw std::runtime_error("Requested value is of different type.");
	}
	std::string result = std::string(data, data + size);
	return result;
}

void RegistryKey::set_string_value(const HKEY hive, const char * keyPath, const char * valueName, const char * value)
{
	HKEY keyHandle;
	open_key(hive, keyPath, &keyHandle);
	const size_t len = strlen(value) + 1;
	LSTATUS winApiResult = RegSetValueExA(keyHandle, valueName, 0, REG_SZ, (const BYTE *)value, len);
	if (winApiResult != ERROR_SUCCESS)
	{
		throw std::runtime_error("Failed to set string value.");
	}
}

void RegistryKey::delete_value(const HKEY hive, const char * keyPath, const char * valueName)
{
	HKEY keyHandle;
	open_key(hive, keyPath, &keyHandle);
	LSTATUS winApiResult = RegDeleteValueA(keyHandle, valueName);
	if (winApiResult != ERROR_SUCCESS)
	{
		throw std::runtime_error("Failed to delete value.");
	}
}


RegistryKey::~RegistryKey()
{
}
