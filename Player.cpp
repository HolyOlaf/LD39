#include "Player.h"

#include "Tile.h"

Player::Player(float _x, float _y, float _sizeX, float _sizeY, float _speedX, float _jumpspeed, float _maxEnergy)
{
	m_Drawable = new sf::RectangleShape(sf::Vector2f(_sizeX, _sizeY));
	m_Texture = new sf::Texture;
	m_Texture->loadFromFile("res/textures/player.png");
	m_Drawable->setTexture(m_Texture);
	m_Drawable->setPosition(_x, _y);

	m_BottomBox = new sf::RectangleShape(sf::Vector2f(_sizeX - 2, 1));
	m_TopBox = new sf::RectangleShape(sf::Vector2f(_sizeX - 2, 1));
	m_RightBox = new sf::RectangleShape(sf::Vector2f(1, _sizeY - 2));
	m_LeftBox = new sf::RectangleShape(sf::Vector2f(1, _sizeY - 2));

	m_BatteryTexture = new sf::Texture;
	m_BatteryTexture->loadFromFile("res/textures/battery.png");

	m_Battery = new sf::RectangleShape(sf::Vector2f(64, 128));
	m_Battery->setTexture(m_BatteryTexture);
	m_Battery->setPosition(0, 0);

	m_EnergyIndicator = new sf::RectangleShape(sf::Vector2f(8, 60));
	m_EnergyIndicator->setFillColor(sf::Color::Red);
	m_EnergyIndicator->setPosition(m_Battery->getPosition().x + 14 * 2, m_Battery->getPosition().y + 17 * 2);

	m_EnergyIndicatorStartPos = m_EnergyIndicator->getPosition();

	m_Jump = false;
	m_Falling = false;
	colRight = colLeft = colTop = false;
	m_SpeedX = _speedX;
	m_JumpSpeed = _jumpspeed;
	m_MaxEnergy = _maxEnergy;
	m_Energy = m_MaxEnergy;
}

Player::~Player()
{
}

void Player::update(const std::vector<sf::RectangleShape*>& _colliders)
{
	if (m_Jump)
	{
		m_Velocity.y = -m_JumpSpeed;
		m_Drawable->setPosition(m_Drawable->getPosition().x, m_Drawable->getPosition().y - 1);
		m_Jump = false;

		m_Energy -= 4.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !colLeft)
	{
		if (m_Drawable->getPosition().x > 0)
		{
			m_Velocity.x = -m_SpeedX;

			m_Energy -= 0.001f;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !colRight)
	{
		if (m_Drawable->getPosition().x + m_Drawable->getSize().x < 500)
		{
			m_Velocity.x = m_SpeedX;
			m_Energy -= 0.001f;
		}
	}

	if (m_Energy > m_MaxEnergy)
		m_Energy = m_MaxEnergy;

	m_Drawable->move(m_Velocity);

	m_RightBox->setPosition(m_Drawable->getPosition().x + m_Drawable->getSize().x, m_Drawable->getPosition().y + 1);
	m_LeftBox->setPosition(m_Drawable->getPosition().x - m_LeftBox->getSize().x, m_Drawable->getPosition().y + 1);
	m_TopBox->setPosition(m_Drawable->getPosition().x + 1, m_Drawable->getPosition().y - m_TopBox->getSize().y);
	m_BottomBox->setPosition(m_Drawable->getPosition().x + 1, m_Drawable->getPosition().y + m_Drawable->getSize().y);

	colLeft = colRight = colTop = colBottom = false;
	bool checkBottom = true;
	bool addVel = true;

	m_RightBox->setFillColor(sf::Color::White);
	m_LeftBox->setFillColor(sf::Color::White);
	m_BottomBox->setFillColor(sf::Color::White);

	for (auto col : _colliders)
	{
		if (m_RightBox->getGlobalBounds().intersects(col->getGlobalBounds()))
		{
			colRight = true;
			m_RightBox->setFillColor(sf::Color::Red);
		}
		if (m_LeftBox->getGlobalBounds().intersects(col->getGlobalBounds()))
		{
			colLeft = true;
			m_LeftBox->setFillColor(sf::Color::Red);
		}

		if (checkBottom)
		{
			if (m_BottomBox->getGlobalBounds().intersects(col->getGlobalBounds()))
			{
				m_Drawable->setPosition(m_Drawable->getPosition().x, col->getPosition().y - m_Drawable->getSize().y);
				m_Velocity.y = 0;
				m_Falling = false;
				checkBottom = false;
				colBottom = true;
				m_BottomBox->setFillColor(sf::Color::Red);
			}
			else
			{
				if (addVel)
					m_Velocity.y += GRAVITY;
				addVel = false;
				m_Falling = true;
			}
		}
	}

	m_Velocity.x = 0;

	int pixelHeight = m_Energy / m_MaxEnergy * 60;
	m_EnergyIndicator->setSize(sf::Vector2f(8, pixelHeight));
	float yPos = m_EnergyIndicatorStartPos.y + (60 - pixelHeight);
	m_EnergyIndicator->setPosition(m_EnergyIndicatorStartPos.x, yPos);
}

void Player::draw(sf::RenderWindow&_window)
{
	_window.draw(*m_Battery);
	_window.draw(*m_EnergyIndicator);
	_window.draw(*m_Drawable);
}
