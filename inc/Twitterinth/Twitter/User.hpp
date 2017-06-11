#ifndef TWITTERINTH_TWITTER_USER
#define TWITTERINTH_TWITTER_USER

// App includes
#include "Twitter/Types.hpp"

// External lib includes.
#include "rapidjson/document.h"

// STD includes
#include <string>
#include <cstdint>

namespace Twitter
{

struct User
{
	User(text_t a_name, text_t a_screen_name, std::uint32_t a_followers, std::uint32_t a_favourites, std::uint32_t a_profile_background_color, std::uint32_t a_profile_link_color, std::uint32_t a_profile_sidebar_color, std::uint32_t a_profile_fill_color, std::uint32_t a_profile_text_color, string_t a_profile_background, string_t a_profile_banner, string_t a_profile_image);

	const string_t &profile_picture() const;

	const string_t &profile_banner() const;

	std::uint32_t profile_text_color() const;

	std::uint32_t profile_background_color() const;

	static const User Null;
	
private:
	User() = default;

	text_t m_name{};
	text_t m_screen_name{};
	std::uint32_t m_followers{};
	std::uint32_t m_favourites{};
	std::uint32_t m_profile_background_color{};
	std::uint32_t m_profile_link_color{};
	std::uint32_t m_profile_sidebar_color{};
	std::uint32_t m_profile_fill_color{};
	std::uint32_t m_profile_text_color{};
	string_t m_profile_background{};
	string_t m_profile_banner{};
	string_t m_profile_image{};

};

}

#endif