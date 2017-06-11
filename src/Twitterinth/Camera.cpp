// App includes.
#include "Camera.hpp"
#include "Room/Room.hpp"

namespace Twitterinth
{

Camera::Camera(const Room &a_room, const sf::Vector3f &a_position) :
	m_position { a_position },
	m_target   { &a_room    }
{
}

void Camera::set_target(const Room &a_room)
{
	m_target = &a_room;
}

void Camera::zoom(float a_zoom)
{
	m_position.z += a_zoom;
}

void Camera::set_position(const sf::Vector2f &a_position)
{
	m_position.x = a_position.x;
	m_position.y = a_position.y;
}

void Camera::update()
{
	// Calculate tile scales according to zoom (position Z value).
	//auto tile_size = m_target->m_generator.get().tile_size();

	//m_target->m_sprites
}

}
