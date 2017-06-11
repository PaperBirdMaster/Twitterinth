#ifndef URL_DOWNLOAD_FILE
#define URL_DOWNLOAD_FILE

// STD includes.
#include <string>

namespace download
{

/**
*/
std::string temp_file(const std::string &a_url_to_file);

/**
*/
std::string temp_file(const char *const a_url_to_file);

/**
*/
bool file(const std::string &a_url_to_file, const std::string &a_path_to_file);

/**
*/
bool file(const char *const a_url_to_file, const char *const a_path_to_file);

}

#endif