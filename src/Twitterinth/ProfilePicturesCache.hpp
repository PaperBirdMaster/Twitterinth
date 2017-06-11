#ifndef TWITTERINT_PROFILE_PICTURES_CACHE
#define TWITTERINT_PROFILE_PICTURES_CACHE

// App includes.
#include "Twitter/Types.hpp"

// External lib includes.
#include "SFML/Graphics.hpp"

// STD includes.
#include <vector>

namespace Twitterinth
{

struct ProfilePicturesCache
{
	//! Width of each cache texture.
	static constexpr std::int32_t texture_width = 2048;
	//! Width of each cache icon.
	static constexpr std::int32_t sprite_width = 128;
	//! Width of each cache icon.
	static constexpr std::int32_t sprites_per_row = texture_width / sprite_width;
	//! Amount of pictures that fits in each cache texture.
	static constexpr std::int32_t sprite_per_texture = sprites_per_row * sprites_per_row;

	//!
	static constexpr const char users_file[] = "../media/cache/profile_pictures_cache.bin";

	//!
	static constexpr const char pictures_file[] = "../media/cache/profile_pictures_cache.png";

	/**
	*/
	ProfilePicturesCache();

	/**
	*/
	~ProfilePicturesCache();

	/**
	*/
	sf::Sprite &get_user_profile(Twitter::id_t a_user_id);

private:

	/* To download user profile pictures is expensive, so we store them
	* locally, each time we download one we save the owner user-id in this
	* collection; the order of insertion allows us to know where the picture
	* is into the cached textures. */
	std::vector<Twitter::id_t> m_saved_users{};

	//!
	std::vector<sf::Sprite> m_saved_sprites{};

	//!
	sf::RenderTexture m_working_texture{};

	/**
	*/
	sf::Sprite &file_to_cache(const std::string &a_file_name, Twitter::id_t a_user_id);
};

}

#endif
