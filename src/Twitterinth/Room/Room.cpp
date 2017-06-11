// App includes
#include "Room/Room.hpp"
#include "Twitter/Tweet.hpp"

// External lib includes.
#include "SFML/Graphics.hpp"

// STD includes.
#include <memory>

namespace Twitterinth
{

Room::Room(AddOn &a_generator, const Twitter::Tweet &a_tweet) :
	m_generator{ a_generator }
{
	if (m_texture.loadFromFile(a_generator.texture_file()))
	{
		const auto size = std::sqrt(a_tweet.text().length());
		const auto w = static_cast<Floorplan::index_t>(size * 2.75);
		const auto h = static_cast<Floorplan::index_t>(size * 2.25);
		const auto tile_size = m_generator.tile_size();

		m_tiles.reset(new std::uint32_t[w * h]{});
		m_floorplan.reset(new floorplan_t{m_tiles, w, h});
		m_sprites.reserve(w * h);

		m_generator.create_room(m_tiles.get(), w * h, w, h, a_tweet.text().c_str());

		auto &floorplan = *m_floorplan;

		for (std::uint32_t y = 0u; y < h; ++y)
		{
			for (std::uint32_t x = 0u; x < w; ++x)
			{
				std::uint32_t sprite_x, sprite_y;
				m_generator.get_tile(floorplan(x, y), &sprite_x, &sprite_y);
				m_sprites.emplace_back(m_texture, sf::IntRect(sprite_x, sprite_y, tile_size, tile_size));
			}
		}
	}
}

void Room::zoom(float a_zoom)
{
	m_position.z += a_zoom;
}

void Room::center_in(const sf::Vector2f &a_position)
{
	m_position.x = a_position.x;
	m_position.y = a_position.y;
}

std::uint32_t Room::width() const
{
	return m_floorplan->width();
}

std::uint32_t Room::height() const
{
	return m_floorplan->height();
}

std::uint32_t Room::tiles_count() const
{
	return m_floorplan->width() * m_floorplan->height();
}

float Room::zoom() const
{
	return m_position.z;
}

const Room::floorplan_t &Room::floorplan() const
{
	return *m_floorplan.get();
}

sf::Vector2f Room::room_point_to_screen(const sf::Vector2f &a_point, const sf::VideoMode &a_video_mode)
{
	const float tile_scale = (a_video_mode.height / m_position.z) / m_generator.tile_size();
	const float tile_size = tile_scale * m_generator.tile_size();

	return{ (a_point.x * tile_size) - (m_position.x * tile_size) + (a_video_mode.width / 2.f), (a_point.y * tile_size) - (m_position.y * tile_size) + (a_video_mode.height / 2.f) };
}

void Room::render(sf::RenderTarget &a_render_target, const sf::VideoMode &a_video_mode)
{
	const float tile_scale = (a_video_mode.height / m_position.z) / m_generator.tile_size();
	const float tile_size = tile_scale * m_generator.tile_size();
	const auto width = m_floorplan->width();
	const auto height = m_floorplan->height();
	sprites_wrapper_t sprites{m_sprites, width , height};

	for (Floorplan::index_t y = 0u; y < height; ++y)
	{
		for (Floorplan::index_t x = 0u; x < width; ++x)
		{
			auto &sprite = sprites(x, y);
			sprite.setScale(tile_scale, tile_scale);
			sprite.setPosition((x * tile_size) - (m_position.x * tile_size) + (a_video_mode.width / 2.f), (y * tile_size) - (m_position.y * tile_size) + (a_video_mode.height / 2.f));
		}
	}

	for (const auto &sprite : m_sprites)
	{
		a_render_target.draw(sprite);
	}
}

}
