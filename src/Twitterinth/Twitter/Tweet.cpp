// App includes
#include "Twitter/Tweet.hpp"

namespace Twitter
{

const Tweet Tweet::Null{};

Tweet::Tweet(text_t a_text, id_t a_author, std::uint32_t a_retweets, std::uint32_t a_favorites, hashtag_collection_t a_hashtags, id_collection_t a_mentions, id_collection_t a_media, id_t a_quote, id_t a_in_reply_to) :
	m_text       { a_text        },
	m_author     { a_author      },
	m_retweets   { a_retweets    },
	m_favorites  { a_favorites   },
	m_hashtags   { a_hashtags    },
	m_mentions   { a_mentions    },
	m_media      { a_media       },
	m_quote      { a_quote       },
	m_in_reply_to{ a_in_reply_to }
{
};

const text_t &Tweet::text() const
{
	return m_text;
}

const id_t Tweet::author() const
{
	return m_author;
}

const id_t Tweet::quote() const
{
	return m_quote;
}

const id_t Tweet::in_reply_to() const
{
	return m_in_reply_to;
}

}
