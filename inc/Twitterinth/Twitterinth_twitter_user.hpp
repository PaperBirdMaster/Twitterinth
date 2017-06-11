#ifndef TWITTERINT_TWITTER_USER
#define TWITTERINT_TWITTER_USER

// App includes.
#include "Twitter/Types.hpp"

namespace sf
{

class Sprite;

}

namespace Twitterinth
{

/**
*/
const Twitter::User &get_user(Twitter::id_t a_user_id);

/**
*/
sf::Sprite &get_profile_picture(Twitter::id_t a_user_id);

}

#endif