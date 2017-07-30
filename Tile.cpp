#include "Tile.h"

#include <iostream>

std::vector<sf::Sprite*> Tile::g_TileTextures;

Tile::Tile(float _x, float _y, sf::Sprite* _spr)
{
	m_Sprite = new sf::Sprite(*_spr);
	m_Collider = new sf::RectangleShape(sf::Vector2f(TILE_SIZE, TILE_SIZE));

	m_X = _x;
	m_Y = _y;

	m_Sprite->setPosition(m_X, m_Y);
	m_Collider->setPosition(m_X, m_Y);
}

Tile::~Tile()
{

}

void Tile::draw(sf::RenderWindow& _window)
{
	_window.draw(*m_Sprite);
}
