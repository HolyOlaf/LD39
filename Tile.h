#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#define TILE_SIZE 64

class Tile
{
public:
	Tile(float _x, float _y, sf::Sprite* _spr);
	~Tile();

	void draw(sf::RenderWindow& _window);

	inline sf::Sprite* getSprite() { return m_Sprite; }
	inline float getX() { return m_X; }
	inline float getY() { return m_Y; }
	inline sf::RectangleShape* getCollider() { return m_Collider; }

	static std::vector<sf::Sprite*> g_TileTextures;

private:
	float m_X, m_Y;
	sf::Sprite* m_Sprite;
	sf::RectangleShape* m_Collider;
};

