// App includes.
#include "Scene.hpp"
#include "render.hpp"
#include "scene_item.hpp"

namespace Twitterinth
{

void Scene::add(Card &a_card)
{
	m_elements.emplace_back(new scene_item<Card>(a_card));
}

void Scene::add(Room &a_room)
{
	m_elements.emplace_back(new scene_item<Room>(a_room));
}

void Scene::render() const
{
	//for (const auto &element : m_elements)
	//{
	//	element->render();
	//}
}

}
