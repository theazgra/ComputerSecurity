#include "pch.h"
#include <Windows.h>
#include <string.h>
#include <vector>
#include <cstdio>

typedef unsigned char byte;
typedef std::vector<byte> ByteArray;

bool create_ads(const std::string &filePath, const std::string &adsName, const ByteArray &data)
{
	bool success = true;
	std::string ioFileName = filePath + ":" + adsName + '\0';
	HANDLE adsHandle = CreateFileA(ioFileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (adsHandle != INVALID_HANDLE_VALUE)
	{
		BOOL writeResult = WriteFile(adsHandle, data.data(), data.size(), NULL, NULL);
		if (writeResult)
		{
			fprintf(stdout, "Successfuly wrote to ADS stream.\n");
		}
		else
		{
			success = false;
			fprintf(stderr, "Failed on WriteFile call. Error: %lu\n", GetLastError());
		}

		if (!CloseHandle(adsHandle))
		{
			fprintf(stderr, "Failed to close file handle. Error: %lu\n", GetLastError());
		}
	}
	else
	{
		success = false;
		fprintf(stderr, "Failed to open file. Error: %lu\n", GetLastError());
	}
	return success;
}

ByteArray read_ads(const std::string &filePath, const std::string &adsName)
{
	ByteArray data;
	bool success = true;
	std::string ioFileName = filePath + ":" + adsName + '\0';
	HANDLE adsHandle = CreateFileA(ioFileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (adsHandle != INVALID_HANDLE_VALUE)
	{
		DWORD fileSize = GetFileSize(adsHandle, NULL);
		if (fileSize == INVALID_FILE_SIZE)
		{
			fprintf(stderr, "Failed to retrieve file size. Error: %lu.\n", GetLastError());
			return data;
		}
		data.resize(fileSize);
		if (ReadFile(adsHandle, data.data(), fileSize, NULL, NULL))
		{
			fprintf(stdout, "Successfuly read from ADS stream.\n");
			return data;
		}
		else
		{
			fprintf(stderr, "Failed on ReadFile call. Error: %lu\n", GetLastError());
			data.clear();
			return data;
		}

		if (!CloseHandle(adsHandle))
		{
			fprintf(stderr, "Failed to close file handle. Error: %lu\n", GetLastError());
		}
	}
	else
	{
		success = false;
		fprintf(stderr, "Failed to open file. Error: %lu\n", GetLastError());
	}
	return data;
}

int main()
{

	const char * content = "This is ads stream content";
	size_t len = strlen(content);
	ByteArray buffer(len);
	memcpy(buffer.data(), content, len);

	create_ads("C:\\Users\\mor0146\\Documents\\Keylog.log", "FunkyStream2", buffer);


	ByteArray adsData = read_ads("C:\\Users\\mor0146\\Documents\\Keylog.log", "FunkyStream2");
	return 0;
}