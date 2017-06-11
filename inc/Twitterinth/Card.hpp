#ifndef TWITTERINTH_CARD
#define TWITTERINTH_CARD

// App includes.
#include "Twitter/Types.hpp"

// External lib includes.
#include "SFML/Graphics.hpp"


namespace Twitterinth
{

/**
*/
struct Card
{
	/**
	*/
	Card(const sf::IntRect &a_rect);

	/**
	*/
	void set_tweet(const Twitter::Tweet &a_tweet);

	/**
	*/
	void render(sf::RenderTarget &a_render_target, const sf::VideoMode &a_video_mode);

private:
	//!
	sf::Texture m_card_texture{};
	//!
	sf::Sprite m_card_body[9]{};
	//!
	sf::IntRect m_rect{};
	//!
	sf::Sprite m_user_profile_picture{};
	//!
	sf::Font m_font{};
	//!
	sf::Text m_text{};
};

}

#endif
