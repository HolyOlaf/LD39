#pragma once

#include <SFML/Graphics.hpp>

#include "Player.h"

class Enemy
{
public:
	Enemy(float _x, float _y, float _sizeX, float _sizeY, float _speedX, float _health, float _damage, sf::Sprite* _spr);
	~Enemy();

	void update(Player* _player);
	void draw(sf::RenderWindow& _window);

	inline void damage(float _dmg) { m_Health -= _dmg; }
	inline float getHealth() { return m_Health; }
	inline sf::Sprite getSprite() const { return m_Sprite; }

	inline float getX() { return m_Sprite.getPosition().x; }
	inline float getY() { return m_Sprite.getPosition().y; }

	void die();

private:
	float m_SpeedX;
	float m_Health;
	bool m_Jump, m_Falling;
	bool m_Dead;
	float m_Damage;

	sf::Sprite m_Sprite;
	sf::Vector2f m_Velocity;
};

