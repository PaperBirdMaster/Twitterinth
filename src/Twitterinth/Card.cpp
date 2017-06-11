// App includes.
#include "Card.hpp"
#include "Twitter/Tweet.hpp"
#include "Twitter/User.hpp"
#include "Twitterinth_twitter_user.hpp"

// OS includes.
#include <Windows.h>

namespace Twitterinth
{

void set_sprite_size(sf::Sprite &a_sprite, sf::Vector2f a_size)
{
	const auto texture_rect = a_sprite.getTextureRect();
	const auto sprite_scale_x = a_size.x / float(texture_rect.width);
	const auto sprite_scale_y = a_size.y / float(texture_rect.height);

	a_sprite.setScale(sprite_scale_x, sprite_scale_y);
}

sf::Color number_to_color(std::uint32_t a_number)
{
	const sf::Uint8 r = (a_number & 0xff0000u) >> 16u;
	const sf::Uint8 g = (a_number & 0x00ff00u) >> 8u;
	const sf::Uint8 b = (a_number & 0x0000ffu);

	return { r, g, b };
}

Card::Card(const sf::IntRect &a_rect) :
	m_rect{a_rect}
{
	m_card_texture.loadFromFile("../media/cr.png");

	m_card_body[0].setTexture(m_card_texture);
	m_card_body[1].setTexture(m_card_texture);
	m_card_body[2].setTexture(m_card_texture);
	m_card_body[3].setTexture(m_card_texture);
	m_card_body[4].setTexture(m_card_texture);
	m_card_body[5].setTexture(m_card_texture);
	m_card_body[6].setTexture(m_card_texture);
	m_card_body[7].setTexture(m_card_texture);
	m_card_body[8].setTexture(m_card_texture);

	m_card_body[0].setTextureRect({ 0u , 0u , 35u, 35u });
	m_card_body[1].setTextureRect({ 35u, 0u , 35u, 35u });
	m_card_body[2].setTextureRect({ 70u, 0u , 35u, 35u });
	m_card_body[3].setTextureRect({ 0u , 35u, 35u, 35u });
	m_card_body[4].setTextureRect({ 35u, 35u, 35u, 35u });
	m_card_body[5].setTextureRect({ 70u, 35u, 35u, 35u });
	m_card_body[6].setTextureRect({ 0u , 70u, 35u, 35u });
	m_card_body[7].setTextureRect({ 35u, 70u, 35u, 35u });
	m_card_body[8].setTextureRect({ 70u, 70u, 35u, 35u });

	char path[MAX_PATH]{};
	GetWindowsDirectoryA(path, MAX_PATH);
	
	m_font.loadFromFile(std::string{ path }.append("/fonts/arial.ttf"));
	m_text.setFont(m_font);
	m_text.setCharacterSize(30u);
	m_text.setStyle(sf::Text::Regular);

	set_sprite_size(m_card_body[1], sf::Vector2f(m_rect.width - 70, 35));
	set_sprite_size(m_card_body[3], sf::Vector2f(35,                m_rect.height - 70));
	set_sprite_size(m_card_body[4], sf::Vector2f(m_rect.width - 70, m_rect.height - 70));
	set_sprite_size(m_card_body[5], sf::Vector2f(35,                m_rect.height - 70));
	set_sprite_size(m_card_body[7], sf::Vector2f(m_rect.width - 70, 35));

	m_card_body[0].setPosition(m_rect.left,                       m_rect.top);
	m_card_body[1].setPosition(m_rect.left + 35.f,                m_rect.top);
	m_card_body[2].setPosition(m_rect.left + m_rect.width - 35.f, m_rect.top);
	m_card_body[3].setPosition(m_rect.left,                       m_rect.top + 35.f);
	m_card_body[4].setPosition(m_rect.left + 35.f,                m_rect.top + 35.f);
	m_card_body[5].setPosition(m_rect.left + m_rect.width - 35.f, m_rect.top + 35.f);
	m_card_body[6].setPosition(m_rect.left,                       m_rect.top + m_rect.height - 35.f);
	m_card_body[7].setPosition(m_rect.left + 35.f,                m_rect.top + m_rect.height - 35.f);
	m_card_body[8].setPosition(m_rect.left + m_rect.width - 35.f, m_rect.top + m_rect.height - 35.f);

	m_text.setPosition(m_rect.left + 200.f, m_rect.top + 35.f);
}

void Card::set_tweet(const Twitter::Tweet &a_tweet)
{
	auto &author = get_user(a_tweet.author());
	sf::Color text_color{ number_to_color(author.profile_text_color()) }, card_color{ number_to_color(author.profile_background_color()) };

	m_user_profile_picture = get_profile_picture(a_tweet.author());
	m_user_profile_picture.setPosition(m_rect.left + 35.f, m_rect.top + 35.f);

	m_text.setString(a_tweet.text());
	m_text.setColor(text_color);

	for (auto &body : m_card_body)
	{
		body.setColor(card_color);
	}
}

void Card::render(sf::RenderTarget &a_render_target, const sf::VideoMode &a_video_mode)
{
	for (const auto &ct : m_card_body)
	{
		a_render_target.draw(ct);
	}

	a_render_target.draw(m_text);
	a_render_target.draw(m_user_profile_picture);
}

}
