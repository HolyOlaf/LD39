#pragma once

#include <SFML/Graphics.hpp>

#define GRAVITY 0.0005f

class Player
{
public:
	Player(float _x, float _y, float _sizeX, float _sizeY, float _speedX, float _jumpspeed, float _maxEnergy);
	~Player();

	void update(const std::vector<sf::RectangleShape*>& _colliders);
	void draw(sf::RenderWindow& _window);
	inline void jump() { if (colBottom) m_Jump = true; }

	inline float getX() { return m_Drawable->getPosition().x; }
	inline float getY() { return m_Drawable->getPosition().y; }
	inline float getXSize() { return m_Drawable->getSize().x; }
	inline float getYSize() { return m_Drawable->getSize().y; }

	inline float getEnergy() { return m_Energy; }
	inline void removeEnergy(float _energy) { m_Energy -= _energy; }
	inline void addEnergy(float _energy) { m_Energy += _energy; }

	inline float getMaxEnergy() { return m_MaxEnergy; }
	inline void increaseMaxEnergy(float _amount) { m_MaxEnergy += _amount; }

	inline sf::RectangleShape* getDrawable() { return m_Drawable; }

private:
	float m_SpeedX, m_JumpSpeed;
	bool m_Jump, m_Falling;
	bool colRight, colLeft, colTop, colBottom;
	float m_Energy, m_MaxEnergy;

	sf::Texture* m_BatteryTexture;
	sf::RectangleShape* m_Battery;
	sf::RectangleShape* m_EnergyIndicator;
	sf::Vector2f m_EnergyIndicatorStartPos;
	sf::Texture* m_Texture;
	sf::RectangleShape* m_Drawable;
	sf::RectangleShape *m_BottomBox, *m_TopBox, *m_LeftBox, *m_RightBox;
	sf::Vector2f m_Velocity;
};
