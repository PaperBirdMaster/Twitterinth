#ifndef TWITTERINTH_TWITTER_JSON
#define TWITTERINTH_TWITTER_JSON

// App includes
#include "Twitter/Types.hpp"

// External lib includes.
#include "rapidjson/document.h"

// STD includes.
#include <algorithm>

namespace Twitterinth
{
namespace JSON
{

/** JSON Node.
*/
using node_t = rapidjson::GenericValue<rapidjson::UTF8<>>;

Twitter::id_t node_to_id(const node_t &a_json_node);

std::uint32_t node_to_number(const node_t &a_json_node);

std::uint32_t node_to_color(const node_t &a_json_node);

Twitter::text_t node_to_text(const node_t &a_json_node);

Twitter::string_t node_to_string(const node_t &a_json_node);

const node_t *begin(const node_t &a_json_document);

const node_t *end(const node_t &a_json_document);

}
}

#endif
