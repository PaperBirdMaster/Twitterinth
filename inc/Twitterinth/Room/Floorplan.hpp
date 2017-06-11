#ifndef TWITTERINT_FLOORPLAN_HPP
#define TWITTERINT_FLOORPLAN_HPP

// App includes.
#include "as_2D.h"

// STD includes.
#include <cstdint>
#include <vector>

namespace Twitterinth
{

struct Floorplan
{
	//!
	struct Tile_position;
	//!
	struct Room;

	//!
	using tile_t = std::uint32_t;
	//!
	using data_t = Wrapper::as_2D<tile_t *>;
	//!
	using index_t = data_t::index_t;
	//!
	using tile_collection_t = std::vector<Tile_position>;
	//!
	using room_collection_t = std::vector<Room>;

	/**
	*/
	Floorplan(tile_t *&a_data, index_t a_width, index_t a_height);

	/**
	*/
	tile_collection_t select(tile_t a_tile_type, bool(*a_attached)(tile_t (&)[4])) const;

	/**
	*/
	tile_collection_t select(tile_t a_tile_type, bool(*a_neighbors)(tile_t (&)[8])) const;

	/**
	*/
	tile_collection_t select(tile_t a_tile_type) const;

	/**
	*/
	room_collection_t select_rooms(tile_t a_tile_type) const;

	/**
	*/
	index_t floodfill(tile_t a_tile_type, index_t a_x, index_t a_y);

	/**
	*/
	void set(const tile_collection_t &a_tiles, tile_t a_tile_type);

	/**
	*/
	void set(const Tile_position &a_from, const Tile_position &a_to, tile_t a_tile_type);

	/**
	*/
	void set(index_t a_x1, index_t a_y1, index_t a_x2, index_t a_y2, tile_t a_tile_type);

	/**
	*/
	index_t width() const;

	/**
	*/
	index_t height() const;

	/**
	*/
	index_t size() const;

	/**
	*/
	tile_t &operator ()(index_t a_x, index_t a_y);

	/**
	*/
	const tile_t &operator ()(index_t a_x, index_t a_y) const;

	/**
	*/
	tile_t &operator ()(const Tile_position &a_tile_position);

	/**
	*/
	const tile_t &operator ()(const Tile_position &a_tile_position) const;

private:

	/**
	*/
	index_t fill(tile_t a_original_tile, tile_t a_new_tile, index_t a_x, index_t a_y);

	/**
	*/
	tile_collection_t floodfill_select(tile_t a_tile_type, index_t a_x, index_t a_y) const;

	//!
	data_t m_data;
};

struct Floorplan::Tile_position
{
	//!
	const index_t m_x{};
	//!
	const index_t m_y{};

	/**
	*/
	Tile_position(index_t a_x, index_t a_y);
};

struct Floorplan::Room
{
	/**
	*/
	Tile_position up_left() const;
	/**
	*/
	Tile_position down_right() const;
	/**
	*/
	index_t tile_count() const;
	/**
	*/
	bool contains(const Tile_position &a_tile_position) const;
	/**
	*/
	const tile_collection_t &tiles() const;

	/**
	*/
	Room(tile_collection_t &&a_tiles);

private:
	//!
	tile_collection_t m_tiles;
};

}

#endif