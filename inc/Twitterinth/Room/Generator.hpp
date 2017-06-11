#ifndef TWITTERINTH_ROOM_GENERATOR
#define TWITTERINTH_ROOM_GENERATOR

// App includes
#include "Types.hpp"
#include "Twitter/Types.hpp"

namespace Twitterinth
{

Room create_room(const Twitter::Tweet &a_tweet);

}

#endif