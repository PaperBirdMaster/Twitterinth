#ifndef TWITTERINTH_SCENE_ITEM
#define TWITTERINTH_SCENE_ITEM

#include "Scene.hpp"

namespace Twitterinth
{

template <typename scene_element_t>
struct scene_item : Scene::scene_element
{
	/**
	*/
	scene_item(scene_element_t &a_element) :
		m_element{ a_element }
	{}

	/**
	*/
	void render() const override;

	//!
	scene_element_t &m_element;
};

}

#endif
