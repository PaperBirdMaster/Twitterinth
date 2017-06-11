#ifndef TWITTERINTH_CAMERA
#define TWITTERINTH_CAMERA

// App includes.
#include "Types.hpp"

// External lib includes.
#include "SFML/System/Vector3.hpp"
#include "SFML/System/Vector2.hpp"

namespace Twitterinth
{

struct Camera
{
	friend void render(const Camera &);

	Camera(const Room &a_room, const sf::Vector3f &a_position);

	void set_target(const Room &a_room);

	void zoom(float a_zoom);
	
	void set_position(const sf::Vector2f &a_position);

private:

	void update();

	sf::Vector3f m_position;
	const Room *m_target{ nullptr };
};

}

#endif
