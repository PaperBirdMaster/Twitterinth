// App includes.
#include "Cavern.hpp"

// Twitterinth includes.
#include "Types.hpp"

// External lib includes.
#include "Room/Floorplan.hpp"

// STD includes.
#include <algorithm>
#include <tuple>

ADDON_INFO(a_name, a_creator, a_version, a_texture_file, a_tile_size)
{
	static constexpr char NAME[]        { "Caverns of Twitterinth" };
	static constexpr char CREATOR[]     { "@PaperBirdMaster"       };
	static constexpr char TEXTURE_FILE[]{ "Tileset.png"            };

	std::copy(std::begin(NAME), std::end(NAME), std::begin(a_name));
	std::copy(std::begin(CREATOR), std::end(CREATOR), std::begin(a_creator));
	*a_version = 0x01'00'00'00;
	std::copy(std::begin(TEXTURE_FILE), std::end(TEXTURE_FILE), std::begin(a_texture_file));
	*a_tile_size = 32u;
}

GET_TILE(a_tile_id, a_x, a_y)
{
	auto set = [&](auto x, auto y) { *a_x = x; *a_y = y; };

	switch (a_tile_id)
	{
		default:
		case AS_WALL( 0u): set( 992u, 544u);  break;
		case AS_WALL( 1u): set(1120u, 544u); break;
		case AS_WALL( 2u): set(1248u, 544u); break;
		case AS_WALL( 3u): set(1664u, 544u); break;
		case AS_WALL( 4u): set(1696u, 544u); break;
		case AS_WALL( 5u): set(1728u, 544u); break;
		case AS_WALL( 6u): set(1760u, 544u); break;
		case AS_WALL( 7u): set(1792u, 544u); break;
		case AS_WALL( 8u): set(1824u, 544u); break;
		case AS_WALL( 9u): set(1856u, 544u); break;
		case AS_WALL(10u): set(1888u, 544u); break;
		case AS_WALL(11u): set(1920u, 544u); break;
		case AS_WALL(12u): set(1952u, 544u); break;
		case AS_WALL(13u): set(1984u, 544u); break;
		case AS_WALL(14u): set(2016u, 544u); break;
		case AS_WALL(15u): set(1280u, 576u); break;
		case AS_WALL(16u): set(1312u, 576u); break;
		case AS_WALL(17u): set(1344u, 576u); break;
		case AS_WALL(18u): set(1376u, 576u); break;
		case AS_WALL(19u): set(1408u, 576u); break;
		case AS_WALL(20u): set(1440u, 576u); break;
		case AS_WALL(21u): set(1472u, 576u); break;

		case AS_SECRET_WALL(0u): set(1024u, 544u); break;
		case AS_SECRET_WALL(1u): set(1056u, 544u); break;
		case AS_SECRET_WALL(2u): set(1088u, 544u); break;
		case AS_SECRET_WALL(3u): set(1152u, 544u); break;
		case AS_SECRET_WALL(4u): set(1184u, 544u); break;
		case AS_SECRET_WALL(5u): set(1216u, 544u); break;
		case AS_SECRET_WALL(6u): set(1280u, 544u); break;
		case AS_SECRET_WALL(7u): set(1312u, 544u); break;
		case AS_SECRET_WALL(8u): set(1344u, 544u); break;

		case AS_DESTRUCTIBLE_WALL(0u): set( 992u, 448u); break;
		case AS_DESTRUCTIBLE_WALL(1u): set(1024u, 448u); break;
		case AS_DESTRUCTIBLE_WALL(2u): set(1056u, 448u); break;
		case AS_DESTRUCTIBLE_WALL(3u): set(1088u, 448u); break;

		case DOOR: set(1312u, 480u); break;

		case AS_FLOOR( 0u): set( 544u, 448u); break;
		case AS_FLOOR( 1u): set( 576u, 448u); break;
		case AS_FLOOR( 2u): set( 608u, 448u); break;
		case AS_FLOOR( 3u): set( 640u, 448u); break;
		case AS_FLOOR( 4u): set( 928u, 448u); break;
		case AS_FLOOR( 5u): set( 960u, 448u); break;
		case AS_FLOOR( 6u): set( 992u, 448u); break;
		case AS_FLOOR( 7u): set(1024u, 448u); break;
		case AS_FLOOR( 8u): set(1056u, 448u); break;
		case AS_FLOOR( 9u): set(1088u, 448u); break;
		case AS_FLOOR(10u): set(1120u, 448u); break;
		case AS_FLOOR(11u): set(1152u, 448u); break;

		case PIT: set(1824u, 0u); break;
	}
}

using Twitterinth::Floorplan;
using index_t = Floorplan::index_t;

CREATE_ROOM(a_map_tiles, a_tile_count, a_width, a_heigth, a_tweet_text)
{
	const auto center_x = a_width  / 2u;
	const auto center_y = a_heigth / 2u;
	Floorplan floorplan(a_map_tiles, a_width, a_heigth);
	std::string text = a_tweet_text;
	auto carry = text.length() + 1u;

	auto get_value = [&carry, &text](int other_value)
	{
		int result = other_value;
		if (text.empty())
		{
			carry = result = std::abs(static_cast<int>(carry ^ other_value));
		}
		else
		{
			result = std::abs(text.back());
			text.pop_back();
		}
		return result;
	};

	// Fill the full map with walls and...
	std::fill(a_map_tiles, a_map_tiles + (a_width * a_heigth), WALL);
	// ... make a hole in the middle:
	floorplan.set(center_x - (a_width / 8u), center_y - (a_heigth / 8u), center_x + (a_width / 8u), center_y + (a_heigth/ 8u), AS_FLOOR(0u));

	std::vector<int> unused_values{};

	/* We use the character value with sinus and cosinus in order to get a displacement 'angle'
	* from the center, the resulting point will become floor tile and will be transitable thereof. */
	for (int step = 0, last_step = std::max<int>(text.size(), 10); step != last_step; ++step)
	{
		bool value_used = false;
		auto value = get_value(step);
		double x = std::sin(value);
		double y = std::cos(value);
		double step_x = std::min(a_width, a_heigth);
		double step_y = step_x;
		step_x /= a_heigth;
		step_y /= a_width;

		for (int tile = 0, last_tile = std::max(center_x, center_y) - 1; !value_used && (tile != last_tile); ++tile)
		{
			auto &cell = floorplan(static_cast<index_t>(center_x - (x * step_x * tile)), static_cast<index_t>(center_y - (y * step_y * tile)));

			if ((value_used = (cell == WALL)))
			{
				cell = AS_FLOOR(0u);
			}
		}

		if (!value_used)
		{
			unused_values.push_back(value);
		}
	}

	/* Search for rooms, the one in first position would be the bigger one,
	* hence it will be the reference one and the one with all the doors.
	* Smaller rooms will be used to hide bonuses and rewards, also small rooms
	* should be connected with destructible walls. */
	auto rooms = floorplan.select_rooms(AS_FLOOR(0u));

	if (rooms.size() > 1u)
	{
		// Mark rooms...
		index_t id = 0u;
		for (auto room = rooms.cbegin() + 1u, last_room = rooms.cend(); room != last_room; ++room)
		{
			floorplan.set(room->tiles(), AS_FLOOR(++id));
		}

		/* ... detect possible room joiners, they will be changed into
		* destructible walls. */
		auto candidates = floorplan.select(WALL, [](tile_t(&a_attached)[4])
		{
			auto last = std::remove(a_attached, a_attached + 4, WALL);
			last = std::remove(a_attached, last, UNKNOWN);
			// 'obstacle' tiles with attached tiles from different rooms.
			return std::count(a_attached, last, *a_attached) < std::distance(a_attached, last);
		});

		// Change the joiners into destructible walls
		for (const auto &d_wall : candidates)
		{
			floorplan(d_wall) = get_value(d_wall.m_x + d_wall.m_y) % 19 ? SECRET_WALL : DESTRUCTIBLE_WALL;
		}
	}

	// Set door blocks.
	floorplan.set(floorplan.select(WALL, [](tile_t(&a_attached)[4])
	{
		/* 'obstacle' tiles with no attached room joiners, exactly 1 floor tile (from main room)
		* and wit two or more attached 'obstacle' tiles. */
		return (((std::count(a_attached, a_attached + 4, SECRET_WALL) == 0) &&
			(std::count(a_attached, a_attached + 4, DESTRUCTIBLE_WALL) == 0) &&
			std::count(a_attached, a_attached + 4, AS_FLOOR(0u)) == 1) &&
			(std::count(a_attached, a_attached + 4, WALL) >= 2));
	}), DOOR);

	if (rooms.size() > 1u)
	{
		for (auto room = rooms.cbegin() + 1u, last_room = rooms.cend(); room != last_room; ++room)
		{
			floorplan.set(room->tiles(), AS_FLOOR(0u));
		}
	}
}
