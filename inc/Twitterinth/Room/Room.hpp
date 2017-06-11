#ifndef TWITTERINTH_ROOM_HPP
#define TWITTERINTH_ROOM_HPP

// App includes
#include "Twitter/Types.hpp"
#include "Types.hpp"
#include "Floorplan.hpp"
#include "AddOn.hpp"

// 3rd party libraries.
#include "SFML/Graphics.hpp"

// STD includes
#include <cstdint>
#include <vector>
#include <memory>

namespace Twitterinth
{

struct Room
{
	//!
	using tiles_t = std::unique_ptr<std::uint32_t[]>;
	//!
	using floorplan_t = Wrapper::as_2D<tiles_t>;
	//!
	using sprites_wrapper_t = Wrapper::as_2D<std::vector<sf::Sprite>>;
	//!
	using floorplan_ptr = std::unique_ptr<floorplan_t>;

	/**
	*/
	Room(AddOn &a_generator, const Twitter::Tweet &a_tweet);

	/**
	*/
	void zoom(float a_zoom);

	/**
	*/
	void center_in(const sf::Vector2f &a_position);

	/**
	*/
	void render(sf::RenderTarget &a_render_target, const sf::VideoMode &a_video_mode);

	/**
	*/
	std::uint32_t width() const;

	/**
	*/
	std::uint32_t height() const;

	/**
	*/
	std::uint32_t tiles_count() const;

	/**
	*/
	float zoom() const;

	/**
	*/
	const floorplan_t &floorplan() const;

	/**
	*/
	sf::Vector2f room_point_to_screen(const sf::Vector2f &a_point, const sf::VideoMode &a_video_mode);

private:
	//!
	tiles_t m_tiles{ nullptr };
	//!
	floorplan_ptr m_floorplan{ nullptr };
	//!
	AddOn &m_generator;

	//!
	sf::Texture m_texture;
	//!
	std::vector<sf::Sprite> m_sprites;
	//!
	sf::Vector3f m_position{0.f, 0.f, 15.f};
};

}

#endif