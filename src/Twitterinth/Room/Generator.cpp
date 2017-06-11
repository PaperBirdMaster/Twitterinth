// App includes
#include "Room/Generator.hpp"
#include "Room/AddOn.hpp"
#include "Room/Room.hpp"
#include "Twitter/Tweet.hpp"

// STD includes.
#include <vector>
#include <algorithm>

namespace Twitterinth
{

using room_generator_collection_t = std::vector<Twitterinth::AddOn>;

room_generator_collection_t load_room_generators();

room_generator_collection_t room_generators = load_room_generators();

room_generator_collection_t load_room_generators()
{
	room_generator_collection_t result{};

	WIN32_FIND_DATAA folder_data{};
	auto addon_folders = FindFirstFileA("../AddOns/*", &folder_data);

	do
	{
		if (folder_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			std::string path{ folder_data.cFileName };

			if ((path != ".") && (path != ".."))
			{
				WIN32_FIND_DATAA file_data{};
				auto addon_file = FindFirstFileA(("../AddOns/" + path + "/*.rg").c_str(), &file_data);

				if ((addon_file != INVALID_HANDLE_VALUE) && (file_data.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE))
				{
					result.emplace_back("../AddOns/" + path + '/', file_data.cFileName);
				}
			}
		}
	}
	while (FindNextFileA(addon_folders, &folder_data));

	result.erase
	(
		std::remove_if(result.begin(), result.end(), [](auto &a_addon) { return a_addon == false;  }),
		result.end()
	);
	result.shrink_to_fit();

	return result;
}

Room create_room(const Twitter::Tweet &a_tweet)
{
	if (!room_generators.empty())
	{
		//const auto &generator = room_generators.front();
		//generator.create_room(a_room.tiles(), a_room.tiles_count(), a_room.width(), a_room.heigth(), a_tweet.text().c_str());
		return { room_generators.front(), a_tweet };
	}

	return { AddOn{ "", "" }, Twitter::Tweet::Null };
}

}
