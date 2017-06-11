#ifndef TWITTERINTH_ROOM_ADDON_HPP
#define TWITTERINTH_ROOM_ADDON_HPP

// App includes.
#include "Twitterinth_Room_AddOn.hpp"

// STD includes.
#include <cstdint>
#include <string>
#include <map>
#include <cstdio>

// OS includes.
#include <Windows.h>

namespace Twitterinth
{

struct AddOn
{
	/**
	*/
	AddOn(const std::string &a_path, const std::string &a_file_name);

	/**
	*/
	~AddOn();

	/**
	*/
	operator bool() const;

	/**
	*/
	const std::string &name() const;

	/**
	*/
	const std::string &creator() const;

	/**
	*/
	std::uint32_t version() const;
	
	/**
	*/
	std::string texture_file() const;

	/**
	*/
	std::uint32_t tile_size() const;

	/**
	*/
	void get_tile(std::uint32_t, std::uint32_t *, std::uint32_t *) const;

	/**
	*/
	void create_room(std::uint32_t *, std::uint32_t, ::uint32_t, ::uint32_t, const char *) const;

private:

	//! 
	using addon_info_t = void(char(&)[50], char(&)[20], std::uint32_t *, char(&)[FILENAME_MAX], std::uint32_t *);
	//!
	using get_tile_t = void(std::uint32_t, std::uint32_t *, std::uint32_t *);
	//!
	using create_room_t = void(std::uint32_t *, std::uint32_t, ::uint32_t, ::uint32_t, const char *);
	//! 
	using handler_t = HMODULE;

	//!
	handler_t m_addon_handler{};
	//!
	addon_info_t *m_addon_info{};
	//!
	get_tile_t *m_get_tile{};
	//!
	create_room_t *m_create_room{};

	//!
	std::string m_name{};
	//!
	std::string m_creator{};
	//!
	std::uint32_t m_version;
	//!
	std::string m_texture_file{};
	//!
	std::uint32_t m_tile_size{};
	//!
	std::string m_working_directory{};
};

}

#endif