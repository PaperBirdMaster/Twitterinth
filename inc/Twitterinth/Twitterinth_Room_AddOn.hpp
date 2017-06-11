#ifndef TWITTERINTH_ROOM_ADDON_PUBLIC_HPP
#define TWITTERINTH_ROOM_ADDON_PUBLIC_HPP

// STD includes.
#include <cstdint>
#include <cstdio>

#define NOMINMAX // Read as "I hate you"...

// OS includes.
#include <Windows.h>

//! Room tile type is 32bit unsigned integer.
using tile_t = std::uint32_t;

/** Door tiles connects different rooms.
* This is a helper funtion to create a Door tile id from a number.
* @param a_id Tile id to be transformed to Door id.
* @return Door Tile id.
*/
constexpr tile_t AS_DOOR(tile_t a_id)
{ return (a_id & 0xffffu) + 0x10000u; }

/** Pit tiles are untransitable unless flying, sometimes you can see what's below current room through it!
* This is a helper funtion to create a Pit tile id from a number.
* @param a_id Tile id to be transformed to Floor id.
* @return Pit Tile id.
*/
constexpr tile_t AS_PIT(tile_t a_id)
{ return (a_id & 0xffffu) + 0x20000u; }

/** Floor tiles are the ones where the player and enemies can stand and move.
* This is a helper funtion to create a Floor tile id from a number.
* @param a_id Tile id to be transformed to Floor id.
* @return Floor Tile id.
*/
constexpr tile_t AS_FLOOR(tile_t a_id)
{ return (a_id & 0xffffu) + 0x30000u; }

/** Player and enemies can stand and move over this tiles but they can also be destroyed by powerful attacks leaving a PIT.
* This is a helper funtion to create a Destructible Floor tile id from a number.
* @param a_id Tile id to be transformed to Destructible Floor id.
* @return Destructible Floor Tile id.
*/
constexpr tile_t AS_DESTRUCTIBLE_FLOOR(tile_t a_id)
{ return (a_id & 0xffffu) + 0x40000u; }

/** Player and enemies cannot move on this tiles.
* This is a helper funtion to create a Wall tile id from a number.
* @param a_id Tile id to be transformed to Wall id.
* @return Wall Tile id.
*/
constexpr tile_t AS_WALL(tile_t a_id)
{ return (a_id & 0xffffu) + 0x50000u; }

/** Player and enemies cannot move on this tiles but they can be destroyed to (maybe) reveal secrets.
* This is a helper funtion to create a Secret Wall tile id from a number.
* @param a_id Tile id to be transformed to Secret Wall id.
* @return Secret Wall Tile id.
*/
constexpr tile_t AS_SECRET_WALL(tile_t a_id)
{ return (a_id & 0xffffu) + 0x60000u; }

/** Player and enemies cannot move on this tiles but they can be destroyed and it should be obvious that you can do it.
* This is a helper funtion to create a Destructible Wall tile id from a number.
* @param a_id Tile id to be transformed to Destructible Wall id.
* @return Destructible Wall Tile id.
*/
constexpr tile_t AS_DESTRUCTIBLE_WALL(tile_t a_id)
{ return (a_id & 0xffffu) + 0x70000u; }

/** Player and enemies cannot move on this tiles but they can be pulled or pushed.
* This is a helper funtion to create a Moveable Wall tile id from a number.
* @param a_id Tile id to be transformed to Moveable Wall id.
* @return Moveable Wall Tile id.
*/
constexpr tile_t AS_MOVEABLE_WALL(tile_t a_id)
{ return (a_id & 0xffffu) + 0x80000u; }

/** 
*/
constexpr bool IS_DOOR(tile_t a_id)
{
	return (a_id >= 0x10000u) && (a_id <= 0x1ffffu);
}

/** 
*/
constexpr bool IS_PIT(tile_t a_id)
{
	return (a_id >= 0x20000u) && (a_id <= 0x2ffffu);
}

/** 
*/
constexpr bool IS_FLOOR(tile_t a_id)
{
	return (a_id >= 0x30000u) && (a_id <= 0x3ffffu);
}

/**
*/
constexpr bool IS_DESTRUCTIBLE_FLOOR(tile_t a_id)
{
	return (a_id >= 0x40000u) && (a_id <= 0x4ffffu);
}

/**
*/
constexpr bool IS_WALL(tile_t a_id)
{
	return (a_id >= 0x50000u) && (a_id <= 0x5ffffu);
}

/**
*/
constexpr bool IS_SECRET_WALL(tile_t a_id)
{
	return (a_id >= 0x60000u) && (a_id <= 0x6ffffu);
}

/**
*/
constexpr bool IS_DESTRUCTIBLE_WALL(tile_t a_id)
{
	return (a_id >= 0x70000u) && (a_id <= 0x7ffffu);
}

/**
*/
constexpr bool IS_MOVEABLE_WALL(tile_t a_id)
{
	return (a_id >= 0x80000u) && (a_id <= 0x8ffffu);
}

//! Unknown tile type: default out-of-bounds tile type or type returned on error.
constexpr static tile_t UNKNOWN            = 0u;
constexpr static tile_t DOOR               = AS_DOOR(0u);
constexpr static tile_t PIT                = AS_PIT(0u);
constexpr static tile_t FLOOR              = AS_FLOOR(0u);
constexpr static tile_t DESTRUCTIBLE_FLOOR = AS_DESTRUCTIBLE_FLOOR(0u);
constexpr static tile_t WALL               = AS_WALL(0u);
constexpr static tile_t SECRET_WALL        = AS_SECRET_WALL(0u);
constexpr static tile_t DESTRUCTIBLE_WALL  = AS_DESTRUCTIBLE_WALL(0u);
constexpr static tile_t MOVEABLE_WALL      = AS_MOVEABLE_WALL(0u);

/** TODO
* @param NAME
* @param CREATOR
* @param VERSION
* @param TEXTURE_FILE
* @param TILE_SIZE
*/
#define ADDON_INFO(NAME, CREATOR, VERSION, TEXTURE_FILE, TILE_SIZE) __declspec(dllexport) void __cdecl addon_info(char (&NAME)[50], char (&CREATOR)[20], std::uint32_t *VERSION, char (&TEXTURE_FILE)[FILENAME_MAX], std::uint32_t *TILE_SIZE)

/** TODO
* @param TILE_ID
* @param X
* @param Y
*/
#define GET_TILE(TILE_ID, X, Y) __declspec(dllexport) void __cdecl get_tile(std::uint32_t TILE_ID, std::uint32_t *X, std::uint32_t *Y)

/** Create the room floorplan using the values above.
* @param MAP_TILES pointer to a dynamic-allocated array with enough room to fit TILE_COUNT elements.
* @param TILE_COUNT amount of tiles allocated in MAP_TILES pointer.
* @param ROOM_WIDTH width of the room to be generated.
* @param ROOM_HEIGTH heigth of the room to be generated.
* @param TWEET_TEXT text of the tweet for this room.
*/
#define CREATE_ROOM(MAP_TILES, TILE_COUNT, ROOM_WIDTH, ROOM_HEIGTH, TWEET_TEXT) __declspec(dllexport) void __cdecl create_room(std::uint32_t *MAP_TILES, std::uint32_t TILE_COUNT, std::uint32_t ROOM_WIDTH, std::uint32_t ROOM_HEIGTH, const char *TWEET_TEXT)

#endif