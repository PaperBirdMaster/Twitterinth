// App includes.
#include "Twitter/Types.hpp"
#include "JSON/JSON.hpp"

// STD includes.
#include <string>

namespace Twitterinth
{
namespace JSON
{

Twitter::id_t node_to_id(const node_t &a_json_node)
{
	Twitter::id_t result{};

	if (!a_json_node.IsNull() && a_json_node.IsNumber())
	{
		result = a_json_node.GetUint64();
	}

	return result;
}

std::uint32_t node_to_number(const node_t &a_json_node)
{
	std::uint32_t result{};

	if (!a_json_node.IsNull() && a_json_node.IsNumber())
	{
		result = a_json_node.GetUint();
	}

	return result;
}

std::uint32_t node_to_color(const node_t &a_json_node)
{
	std::uint32_t result{};

	if (!a_json_node.IsNull() && a_json_node.IsString())
	{
		result = std::stoi(a_json_node.GetString(), nullptr, 0x10);
	}

	return result;
}

Twitter::text_t node_to_text(const node_t &a_json_node)
{
	Twitter::text_t result{};

	if (!a_json_node.IsNull() && a_json_node.IsString())
	{
		std::copy_n(a_json_node.GetString(), a_json_node.GetStringLength(), std::back_inserter(result));
	}

	return result;
}

Twitter::string_t node_to_string(const node_t &a_json_node)
{
	Twitter::string_t result{};

	if (!a_json_node.IsNull() && a_json_node.IsString())
	{
		result = a_json_node.GetString();
	}

	return result;
}

const node_t *begin(const node_t &a_json_document)
{
	return a_json_document.Begin();
}

const node_t *end(const node_t &a_json_document)
{
	return a_json_document.End();
}

}
}
