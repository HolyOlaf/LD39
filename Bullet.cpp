#include "Bullet.h"

Bullet::Bullet(sf::Sprite* _spr, sf::Vector2f _startPos, sf::Vector2f _direction, float _angle, float _speed, float _dmg)
{
	m_Sprite = sf::Sprite(*_spr);
	m_Sprite.setPosition(_startPos);
	m_Sprite.setRotation(_angle);
	m_Speed = _speed;
	m_Direction = _direction;
	m_Dmg = _dmg;
	m_ShouldDestroy = false;
}

Bullet::~Bullet()
{
}

void Bullet::update(std::vector<Enemy*>& _enemies, float _delta)
{
	m_Sprite.move((m_Direction * m_Speed) * _delta);

	for (auto enemy : _enemies)
	{
		if (enemy->getSprite().getGlobalBounds().intersects(m_Sprite.getGlobalBounds()))
		{
			enemy->damage(m_Dmg);
			m_ShouldDestroy = true;
		}
	}
}

void Bullet::draw(sf::RenderWindow& _window)
{
	_window.draw(m_Sprite);
}
