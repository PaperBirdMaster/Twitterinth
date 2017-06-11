#ifndef TWITTERINT_CAVERN_HPP
#define TWITTERINT_CAVERN_HPP

#include "Twitterinth_Room_AddOn.hpp"

extern "C"
{

ADDON_INFO(a_name, a_creator, a_version, a_texture_file, a_tile_size);

GET_TILE(a_tile_id, a_x, a_y);

CREATE_ROOM(a_map_tiles, a_tile_count, a_width, a_heigth, a_tweet_text);

}

#endif