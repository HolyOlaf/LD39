#include "Enemy.h"
#include "Tile.h"

Enemy::Enemy(float _x, float _y, float _sizeX, float _sizeY, float _speedX, float _health, float _damage, sf::Sprite* _spr)
{
	m_Sprite = sf::Sprite(*_spr);
	m_Sprite.setPosition(_x, _y);
	m_Sprite.setTextureRect(sf::IntRect(32, 0, 32, 32));

	m_Jump = false;
	m_Falling = false;
	m_SpeedX = _speedX;

	m_Health = _health;
	m_Damage = _damage;

	m_Dead = false;
}

Enemy::~Enemy()
{
	die();
}

void Enemy::update(Player* _player)
{
	if (!m_Dead)
	{
		if (_player->getX() + _player->getXSize() < m_Sprite.getPosition().x)
		{
			m_Velocity.x = -m_SpeedX;
			m_Sprite.setTextureRect(sf::IntRect(32, 0, 32, 32));
		}
		else if (_player->getX() > m_Sprite.getPosition().x + m_Sprite.getGlobalBounds().width)
		{
			m_Velocity.x = m_SpeedX;
			m_Sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
		}
		else
		{
			if (_player->getY() >= 500 - TILE_SIZE * 2 - _player->getYSize())
				_player->removeEnergy(m_Damage);
		}

		m_Sprite.move(m_Velocity);

		if (m_Sprite.getPosition().y > 500 - TILE_SIZE * 2)
		{
			m_Velocity.y += GRAVITY;
			m_Falling = true;
		}
		else
		{
			m_Sprite.setPosition(m_Sprite.getPosition().x, 500 - TILE_SIZE * 2 - m_Sprite.getGlobalBounds().height);
			m_Velocity.y = 0;
			m_Falling = false;
		}

		m_Velocity.x = 0;
	}
}

void Enemy::draw(sf::RenderWindow& _window)
{
	if (!m_Dead)
		_window.draw(m_Sprite);
}

void Enemy::die()
{
	m_Dead = true;
}