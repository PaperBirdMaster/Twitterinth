// App inclues.
#include "Room/AddOn.hpp"

// STD includes.
#include <iostream>

// SO includes.
#include <Windows.h>

namespace Twitterinth
{

template <typename function_t>
void get_function(function_t &a_function_pointer, HMODULE a_handler, const char *a_function_name)
{
	if (FARPROC function = GetProcAddress(a_handler, a_function_name))
	{
		a_function_pointer = reinterpret_cast<function_t>(function);
	}
	else
	{
		std::cerr << "Error [" << GetLastError() << "] loading Twitterinth::AddOn function \"" << a_function_name << "\"\n";
	}
}

AddOn::AddOn(const std::string &a_path, const std::string &a_file_name) :
	m_working_directory{a_path}
{
	constexpr char ADDON_INFO[] { "addon_info"  };
	constexpr char GET_TILE[]   { "get_tile"    };
	constexpr char CREATE_ROOM[]{ "create_room" };

	if ((m_addon_handler = LoadLibraryA((a_path + a_file_name).c_str())))
	{
		get_function(m_addon_info,  m_addon_handler, ADDON_INFO);
		get_function(m_get_tile,    m_addon_handler, GET_TILE);
		get_function(m_create_room, m_addon_handler, CREATE_ROOM);

		if (m_addon_info)
		{
			char name[50]{};
			char creator[20]{};
			char texture_file[FILENAME_MAX]{};

 			m_addon_info(name, creator, &m_version, texture_file, &m_tile_size);
			m_name = name;
			m_creator = creator;
			m_texture_file = texture_file;

			std::cout << "Loaded " << m_name << " Room generator by " << m_creator << " Version "
						<< ((m_version & 0xff000000u) >> 24u) << '.'
						<< ((m_version & 0x00ff0000u) >> 16u) << '.'
						<< ((m_version & 0x0000ff00u) >>  8u) << '.'
						<< (m_version & 0x000000ffu)
						<< '\n';
		}
	}
}

AddOn::~AddOn()
{
	if (m_addon_handler)
	{
		FreeLibrary(m_addon_handler);
	}
}

AddOn::operator bool() const
{
	const auto file = texture_file();
	DWORD texture_file_attributes = GetFileAttributesA(file.c_str());

	return (m_addon_handler != NULL) && (m_create_room != NULL) && (texture_file_attributes != INVALID_FILE_ATTRIBUTES);
}

const std::string &AddOn::name() const
{
	return m_name;
}

const std::string &AddOn::creator() const
{
	return m_creator;
}

std::uint32_t AddOn::version() const
{
	return m_version;
}

std::string AddOn::texture_file() const
{
	return m_working_directory + m_texture_file;
}

std::uint32_t AddOn::tile_size() const
{
	return m_tile_size;
}

void AddOn::get_tile(std::uint32_t a_tile_id, std::uint32_t *a_x, std::uint32_t *a_y) const
{
	if (m_get_tile)
	{
		m_get_tile(a_tile_id, a_x, a_y);
	}
}

void AddOn::create_room(std::uint32_t *a_map_tiles, std::uint32_t a_tile_count, ::uint32_t a_width, ::uint32_t a_heigth, const char *a_tweet_text) const
{
	if (m_create_room)
	{
		m_create_room(a_map_tiles, a_tile_count, a_width, a_heigth, a_tweet_text);
	}
}


}
