#ifndef TWITTERINTH_SCENE
#define TWITTERINTH_SCENE

// App includes
#include "Types.hpp"

// STD includes.
#include <vector>
#include <memory>

namespace Twitterinth
{

struct Scene
{
	//! 
	struct scene_element
	{
		/**
		*/
		virtual void render() const = 0;

		/**
		*/
		virtual ~scene_element() = default;
	};

	/**
	*/
	void add(Card &a_card);

	/**
	*/
	void add(Room &a_room);

	/**
	*/
	void render() const;

private:

	//!
	using scene_element_ptr_t = std::unique_ptr<scene_element>;
	//! 
	using scene_element_collection_t = std::vector<scene_element_ptr_t>;

	//!
	scene_element_collection_t m_elements;
};

}

#endif
