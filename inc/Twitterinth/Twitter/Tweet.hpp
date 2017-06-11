#ifndef TWITTER_TWEET_HPP
#define TWITTER_TWEET_HPP

// App includes
#include "Twitter/Types.hpp"

// STD includes
#include <cstdint>

namespace Twitter
{

struct Tweet
{
	Tweet(text_t a_text, id_t a_author, std::uint32_t a_retweets, std::uint32_t a_favorites, hashtag_collection_t a_hashtags, id_collection_t a_mentions, id_collection_t a_media, id_t a_quote, id_t a_in_reply_to);

	const text_t &text() const;

	const id_t author() const;

	const id_t in_reply_to() const;

	const id_t quote() const;

	static const Tweet Null;

private:
	Tweet() = default;
	
	text_t m_text{};
	id_t m_author{};
	std::uint32_t m_retweets{};
	std::uint32_t m_favorites{};
	hashtag_collection_t m_hashtags{};
	id_collection_t m_mentions{};
	id_collection_t m_media{};
	id_t m_quote{};
	id_t m_in_reply_to{};
};

}

#endif