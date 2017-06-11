// App includes
#include "Twitter/Media.hpp"

namespace Twitter
{

Media::Media(string_t a_path, media_type a_media_type, std::uint32_t a_duration) :
	m_path      { a_path       },
	m_media_type{ a_media_type },
	m_duration  { a_duration   }
{
}

}
