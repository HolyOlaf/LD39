#pragma once

#include <SFML/Graphics.hpp>

#include "Enemy.h"

class Bullet
{
public:
	Bullet(sf::Sprite* _spr, sf::Vector2f _startPos, sf::Vector2f _direction, float _angle, float _speed, float _dmg);
	~Bullet();

	void update(std::vector<Enemy*>& _enemies, float _delta);
	void draw(sf::RenderWindow& _window);

	inline float getX() { return m_Sprite.getPosition().x; }
	inline float getY() { return m_Sprite.getPosition().y; }
	inline bool shouldDestroy() { return m_ShouldDestroy; }

private:
	float m_Speed;
	float m_Dmg;
	bool m_ShouldDestroy;
	sf::Sprite m_Sprite;
	sf::Vector2f m_Direction;
};
