#ifndef TWITTER_MEDIA_HPP
#define TWITTER_MEDIA_HPP

// App includes
#include "Twitter/Types.hpp"

// STD includes
#include <cstdint>

namespace Twitter
{

struct Media
{
	Media(string_t a_path, media_type a_media_type, std::uint32_t m_duration);
	Media() = default;

private:
	string_t m_path{};
	media_type m_media_type{ media_type::unknown };
	std::uint32_t m_duration{};
};

}

#endif