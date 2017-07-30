#pragma once

#include <functional>

#include <SFML/Graphics.hpp>

#include "Player.h"

class Item
{
public:
	Item(float _x, float _y, sf::Sprite* _spr, std::function<void()> _effect);
	~Item();

	void update(Player* _player);
	void draw(sf::RenderWindow& _window);

	inline bool isCollected() { return m_Collected; }

private:
	sf::Sprite m_Sprite;
	std::function<void()> m_Effect;

	bool m_Collected;
};

