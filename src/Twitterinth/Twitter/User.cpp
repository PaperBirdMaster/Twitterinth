// App includes
#include "Twitter/User.hpp"

namespace Twitter
{

const User User::Null{};

User::User(text_t a_name, text_t a_screen_name, std::uint32_t a_followers, std::uint32_t a_favourites, std::uint32_t a_profile_background_color, std::uint32_t a_profile_link_color, std::uint32_t a_profile_sidebar_color, std::uint32_t a_profile_fill_color, std::uint32_t a_profile_text_color, string_t a_profile_background, string_t a_profile_banner, string_t a_profile_image) :
	m_name                    { a_name                     },
	m_screen_name             { a_screen_name              },
	m_followers               { a_followers                },
	m_favourites              { a_favourites               },
	m_profile_background_color{ a_profile_background_color },
	m_profile_link_color      { a_profile_link_color       },
	m_profile_sidebar_color   { a_profile_sidebar_color    },
	m_profile_fill_color      { a_profile_fill_color       },
	m_profile_text_color      { a_profile_text_color       },
	m_profile_background      { a_profile_background       },
	m_profile_banner          { a_profile_banner           },
	m_profile_image           { a_profile_image            }
{
}

const string_t &User::profile_picture() const
{
	return m_profile_image;
}

const string_t &User::profile_banner() const
{
	return m_profile_banner;
}

std::uint32_t User::profile_text_color() const
{
	return m_profile_text_color;
}

std::uint32_t User::profile_background_color() const
{
	return m_profile_background_color;
}


}
