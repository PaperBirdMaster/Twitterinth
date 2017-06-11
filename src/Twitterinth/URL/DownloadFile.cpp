// App includes
#include "URL/DownloadFile.hpp"

// OS includes.
#include <Windows.h>

namespace download
{

std::string temp_file(const std::string &a_url_to_file)
{
	char file_name[MAX_PATH]{};

	if (URLDownloadToCacheFileA(nullptr, a_url_to_file.c_str(), file_name, MAX_PATH, 0, nullptr) == S_OK)
	{
		return file_name;
	}

	return "";
}

std::string temp_file(const char *const a_url_to_file)
{
	char file_name[MAX_PATH]{};

	if (URLDownloadToCacheFileA(nullptr, a_url_to_file, file_name, MAX_PATH, 0, nullptr) == S_OK)
	{
		return file_name;
	}

	return "";
}

bool file(const std::string &a_url_to_file, const std::string &a_path_to_file)
{
	return URLDownloadToFileA(nullptr, a_url_to_file.c_str(), a_path_to_file.c_str(), 0, nullptr) == S_OK;
}

bool file(const char *const a_url_to_file, const char *const a_path_to_file)
{
	return URLDownloadToFileA(nullptr, a_url_to_file, a_path_to_file, 0, nullptr) == S_OK;
}

}