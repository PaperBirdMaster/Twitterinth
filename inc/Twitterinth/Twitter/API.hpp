#ifndef TWITTER_API
#define TWITTER_API

// App includes
#include "Twitter/Types.hpp"
#include "JSON/JSON.hpp"

namespace Twitter
{

/** Twitter API URL to get authenticaded user timeline.
* Returns a collection of the most recent Tweets and retweets posted by the authenticating user and the users they follow.
* The home timeline is central to how most users interact with the Twitter service.
* More info: https://dev.twitter.com/rest/reference/get/statuses/home_timeline
*/
rapidjson::Document get_home_timeline(std::uint32_t a_tweet_count);

/** Twitter API URL to get Twitter user information.
* Returns a variety of information about the user specified by the required user_id or screen_name parameter.
* The author’s most recent Tweet will be returned inline when possible.
* More info: https://dev.twitter.com/rest/reference/get/users/show
*/
rapidjson::Document get_user(id_t a_user_id);

/** Twitter API URL to get Tweet information from Tweet id.
* Returns a single Tweet, specified by the id parameter.
* The Tweet’s author will also be embedded within the Tweet.
* More info: https://dev.twitter.com/rest/reference/get/statuses/show/id
*/
rapidjson::Document get_tweet(id_t a_tweet_id);

}

#endif