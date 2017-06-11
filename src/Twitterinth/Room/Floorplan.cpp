// App includes.
#include "Room/Floorplan.hpp"
#include "Twitterinth_Room_AddOn.hpp"

// STL includes.
#include <algorithm>
#include <functional>
#include <tuple>
#include <set>

namespace Twitterinth
{

bool operator ==(const Floorplan::Tile_position a_left, const Floorplan::Tile_position a_right)
{
	return (a_left.m_x == a_right.m_x) && (a_left.m_y == a_right.m_y);
}

bool operator <(const Floorplan::Tile_position a_left, const Floorplan::Tile_position a_right)
{
	return std::tie(a_left.m_x, a_left.m_y) < std::tie(a_right.m_x, a_right.m_y);
}

bool operator <(const Floorplan::Room a_left, const Floorplan::Room a_right)
{
	const auto &lc = a_left.tile_count();
	const auto &rc = a_right.tile_count();

	return lc == rc ? a_left.up_left() < a_right.up_left() : lc > rc;
}

Floorplan::Floorplan(tile_t *&a_data, index_t a_width, index_t a_height) :
	m_data{a_data, a_width, a_height}
{
}

Floorplan::tile_collection_t Floorplan::select(tile_t a_tile_type, bool(*a_attached)(tile_t(&)[4])) const
{
	tile_collection_t result{};

	for (index_t y = 0, height = m_data.height(); y != height; ++y)
	{
		for (index_t x = 0, width = m_data.width(); x != width; ++x)
		{
			if (m_data(x, y) == a_tile_type)
			{
				tile_t attached[4]
				{
					y ?           m_data(x, y - 1) : UNKNOWN, // N
					x != width ?  m_data(x + 1, y) : UNKNOWN, // E
					y != height ? m_data(x, y + 1) : UNKNOWN, // S
					x ?           m_data(x - 1, y) : UNKNOWN  // W
				};

				if (a_attached(attached))
				{
					result.emplace_back(x, y);
				}
			}
		}
	}

	return result;
}

Floorplan::tile_collection_t Floorplan::select(tile_t a_tile_type, bool(*a_neighbors)(tile_t(&)[8])) const
{
	tile_collection_t result{};

	for (index_t y = 0, height = m_data.height(); y != height; ++y)
	{
		for (index_t x = 0, width = m_data.width(); x != width; ++x)
		{
			if (m_data(x, y) == a_tile_type)
			{
				tile_t attached[8]
				{
					y ?                             m_data(x, y - 1)     : UNKNOWN, // N
					y && (x != width) ?             m_data(x + 1, y - 1) : UNKNOWN, // NE
					x != width ?                    m_data(x + 1, y)     : UNKNOWN, // E
					(x != width) && (y != height) ? m_data(x + 1, y + 1) : UNKNOWN, // SE
					y != height ?                   m_data(x, y + 1)     : UNKNOWN, // S
					x && (y != height) ?            m_data(x - 1, y + 1) : UNKNOWN, // SW
					x ?                             m_data(x - 1, y)     : UNKNOWN, // W
					x && y ?                        m_data(x - 1, y - 1) : UNKNOWN  // NW
				};

				if (a_neighbors(attached))
				{
					result.emplace_back(x, y);
				}
			}
		}
	}

	return result;
}

Floorplan::tile_collection_t Floorplan::select(tile_t a_tile_type) const
{
	tile_collection_t result{};
	const auto &data = m_data.m_data;
	const auto height = m_data.height();
	const auto width = m_data.width();
	const auto count = m_data.count();

	for (index_t tile = 0u; tile != count; ++tile)
	{
		if (data[tile] == a_tile_type)
		{
			result.emplace_back(tile % width, tile / width);
		}
	}

	return result;
}

Floorplan::room_collection_t Floorplan::select_rooms(tile_t a_tile_type) const
{
	std::multiset<Room> result{};
	const auto height = m_data.height();
	const auto width = m_data.width();

	for (index_t y = 0; y < height; ++y)
	{
		for (index_t x = 0; x < width; ++x)
		{
			if ((m_data(x, y) == a_tile_type) && !std::any_of(result.begin(), result.end(), [&](const auto &a_room) { return a_room.contains({ x, y }); }))
			{
				result.emplace(floodfill_select(a_tile_type, x, y));
			}
		}
	}

	return { result.begin(), result.end() };
}

Floorplan::index_t Floorplan::floodfill(tile_t a_tile_type, index_t a_x, index_t a_y)
{
	return fill(m_data(a_x, a_y), a_tile_type, a_x, a_y);
}

void Floorplan::set(const tile_collection_t &a_tiles, tile_t a_tile_type)
{
	for (const auto &tile_position : a_tiles)
	{
		m_data(tile_position.m_x, tile_position.m_y) = a_tile_type;
	}
}

void Floorplan::set(const Tile_position &a_from, const Tile_position &a_to, tile_t a_tile_type)
{
	const index_t w = a_to.m_x - a_from.m_x;
	const index_t h = a_to.m_y - a_from.m_y;

	for (index_t y = 0; y < h; ++y)
	{
		for (index_t x = 0; x < w; ++x)
		{
			m_data(a_from.m_x + x, a_from.m_y + y);
		}
	}
}

void Floorplan::set(index_t a_x1, index_t a_y1, index_t a_x2, index_t a_y2, tile_t a_tile_type)
{
	const index_t w = a_x2 - a_x1;
	const index_t h = a_y2 - a_y1;

	for (index_t y = 0; y < h; ++y)
	{
		for (index_t x = 0; x < w; ++x)
		{
			m_data(a_x1 + x, a_y1 + y) = a_tile_type;
		}
	}
}

Floorplan::index_t Floorplan::width() const
{
	return m_data.width();
}

Floorplan::index_t Floorplan::height() const
{
	return m_data.height();
}

Floorplan::index_t Floorplan::size() const
{
	return m_data.count();
}

Floorplan::tile_t &Floorplan::operator ()(index_t a_x, index_t a_y)
{
	return m_data(a_x, a_y);
}

const Floorplan::tile_t &Floorplan::operator ()(index_t a_x, index_t a_y) const
{
	return m_data(a_x, a_y);
}

Floorplan::tile_t &Floorplan::operator ()(const Floorplan::Tile_position &a_tile_position)
{
	return m_data(a_tile_position.m_x, a_tile_position.m_y);
}

const Floorplan::tile_t &Floorplan::operator ()(const Floorplan::Tile_position &a_tile_position) const
{
	return m_data(a_tile_position.m_x, a_tile_position.m_y);
}

Floorplan::index_t Floorplan::fill(tile_t a_original_tile, tile_t a_new_tile, index_t a_x, index_t a_y)
{
	auto &tile = m_data(a_x, a_y);
	const auto height = m_data.height();
	const auto width = m_data.width();
	int result{};

	if ((tile == a_original_tile) && (tile != a_new_tile))
	{
		tile = a_new_tile;
		result = 1;
		if (a_x)          result += fill(a_original_tile, a_new_tile, a_x - 1, a_y);
		if (a_y)          result += fill(a_original_tile, a_new_tile, a_x, a_y - 1);
		if (a_x < width)  result += fill(a_original_tile, a_new_tile, a_x + 1, a_y);
		if (a_y < height) result += fill(a_original_tile, a_new_tile, a_x, a_y + 1);
	}

	return result;
}

Floorplan::tile_collection_t Floorplan::floodfill_select(tile_t a_tile_type, index_t a_x, index_t a_y) const
{
	tile_collection_t result{};
	const auto height = m_data.height();
	const auto width = m_data.width();

	std::vector<char> tile_checked(width * height, false);
	Wrapper::as_2D<std::vector<char>> already_checked{tile_checked, width, height};

	std::function<void(index_t, index_t)> recursive_call;

	recursive_call = [&](index_t a_x, index_t a_y)
	{
		const auto &tile = (*this)(a_x, a_y);
		auto &checked = already_checked(a_x, a_y);

		if ((tile == a_tile_type) && !checked)
		{
			result.emplace_back(a_x, a_y);
			checked = true;

			if (a_x)          recursive_call(a_x - 1, a_y);
			if (a_y)          recursive_call(a_x, a_y - 1);
			if (a_x < width)  recursive_call(a_x + 1, a_y);
			if (a_y < height) recursive_call(a_x, a_y + 1);
		}
	};

	recursive_call(a_x, a_y);

	return result;
}


/* Tile Position */

Floorplan::Tile_position::Tile_position(index_t a_x, index_t a_y) :
	m_x{ a_x },
	m_y{ a_y }
{
}

/* Room */

Floorplan::Tile_position Floorplan::Room::up_left() const
{
	index_t x{ std::numeric_limits<index_t>::max() },
			y{ std::numeric_limits<index_t>::max() };

	for (const auto &tile : m_tiles)
	{
		x = std::min(x, tile.m_x);
		y = std::min(y, tile.m_y);
	}

	return { x, y };
}

Floorplan::Tile_position Floorplan::Room::down_right() const
{
	index_t x{}, y{};

	for (const auto &tile : m_tiles)
	{
		x = std::max(x, tile.m_x);
		y = std::max(y, tile.m_y);
	}

	return{ x, y };
}

Floorplan::index_t Floorplan::Room::tile_count() const
{
	return m_tiles.size();
}

bool Floorplan::Room::contains(const Tile_position &a_tile_position) const
{
	return std::find(m_tiles.begin(), m_tiles.end(), a_tile_position) != m_tiles.end();
}

const Floorplan::tile_collection_t &Floorplan::Room::tiles() const
{
	return m_tiles;
}

Floorplan::Room::Room(tile_collection_t &&a_tiles) :
	m_tiles {a_tiles}
{
}

}
