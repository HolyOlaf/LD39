#include "Player.h"

#include "Tile.h"

#include <iostream>
#include <string>

Player::Player(float _x, float _y, float _sizeX, float _sizeY, float _speedX, float _jumpspeed, float _maxEnergy, char* _charsetpath)
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

	m_CharSetTexture = new sf::Texture;
	m_CharSetTexture->loadFromFile(_charsetpath);

	for (int i = 0; i < 8; i++)
	{
		m_Charsets.push_back(new sf::Sprite(*m_CharSetTexture, sf::IntRect(i * 8, 0, 8, 8)));
	}

	for (int i = 0; i < 8; i++)
	{
		m_Charsets.push_back(new sf::Sprite(*m_CharSetTexture, sf::IntRect(i * 8, 8, 8, 8)));
	}

	for (int i = 0; i < 11; i++)
	{
		m_CurrentEnergyNum.push_back(new sf::Sprite(*m_Charsets[0]));
		m_CurrentEnergyNum[i]->setPosition(m_Battery->getPosition().x + m_Battery->getSize().x + 10 + i * 9, 10);
	}
}

Player::~Player()
{
}

void Player::update(const std::vector<sf::RectangleShape*>& _colliders, float _delta)
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

	m_Drawable->move(m_Velocity * _delta);

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
		}
		if (m_LeftBox->getGlobalBounds().intersects(col->getGlobalBounds()))
		{
			colLeft = true;
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

	std::string energystring = std::to_string((int)m_Energy);
	for (int i = energystring.length(); i < 5; i++)
	{
		energystring = "0" + energystring;
	}
	energystring += "/";

	std::string maxenergystring = std::to_string((int)m_MaxEnergy);
	for (int i = maxenergystring.length(); i < 5; i++)
	{
		maxenergystring = "0" + maxenergystring;
	}

	energystring += maxenergystring;

	for (int i = 0; i < energystring.length(); i++)
	{
		sf::Sprite* character;
		
		int index = i;

		sf::Vector2f pos = m_CurrentEnergyNum[index]->getPosition();

		if (energystring[i] == '0')
		{
			delete m_CurrentEnergyNum[index];
			m_CurrentEnergyNum[index] = new sf::Sprite(*m_Charsets[0]);
			m_CurrentEnergyNum[index]->setPosition(pos);
		}
		else if (energystring[i] == '1')
		{
			delete m_CurrentEnergyNum[index];
			m_CurrentEnergyNum[index] = new sf::Sprite(*m_Charsets[1]);
			m_CurrentEnergyNum[index]->setPosition(pos);
		}
		else if (energystring[i] == '2')
		{
			delete m_CurrentEnergyNum[index];
			m_CurrentEnergyNum[index] = new sf::Sprite(*m_Charsets[2]);
			m_CurrentEnergyNum[index]->setPosition(pos);
		}
		else if (energystring[i] == '3')
		{
			delete m_CurrentEnergyNum[index];
			m_CurrentEnergyNum[index] = new sf::Sprite(*m_Charsets[3]);
			m_CurrentEnergyNum[index]->setPosition(pos);
		}
		else if (energystring[i] == '4')
		{
			delete m_CurrentEnergyNum[index];
			m_CurrentEnergyNum[index] = new sf::Sprite(*m_Charsets[4]);
			m_CurrentEnergyNum[index]->setPosition(pos);
		}
		else if (energystring[i] == '5')
		{
			delete m_CurrentEnergyNum[index];
			m_CurrentEnergyNum[index] = new sf::Sprite(*m_Charsets[5]);
			m_CurrentEnergyNum[index]->setPosition(pos);
		}
		else if (energystring[i] == '6')
		{
			delete m_CurrentEnergyNum[index];
			m_CurrentEnergyNum[index] = new sf::Sprite(*m_Charsets[6]);
			m_CurrentEnergyNum[index]->setPosition(pos);
		}
		else if (energystring[i] == '7')
		{
			delete m_CurrentEnergyNum[index];
			m_CurrentEnergyNum[index] = new sf::Sprite(*m_Charsets[7]);
			m_CurrentEnergyNum[index]->setPosition(pos);
		}
		else if (energystring[i] == '8')
		{
			delete m_CurrentEnergyNum[index];
			m_CurrentEnergyNum[index] = new sf::Sprite(*m_Charsets[8]);
			m_CurrentEnergyNum[index]->setPosition(pos);
		}
		else if (energystring[i] == '9')
		{
			delete m_CurrentEnergyNum[index];
			m_CurrentEnergyNum[index] = new sf::Sprite(*m_Charsets[9]);
			m_CurrentEnergyNum[index]->setPosition(pos);
		}
		else if (energystring[i] == '/')
		{
			delete m_CurrentEnergyNum[index];
			m_CurrentEnergyNum[index] = new sf::Sprite(*m_Charsets[10]);
			m_CurrentEnergyNum[index]->setPosition(pos);
		}
	}
}

void Player::draw(sf::RenderWindow&_window)
{
	_window.draw(*m_Battery);
	_window.draw(*m_EnergyIndicator);
	_window.draw(*m_Drawable);

	for (int i = 0; i < m_CurrentEnergyNum.size(); i++)
	{
		_window.draw(*m_CurrentEnergyNum[i]);
	}
}
