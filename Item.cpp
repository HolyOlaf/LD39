#include "Item.h"

Item::Item(float _x, float _y, sf::Sprite* _spr, std::function<void()> _effect)
{
	m_Sprite = sf::Sprite(*_spr);
	m_Sprite.setPosition(_x, _y);

	m_Effect = _effect;

	m_Collected = false;
}

Item::~Item()
{

}

void Item::update(Player* _player)
{
	if (!m_Collected)
	{
		if (m_Sprite.getGlobalBounds().intersects(_player->getDrawable()->getGlobalBounds()))
		{
			m_Effect();

			m_Collected = true;
		}
	}
}

void Item::draw(sf::RenderWindow& _window)
{
	_window.draw(m_Sprite);
}