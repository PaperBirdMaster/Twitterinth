// App includes.
#include "Twitterinth_twitter_user.hpp"
#include "ProfilePicturesCache.hpp"
#include "json_keys.hpp"
#include "JSON/JSON.hpp"
#include "Twitter/User.hpp"
#include "Twitter/API.hpp"
#include "URL/DownloadFile.hpp"

// External lib includes.
#include "SFML/Graphics.hpp"

namespace Twitterinth
{

/* In order to avoid re-parsing users, we store the already parsed users
* of this session. */
Twitter::user_collection_t user_cache;

/**
* Parse the given JSON node in order to create a Twitter user, the
* newly created Twitter user is stored in the cache in order to avoid
* future unnecessary parsing.
*
* @param a_user_json : JSON node pointing to the Twitter user data.
* @return The Twitter user from the given JSON data.
*/
const Twitter::User &parse_user(const Twitterinth::JSON::node_t &a_user_json)
{
	Twitter::id_t user_id = a_user_json[ID].GetUint64();

	auto insertion = user_cache.emplace
	(
		Twitter::user_collection_t::value_type
		{
			user_id,
			{
				JSON::node_to_text  (a_user_json[NAME]                        ),
				JSON::node_to_text  (a_user_json[SCREEN_NAME]                 ),
				JSON::node_to_number(a_user_json[FOLLOWERS_COUNT]             ),
				JSON::node_to_number(a_user_json[FAVOURITES_COUNT]            ),
				JSON::node_to_color (a_user_json[PROFILE_BACKGROUND_COLOR]    ),
				JSON::node_to_color (a_user_json[PROFILE_LINK_COLOR]          ),
				JSON::node_to_color (a_user_json[PROFILE_SIDEBAR_BORDER_COLOR]),
				JSON::node_to_color (a_user_json[PROFILE_SIDEBAR_FILL_COLOR]  ),
				JSON::node_to_color (a_user_json[PROFILE_TEXT_COLOR]          ),
				JSON::node_to_string(a_user_json[PROFILE_BACKGROUND_IMAGE_URL]),
				JSON::node_to_string(a_user_json[PROFILE_BANNER_URL]          ),
				JSON::node_to_string(a_user_json[PROFILE_IMAGE_URL]           )
			}
		}
	);

	return insertion.first->second;
}

const Twitter::User &get_user(Twitter::id_t a_user_id)
{
	using Twitter::User;

	// Look for the user in the cache.
	auto user_found = user_cache.find(a_user_id);

	if (user_found == user_cache.end())
	{
		// User not found! Download user data.
		auto user_from_id = Twitter::get_user(a_user_id);

		if (user_from_id.IsNull())
		{
			return Twitter::User::Null;
		}

		// Save user in cache and return it.
		return parse_user(user_from_id);
	}

	// User found, we're done.
	return user_found->second;
}

sf::Sprite &get_profile_picture(Twitter::id_t a_user_id)
{
	// Local static in order to ensure lazy initialization.
	static ProfilePicturesCache pictures_cache;

	return pictures_cache.get_user_profile(a_user_id);
}

//sf::Sprite get_profile_banner(Twitter::id_t a_user_id)
std::string get_profile_banner(Twitter::id_t a_user_id)
{
	return "";
}

}
