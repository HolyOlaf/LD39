#include <vector>
#include <iostream>
#include <ctime>
#include <random>

#include <SFML/Graphics.hpp>

#include "Player.h"
#include "Enemy.h"
#include "Tile.h"
#include "Bullet.h"
#include "Item.h"

#define PI 3.14159265359

void genTileTextures(std::string _sheetPath, int _tileSize)
{
	sf::Texture* sheetTex = new sf::Texture();
	sheetTex->loadFromFile(_sheetPath);
	
	for (int y = 0; y < sheetTex->getSize().y; y += _tileSize)
	{
		for (int x = 0; x < sheetTex->getSize().x; x += _tileSize)
		{
			Tile::g_TileTextures.push_back(new sf::Sprite(*sheetTex, sf::IntRect(x, y, _tileSize, _tileSize)));
		}
	}
}

sf::Sprite* getSheet(std::string _sheetPath)
{
	sf::Texture* sheetTex = new sf::Texture();
	sheetTex->loadFromFile(_sheetPath);

	return new sf::Sprite(*sheetTex);
}

int game(sf::RenderWindow* window)
{
	genTileTextures("res/textures/tilesheet.png", 64);

	Player player(64.f, 10.f, 64.f, 64.f, 0.08f, 0.5f, 150.f);

	std::vector<Tile*> ground;
	std::vector<sf::RectangleShape*> colliders;
	std::vector<Bullet*> bullets;
	std::vector<Enemy*> enemies;
	std::vector<Item*> items;

	sf::Texture* itemTex = new sf::Texture();
	itemTex->loadFromFile("res/textures/itemsheet.png");
	sf::Sprite* batterySprite = new sf::Sprite(*itemTex, sf::IntRect(0, 0, 16, 16));
	sf::Sprite* damageUpSprite = new sf::Sprite(*itemTex, sf::IntRect(16, 0, 16, 16));
	sf::Sprite* maxEnergyUpSprite = new sf::Sprite(*itemTex, sf::IntRect(32, 0, 16, 16));

	sf::Texture* bulletTex = new sf::Texture();
	bulletTex->loadFromFile("res/textures/energybullet.png");

	sf::Sprite* bulletSprite = new sf::Sprite(*bulletTex);

	sf::Texture* gameoverTex = new sf::Texture;
	gameoverTex->loadFromFile("res/textures/gameoverscreen.png");

	sf::Sprite* gameOverScreen = new sf::Sprite(*gameoverTex);
	gameOverScreen->scale(sf::Vector2f(0.5f, 0.5f));

	for (int i = 0; i < window->getSize().x + 100; i += TILE_SIZE)
	{
		ground.push_back(new Tile(i, window->getSize().y - TILE_SIZE * 2, Tile::g_TileTextures[0]));
		colliders.push_back(ground[i / TILE_SIZE]->getCollider());
	}

	for (int i = 0; i < window->getSize().x + 100; i += TILE_SIZE)
	{
		ground.push_back(new Tile(i, window->getSize().y - TILE_SIZE, Tile::g_TileTextures[1]));
	}

	sf::Sprite* lanternTop1 = new sf::Sprite(*Tile::g_TileTextures[2]);
	sf::Sprite* lanternBottom1 = new sf::Sprite(*Tile::g_TileTextures[3]);

	lanternBottom1->setPosition(TILE_SIZE, window->getSize().y - TILE_SIZE * 3);
	lanternTop1->setPosition(TILE_SIZE, window->getSize().y - TILE_SIZE * 4);

	sf::Sprite* lanternTop2 = new sf::Sprite(*Tile::g_TileTextures[2]);
	sf::Sprite* lanternBottom2 = new sf::Sprite(*Tile::g_TileTextures[3]);

	sf::Sprite* grass1 = new sf::Sprite(*Tile::g_TileTextures[4]);
	grass1->setPosition(TILE_SIZE, window->getSize().y - TILE_SIZE * 3);
	sf::Sprite* grass2 = new sf::Sprite(*Tile::g_TileTextures[4]);
	grass2->setPosition(TILE_SIZE * 4, window->getSize().y - TILE_SIZE * 3);

	sf::Sprite* wall1 = new sf::Sprite(*Tile::g_TileTextures[5]);
	wall1->setPosition(0, window->getSize().y - TILE_SIZE * 3);
	sf::Sprite* wall2 = new sf::Sprite(*Tile::g_TileTextures[6]);
	wall2->setPosition(TILE_SIZE, window->getSize().y - TILE_SIZE * 3);
	sf::Sprite* wall3 = new sf::Sprite(*Tile::g_TileTextures[7]);
	wall3->setPosition(TILE_SIZE * 2, window->getSize().y - TILE_SIZE * 3);

	lanternBottom2->setPosition(TILE_SIZE * 6, window->getSize().y - TILE_SIZE * 3);
	lanternTop2->setPosition(TILE_SIZE * 6, window->getSize().y - TILE_SIZE * 4);

	bool forceEnergyUpdate = false;

	sf::Vector2i mousePos;

	std::clock_t startTime;
	startTime = std::clock();

	std::mt19937 rng(time(NULL));
	std::uniform_int_distribution<int> genEnemyPos(0, window->getSize().x);
	std::uniform_int_distribution<int> genItemType(0, 1000);
	std::uniform_int_distribution<int> genEnemyType(0, 1000);

	float playerDamage = 8.0f;

	int spawnCount = 0;
	int spawnAmount = 1;
	int spawnTime = 3000;
	
	while (window->isOpen())
	{
		mousePos = sf::Mouse::getPosition(*window);

		forceEnergyUpdate = false;

		sf::Event evnt;
		while (window->pollEvent(evnt))
		{
			if (evnt.type == sf::Event::Closed)
			{
				window->close();
				return 0;
			}
			if (evnt.type == sf::Event::KeyPressed)
			{
				if (evnt.key.code == sf::Keyboard::Space)
					player.jump();
				if (evnt.key.code == sf::Keyboard::R)
				{
					if (player.getEnergy() <= 0)
						return 1;
				}
			}
			if (evnt.type == sf::Event::MouseButtonPressed)
			{
				if (evnt.mouseButton.button == sf::Mouse::Button::Left)
				{
					float angle = atan2(mousePos.y - (player.getY() + player.getXSize() / 2), mousePos.x - (player.getX() + player.getYSize() / 2));
					bullets.push_back(new Bullet(bulletSprite, sf::Vector2f(player.getX() + player.getXSize() / 2, player.getY() + player.getYSize() / 2), sf::Vector2f(cos(angle), sin(angle)), angle * (180 / PI), 0.5f, playerDamage));
					player.removeEnergy(3);
					forceEnergyUpdate = true;
				}
			}
		}

		window->clear(sf::Color(96, 78, 66));

		if (spawnCount > 5)
		{
			spawnCount = 0;
			if (spawnTime > 500)
				spawnTime -= 500;
			else
			{
				spawnTime = 3000;
				spawnAmount++;
			}
		}

		window->draw(*wall1);
		window->draw(*wall2);
		window->draw(*wall3);

		if (player.getEnergy() > 0)
		{
			if (std::clock() - startTime >= spawnTime)
			{
				spawnCount++;

				startTime = std::clock();

				for (int i = 0; i < spawnAmount; i++)
				{
					int xPos = genEnemyPos(rng);

					while (!(xPos < player.getX() - 50) && !(xPos > player.getX() + player.getXSize() + 50))
					{
						xPos = genEnemyPos(rng);
					}

					int num = genItemType(rng);
					if (num >= 0 && num <= 500)
					{
						enemies.push_back(new Enemy(xPos, window->getSize().y - TILE_SIZE * 2 - 1, 32.f, 32.f, 0.07f, 100.f, 0.002f, getSheet("res/textures/spidersheet.png")));
					}
					else if (num >= 501 && num <= 1000)
					{
						enemies.push_back(new Enemy(xPos, window->getSize().y - TILE_SIZE * 2 - 1, 32.f, 32.f, 0.04f, 30.f, 0.01f, getSheet("res/textures/robotsheet.png")));
					}
				}
			}

			player.update(colliders);

			for (int i = 0; i < ground.size(); i++)
			{
				ground[i]->draw(*window);
			}

			for (int i = 0; i < bullets.size(); i++)
			{
				if (bullets[i]->getX() < 0 || bullets[i]->getY() < 0 || bullets[i]->getX() > 1000 || bullets[i]->getY() > 1000 || bullets[i]->shouldDestroy())
				{
					delete bullets[i];
					bullets.erase(bullets.begin() + i);
				}
				else
				{
					bullets[i]->update(enemies);
					bullets[i]->draw(*window);
				}
			}

			for (int i = 0; i < enemies.size(); i++)
			{
				if (enemies[i]->getHealth() <= 0)
				{
					int num = genItemType(rng);

					if (num >= 0 && num <= 500)
					{
						items.push_back(new Item(enemies[i]->getX(), window->getSize().y - TILE_SIZE * 2 - 16, batterySprite, [&player] { player.addEnergy(50); }));
					}
					else if (num >= 501 && num <= 900)
					{
						items.push_back(new Item(enemies[i]->getX(), window->getSize().y - TILE_SIZE * 2 - 16, damageUpSprite, [&playerDamage] { playerDamage += 5.f; }));
					}
					else if (num >= 901 && num <= 1000)
					{
						items.push_back(new Item(enemies[i]->getX(), window->getSize().y - TILE_SIZE * 2 - 16, maxEnergyUpSprite, [&player] { player.increaseMaxEnergy(40); }));
					}

					enemies[i]->die();
					delete enemies[i];
					enemies.erase(enemies.begin() + i);
				}
				else
				{
					enemies[i]->update(&player);
					enemies[i]->draw(*window);
				}
			}

			for (int i = 0; i < items.size(); i++)
			{
				if (items[i]->isCollected())
				{
					delete items[i];
					items.erase(items.begin() + i);
				}
				else
				{
					items[i]->update(&player);
					items[i]->draw(*window);
				}
			}

			player.draw(*window);

			window->draw(*lanternBottom1);
			window->draw(*lanternTop1);
			window->draw(*lanternBottom2);
			window->draw(*lanternTop2);
			window->draw(*grass1);
			window->draw(*grass2);
		}
		else
		{
			window->draw(*gameOverScreen);
		}

		window->display();
	}

	return 0;
}

int main()
{
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(500, 500), "LD39");
	
	while (game(window) == 1)
	{
		game(window);
	}

	return 0;
}