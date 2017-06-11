// App includes
#include "Twitterinth_twitter_tweet.hpp"
#include "json_keys.hpp"
#include "JSON/JSON.hpp"
#include "Twitter/Tweet.hpp"
#include "Twitter/API.hpp"

namespace std
{

using Twitterinth::JSON::begin;
using Twitterinth::JSON::end;

}

namespace Twitterinth
{

//!
Twitter::tweet_collection_t tweet_cache;

/**
* Parse the given JSON node in order to create a Tweet, the newly
* created Tweet is stored in the cache in order to avoid future
* unnecessary parsing.
*
* @param a_user_json : JSON node pointing to the Tweet data.
* @return The Tweet user from the given JSON data.
*/
const Twitter::Tweet &parse_tweet(const Twitterinth::JSON::node_t &a_tweet_json)
{
	Twitter::id_t tweet_id = a_tweet_json[ID].GetUint64();

	if (tweet_cache.find(tweet_id) == tweet_cache.end())
	{
		Twitter::hashtag_collection_t tweet_hashtags;
		for (const auto &hashtag : a_tweet_json[ENTITIES][HASHTAGS])
		{
			tweet_hashtags.push_back(JSON::node_to_text(hashtag[TEXT]));
		}

		Twitter::id_collection_t tweet_mentions;
		for (const auto &mention : a_tweet_json[ENTITIES][USER_MENTIONS])
		{
			Twitter::id_t mentioned_user{};
			if (mentioned_user = JSON::node_to_id(mention[ID]))
			{
				tweet_mentions.push_back(mentioned_user);
				Twitter::get_user(mentioned_user);
			}
		}

		Twitter::id_collection_t tweet_media;
		//if (a_tweet_json.HasMember(EXTENDED_ENTITIES) && a_tweet_json[EXTENDED_ENTITIES].HasMember(MEDIA))
		//{
		//	for (const auto &media : a_tweet_json[EXTENDED_ENTITIES][MEDIA])
		//	{
		//		Twitter::id_t media_id{};
		//		if (media_id = JSON::node_to_id(media[ID]))
		//		{
		//			tweet_media.push_back(media_id);
		//			parse_media(media, media_id);
		//		}
		//	}
		//}

		Twitter::id_t quoting_id{};
		if (a_tweet_json.HasMember(QUOTED_STATUS) && (quoting_id = JSON::node_to_id(a_tweet_json[QUOTED_STATUS_ID])))
		{
			parse_tweet(a_tweet_json[QUOTED_STATUS]);
		}

		Twitter::id_t in_reply_to_id{};
		if (in_reply_to_id = JSON::node_to_id(a_tweet_json[IN_REPLY_TO_STATUS_ID]))
		{
			Twitter::get_tweet(in_reply_to_id);
		}

		auto insertion = tweet_cache.emplace
		(
			Twitter::tweet_collection_t::value_type
			{
				tweet_id,
				{
					JSON::node_to_text(a_tweet_json[TEXT]),
					JSON::node_to_id(a_tweet_json[USER][ID]),
					JSON::node_to_number(a_tweet_json[RETWEET_COUNT]),
					JSON::node_to_number(a_tweet_json[FAVORITE_COUNT]),
					tweet_hashtags,
					tweet_mentions,
					tweet_media,
					quoting_id,
					in_reply_to_id
				}
			}
		);

		return insertion.first->second;
	}

	return Twitter::Tweet::Null;
}

const Twitter::Tweet &get_start_point()
{
	auto home_timeline = Twitter::get_home_timeline(1);

	if (home_timeline.IsNull())
	{
		return Twitter::Tweet::Null;
	}
	else if (home_timeline.IsArray() && !home_timeline.Empty())
	{
		return parse_tweet(home_timeline[0]);
	}

	return Twitter::Tweet::Null;
}

const Twitter::Tweet &get_tweet(Twitter::id_t a_tweet_id)
{
	using Twitter::Tweet;

	auto tweet_found = tweet_cache.find(a_tweet_id);

	if (tweet_found == tweet_cache.end())
	{
		auto tweet_from_id = Twitter::get_tweet(a_tweet_id);

		if (tweet_from_id.IsNull())
		{
			return Tweet::Null;
		}

		return parse_tweet(tweet_from_id);
	}
	else
	{
		return tweet_found->second;
	}

	return Tweet::Null;
}

}
