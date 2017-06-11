#ifndef TWITTERINTH_TYPES_HPP
#define TWITTERINTH_TYPES_HPP

// App includes.
#include "Twitterinth_Room_AddOn.hpp"

namespace Twitterinth
{

//!
struct Room;

//!
struct Camera;

//!
struct Card;

enum class room_tile : std::uint32_t
{
	unknown           = UNKNOWN,
//	walkable          = WALKABLE,
//	flyable           = FLYABLE,
//	destroyable       = DESTROYABLE,
//	deals_damage      = DEALS_DAMAGE,
//	deals_dot         = DEALS_DOT,
//	slows_down        = SLOWS_DOWN,
//	slippery          = SLIPPERY,
//	stun              = STUN,
//	push              = PUSH,
//	attracts          = ATTRACTS,
//
//	wall              = WALL,
//	floor             = FLOOR,
//	destroyable_wall  = DESTROYABLE_WALL,
//	destroyable_floor = DESTROYABLE_FLOOR,
//	pit               = PIT,
	door              = DOOR,
};

}

#endif