// App includes.
#include "render.hpp"
#include "Card.hpp"

// Twitterinth includes.
#include "Twitterinth.hpp"
#include "Twitter/Tweet.hpp"
#include "Twitter/User.hpp"

#include "Camera.hpp"

#include "Room/Generator.hpp"
#include "Room/Room.hpp"

// External lib includes.
#include "SFML/Graphics.hpp"

// STD includes.
#include <memory>

#include <iostream>
#include <iomanip>
#include <string>

namespace Twitterinth
{

void draw_x(const sf::Vector2f &p, const sf::Color &a_color, sf::RenderTarget &rt)
{
	const sf::Vector2f left{ -5.f, -5.f }, right{ 5.f, -5.f };

	sf::VertexArray
		a(sf::LinesStrip, 2),
		b(sf::LinesStrip, 2);

	a[0].position = p - left;
	a[1].position = p + left;
	b[0].position = p - right;
	b[1].position = p + right;

	a[0].color = a_color;
	a[1].color = a_color;
	b[0].color = a_color;
	b[1].color = a_color;

	rt.draw(a);
	rt.draw(b);
}

void draw_line(const sf::Vector2f &p1, const sf::Vector2f &p2, const sf::Color &a_color, sf::RenderTarget &rt)
{
	sf::VertexArray line(sf::LinesStrip, 2);

	line[0].position = p1;
	line[1].position = p2;

	line[0].color = a_color;
	line[1].color = a_color;

	rt.draw(line);
}

//!
const sf::VideoMode video_mode = sf::VideoMode::getDesktopMode();

//!
std::unique_ptr<sf::RenderWindow> window;

template <typename number_t>
constexpr std::int32_t sign(number_t a_number)
{
	return (number_t{} < a_number) - (a_number < number_t{});
}

template <typename scalar_t>
std::int32_t quadrant(const sf::Vector2<scalar_t> &a_vector2)
{
	constexpr int quadrant_values[3][3] =
	{
		{ 8, 7, 6 },
		{ 1, 0, 5 },
		{ 2, 3, 4 },
	};

	return quadrant_values[sign(a_vector2.x) + 1][sign(a_vector2.y) + 1];
}

float dot(const sf::Vector2f &a, const sf::Vector2f &b)
{
	return ((a.x * b.x) + (a.y * b.y));
}

float cross(const sf::Vector2f &a, const sf::Vector2f &b)
{
	return ((a.x * b.y) - (a.y * b.x));
}

std::tuple<float, sf::Vector2f> intersect(const sf::Vector2f p1, const sf::Vector2f p2, const sf::Vector2f p3, const sf::Vector2f p4)
{
	auto a = p1 - p2;
	auto b = p3 - p4;

	// If d is zero, there is no intersection
	if (float cross_product = cross(a, b))
	{
		// Get the x and y
		float pre = cross(p1, p2), post = cross(p3, p4);
		sf::Vector2f result{ (pre * b.x - a.x * post) / cross_product, (pre * b.y - a.y * post) / cross_product };

		const auto a_dot_p1result = dot(a, p1 - result);
		const auto a_dot_resultp2 = dot(a, result - p2);
		const auto a_intersect_delta = a_dot_p1result / (a_dot_p1result + a_dot_resultp2);

		return { a_intersect_delta, result };
	}

	return { std::numeric_limits<float>::infinity(),{ 0, 0 } };
}

std::pair<sf::Vector2f, sf::Vector2f> top(const sf::FloatRect &a_rect)
{
	return { { a_rect.left, a_rect.top }, { a_rect.left + a_rect.width, a_rect.top } };
}

std::pair<sf::Vector2f, sf::Vector2f> right(const sf::FloatRect &a_rect)
{
	return { { a_rect.left + a_rect.width, a_rect.top }, { a_rect.left + a_rect.width, a_rect.top + a_rect.height } };
}

std::pair<sf::Vector2f, sf::Vector2f> bottom(const sf::FloatRect &a_rect)
{
	return { { a_rect.left, a_rect.top + a_rect.height }, { a_rect.left + a_rect.width, a_rect.top + a_rect.height } };
}

std::pair<sf::Vector2f, sf::Vector2f> left(const sf::FloatRect &a_rect)
{
	return { { a_rect.left, a_rect.top }, { a_rect.left, a_rect.top + a_rect.height } };
}

sf::FloatRect point_to_tile(const sf::Vector2f &a_point)
{
	return { std::floor(a_point.x), std::floor(a_point.y), 1.f, 1.f };
}

struct rect_collision
{
	enum collision_position
	{
		none,
		upleft,
		upright,
		downright,
		downleft,
	};

	rect_collision()
	{}

	rect_collision(collision_position a_position, float a_delta, sf::Vector2f a_point) :
		position{ a_position },
		delta   { a_delta    },
		point   { a_point    }
	{}

	const collision_position position{};
	const float delta{ std::numeric_limits<float>::infinity() };
	const sf::Vector2f point{};
};

std::vector<rect_collision> check_collision(const sf::FloatRect &a_rect, const sf::Vector2f &a_movement, const Room::floorplan_t &a_floorplan)
{
	std::vector<rect_collision> result;
	auto check = [&](auto a_from, auto a_towards)
	{
		const auto from = a_from(a_rect);
		const auto first_target = from.first + a_movement;
		const auto second_target = from.second + a_movement;

		if (!IS_FLOOR(a_floorplan(static_cast<Floorplan::tile_t>(first_target.x), static_cast<Floorplan::tile_t>(first_target.y))))
		{
			const auto obstacle = a_towards(point_to_tile(first_target));
			auto intersection = intersect(from.first, first_target, obstacle.first, obstacle.second);
			if (std::get<float>(intersection) < 1.f)
			{
				result.emplace_back(rect_collision::upleft, std::get<float>(intersection), std::get<sf::Vector2f>(intersection));
			}
		}
		if (!IS_FLOOR(a_floorplan(static_cast<Floorplan::tile_t>(second_target.x), static_cast<Floorplan::tile_t>(second_target.y))))
		{
			const auto obstacle = a_towards(point_to_tile(second_target));
			auto intersection = intersect(from.second, second_target, obstacle.first, obstacle.second);
			if (std::get<float>(intersection) < 1.f)
			{
				result.emplace_back(rect_collision::upright, std::get<float>(intersection), std::get<sf::Vector2f>(intersection));
			}
		}
	};

	switch (quadrant(a_movement))
	{
		case 1: // Moving towards North
			check(top, bottom);
			break;
		case 2: // Moving towards NorthEast
			check(top, bottom);
			check(right, left);
			break;

		case 3: // Moving towards East
			check(right, left);
			break;
		case 4: // Moving towards SouthEeast
			check(bottom, top);
			check(right, left);
			break;

		case 5: // Moving towards South
			check(bottom, top);
			break;
		case 6: // Moving towards SowthWest
			check(bottom, top);
			check(left, right);
			break;

		case 7: // Moving towards West
			check(left, right);
			break;
		case 8: // Moving towards NorthWest
			check(top, bottom);
			check(left, right);
			break;
	}

	return result;
}

struct player
{
	sf::Vector2f m_position{};
	sf::FloatRect m_aabb{};
	sf::Texture m_t{};
	sf::Sprite m_s{};
	float m_speed = 6.f;

	player(const sf::Vector2f &a_position) :
		m_position{a_position}
	{
		m_t.loadFromFile("../AddOns/Twitterinth_cavern_room/Tileset.png");
		m_s.setTexture(m_t);
		m_s.setTextureRect({ 224, 32, 32, 32 });
		m_s.setPosition((video_mode.width / 2.f) - 32.f, (video_mode.height / 2.f) - 32.f);
	}

	void update(sf::Time a_elapsed, const Room::floorplan_t &a_floorplan)
	{
		sf::Vector2f movement{};

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			movement.x = -m_speed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			movement.x = m_speed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			movement.y = -m_speed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			movement.y = m_speed;
		}

		if (movement.x || movement.y)
		{
			movement *= (1.f / std::sqrt((movement.x * movement.x) + (movement.y * movement.y)));
			movement *= (a_elapsed.asSeconds() * m_speed);

			auto collision = check_collision(sf::FloatRect{ m_position - sf::Vector2f{.3f, .3f}, sf::Vector2f{.6f, .6f} }, movement, a_floorplan);

			if (!collision.empty())
			{
				movement *= collision[0].delta;
			}

			m_position.x += movement.x;
			m_position.y += movement.y;
		}
	}

	void move(float a_x, float a_y)
	{
		m_position.x += a_x;
		m_position.y += a_y;
	}

	const sf::FloatRect &get_aabb() const
	{
		return { m_position.x - .3f, m_position.y -.3f, .6f, .6f };
	}
};

void run()
{
	window.reset(new sf::RenderWindow(video_mode, "Twitterinth"/*, sf::Style::None*/));

	// Tweet con dos imagenes 842747633470980096
	// Tweet con gif animado 842820768023089152
	// Tweet con retweet 842804932168351745
	// Tweet video de usuario verificado texto con emojis 842792383624503297
	// Tweet respuesta a otro tweet 842786151396249600
	// Tweet con varias respuestas 842785553762435072
	// Tweet con millones de rt y fav y centenares de respuestas 840894259502940162
	//const auto starting_point = Twitterinth::get_tweet(507934791485100033ull);
	//const auto starting_point = Twitterinth::get_tweet(852241403996508161ull);
	//const auto starting_point = Twitterinth::get_tweet(864194619101388800ull);
	const auto starting_point = Twitterinth::get_start_point();
	const auto user = Twitterinth::get_user(starting_point.author());
	const auto w = std::sqrt(starting_point.text().length());

	auto room = Twitterinth::create_room(starting_point);
	Twitterinth::Card card{sf::IntRect(25, ((5 * video_mode.height) / 6) - 25, video_mode.width - 50, video_mode.height / 6)};
	
	card.set_tweet(starting_point);
	player plyr(sf::Vector2f(room.width() / 2.f, room.height() / 2.f));
	plyr.m_s.setScale(2.f, 2.f);

	sf::Clock clock;

	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					window->close();
					break;
				case sf::Event::MouseWheelMoved:
					room.zoom(event.mouseWheel.delta / 10.f);
					break;
			}
		}

		window->clear();
		room.render(*window, video_mode);
		plyr.update(clock.restart(), room.floorplan());
		room.center_in(plyr.m_position);
		window->draw(plyr.m_s);
		card.render(*window, video_mode);

		draw_x(room.room_point_to_screen(plyr.m_position,                              video_mode), sf::Color::White, *window);
		draw_x(room.room_point_to_screen(plyr.m_position + sf::Vector2f{ -.3f, -.3f }, video_mode), sf::Color::Red, *window);
		draw_x(room.room_point_to_screen(plyr.m_position + sf::Vector2f{  .3f, -.3f }, video_mode), sf::Color::Green, *window);
		draw_x(room.room_point_to_screen(plyr.m_position + sf::Vector2f{ -.3f,  .3f }, video_mode), sf::Color::Blue, *window);
		draw_x(room.room_point_to_screen(plyr.m_position + sf::Vector2f{  .3f,  .3f }, video_mode), sf::Color::Yellow, *window);
//Comentario absurdo
		window->display();
	}
}

}