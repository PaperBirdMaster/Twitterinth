// App includes
#include "Twitter/API.hpp"
#include "URL/DownloadFile.hpp"

// External lib includes.
#include "liboauthcpp/liboauthcpp.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"

// STD includes.
#include <fstream>

namespace Twitter
{

// OAuth Twitter authentication data.
static constexpr char consumer_key[]       = "6hkuhkg1QryeddIG57duI7Im9";
static constexpr char consumer_secret[]    = "kc1xP07JFYC1EPfSJs5LtZvAzG3b0vBtPaPJf3nwHHEZkS6aue";
static constexpr char oauth_token[]        = "107972229-etU05J0FlJC3CU6A888UvBo4ZOJRq2W4x3O3OlHk";
static constexpr char oauth_token_secret[] = "6gWj3aOjiu6ASK4VIea0boL3yRBUMF2My2L18ZYcIk2eP";

// OAuth Initialization
OAuth::Consumer consumer(consumer_key, consumer_secret);
OAuth::Token token(oauth_token, oauth_token_secret);
OAuth::Client oauth(&consumer, &token);

rapidjson::Document create_document(const std::string &a_file_name)
{
	rapidjson::Document result;

	if (!a_file_name.empty())
	{
		std::ifstream resource(a_file_name);
		rapidjson::IStreamWrapper json_stream(resource);

		result.ParseStream(json_stream);
	}

	return result;
}

rapidjson::Document get_home_timeline(std::uint32_t a_tweet_count)
{
	static const std::string home_timeline{ "https://api.twitter.com/1.1/statuses/home_timeline.json" };
	std::string query = home_timeline;

	if (a_tweet_count)
	{
		query.append("?count=").append(std::to_string(a_tweet_count));
	}

	const std::string home_timeline_info_file = download::temp_file(home_timeline + "?" + oauth.getURLQueryString(OAuth::Http::Get, query));

	return create_document(home_timeline_info_file);
}

rapidjson::Document get_user(id_t a_user_id)
{
	static const std::string users_show{ "https://api.twitter.com/1.1/users/show.json" };
	const std::string user_info_file = download::temp_file(users_show + "?" + oauth.getURLQueryString(OAuth::Http::Get, users_show + "?user_id=" + std::to_string(a_user_id)));

	return create_document(user_info_file);
}

rapidjson::Document get_tweet(id_t a_tweet_id)
{
	static const std::string status_show{ "https://api.twitter.com/1.1/statuses/show.json" };
	const std::string tweet_info_file = download::temp_file(status_show + "?" + oauth.getURLQueryString(OAuth::Http::Get, status_show + "?id=" + std::to_string(a_tweet_id)));

	return create_document(tweet_info_file);
}

}
