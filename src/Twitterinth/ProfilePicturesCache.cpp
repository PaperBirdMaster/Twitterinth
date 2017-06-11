// App includes.
#include "Twitterinth_twitter_user.hpp"
#include "ProfilePicturesCache.hpp"
#include "URL/DownloadFile.hpp"
#include "Twitter/User.hpp"

// STD includes.
#include <fstream>

namespace Twitterinth
{

ProfilePicturesCache::ProfilePicturesCache()
{
	std::ifstream profile_pictures_cache(users_file);
	sf::Texture cache_texture;
	
	m_working_texture.create(texture_width, texture_width);
	m_saved_users.reserve(sprite_per_texture);

	// Cache data already exist?
	if (profile_pictures_cache.is_open() && cache_texture.loadFromFile(pictures_file))
	{
		for (Twitter::id_t user_id; profile_pictures_cache.read(reinterpret_cast<char *>(&user_id), sizeof(user_id));)
		{
			m_saved_users.push_back(user_id);
		}

		m_working_texture.draw(sf::Sprite{ cache_texture });
		m_working_texture.display();

		std::int32_t index = 0;
		m_saved_sprites.resize(m_saved_users.size());
		for (auto &sprite : m_saved_sprites)
		{
			const auto sprite_x = (index % sprite_width) * sprite_width;
			const auto sprite_y = (index / sprite_width) * sprite_width;
			auto &sprite = m_saved_sprites[index];

			sprite.setTexture(m_working_texture.getTexture());
			sprite.setTextureRect({ sprite_x, sprite_y, sprite_width, sprite_width });
			++index;
		}


	}
}

ProfilePicturesCache::~ProfilePicturesCache()
{
	std::ofstream profile_pictures_cache(users_file);
	profile_pictures_cache.write(reinterpret_cast<char *>(m_saved_users.data()), m_saved_users.size() * sizeof(Twitter::id_t));

	m_working_texture.getTexture().copyToImage().saveToFile(pictures_file);
}

sf::Sprite &ProfilePicturesCache::get_user_profile(Twitter::id_t a_user_id)
{
	// Look for the user in the cache.
	auto user_found = std::find(m_saved_users.begin(), m_saved_users.end(), a_user_id);

	if (user_found == m_saved_users.end())
	{
		// Get the user (ideally from the cache) and ask for its profile picture url.
		std::string profile_picture_url = Twitterinth::get_user(a_user_id).profile_picture();
		// Download the big version (by getting rid of the "_normal" in the name).
		std::string profile_picture_file = "../media/cache" + profile_picture_url.substr(profile_picture_url.find_last_of('/'));
		download::file(profile_picture_url.replace(profile_picture_url.find("_normal"), 7u, ""), profile_picture_file);

		file_to_cache(profile_picture_file, a_user_id);
	}
	else
	{
		return m_saved_sprites[std::distance(m_saved_users.begin(), user_found)];
	}

	return m_saved_sprites[0];
}

sf::Sprite & ProfilePicturesCache::file_to_cache(const std::string &a_file_name, Twitter::id_t a_user_id)
{
	sf::Texture profile_picture_texture;
	if (profile_picture_texture.loadFromFile(a_file_name))
	{
		const auto users_count = m_saved_users.size();
		const auto texture_size = profile_picture_texture.getSize();
		const auto sprite_scale_x = sprite_width / float(texture_size.x);
		const auto sprite_scale_y = sprite_width / float(texture_size.y);
		const auto sprite_x = (users_count % sprites_per_row) * sprite_width;
		const auto sprite_y = (users_count / sprites_per_row) * sprite_width;

		sf::Sprite profile_picture_sprite;	
		profile_picture_sprite.setTexture(profile_picture_texture);
		profile_picture_sprite.setScale(sprite_scale_x, sprite_scale_x);
		profile_picture_sprite.setPosition(sprite_x, sprite_y);

		m_working_texture.draw(profile_picture_sprite);
		m_working_texture.display();

		m_saved_users.push_back(a_user_id);
		m_saved_sprites.emplace_back(m_working_texture.getTexture(), sf::IntRect(sprite_x, sprite_y, sprite_width, sprite_width));
		return m_saved_sprites.back();
	}

	return m_saved_sprites[0];
}

}
