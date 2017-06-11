#ifndef TWITTER_TYPES_HPP
#define TWITTER_TYPES_HPP

// STD includes
#include <string>
#include <cstdint>
#include <vector>
#include <map>

namespace Twitter
{

/** Twitter user data, it stores information about the user name and profile information.
*/
struct User;

/** Tweet data, it stores the Tweet text, RT's, Fav's, Mentions...
*/
struct Tweet;

/** Tweet data, it stores the Tweet text, RT's, Fav's, Mentions...
*/
struct Media;

enum class media_type : std::uint32_t
{
	unknown,
	photo,
	animated_gif,
	video,
};

/** Twitter Snowflake IDs (64b).
* Snowflake is a service used to generate unique IDs for objects within Twitter (Tweets, Direct Messages, Users, Collections, Lists etc.).
* These IDs are unique 64-bit unsigned integers, which are based on time, instead of being sequential.
* The full ID is composed of a timestamp, a worker number, and a sequence number.
* More info: http://dev.twitter.com/overview/api/twitter-ids-json-and-snowflake
*/
using id_t = std::uint64_t;

/**
*/
template <typename twitter_entity>
using id_map_t = std::map<id_t, twitter_entity>;

using id_collection_t = std::vector<id_t>;

/** Type to store text from Twitter. Using the widest type in order to ensure that all codifications will fit.
*/
using text_t = std::string;

/** Type to store text for internal uses, usually not meant to be showed to the user (i.e.: urls to resources).
*/
using string_t = std::string;

/** Collection of #hashtags, usually related to a Tweet.
*/
using hashtag_collection_t = std::vector<text_t>;

/** Collection of Twitter users.
*/
using user_collection_t = id_map_t<User>;

/** Collection of media items.
*/
using media_collection_t = id_map_t<Media>;

/** Collection of Tweets.
*/
using tweet_collection_t = id_map_t<Tweet>;

}

#endif