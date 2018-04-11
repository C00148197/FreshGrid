//#pragma once
//
//#include "stdafx.h"
//#include "AStar.h"
//#include "Wall.h"
//#include "Player.h"
//
//class PredatorShip {
//public:
//	PredatorShip();
//	PredatorShip(SDL_Point pos, NodeLayout &nodes, Player* player, std::vector<Wall*> &walls);
//
//	void render(sf::RenderWindow &window);
//
//	void update(float deltaTime);
//	void chooseTarget(float deltaTime);
//	void seek(float deltaTime, SDL_Point v, float dist, bool seekingPlayer);
//	//void checkWallCollisions(Wall* wall, float deltaTime);
//	//void checkBulletCollision(Projectile* p);
//	void setupPath();
//	void fireBullet();
//
//	void normalise(SDL_Point &v);
//	float calculateMagnitude(SDL_Point v);
//	float calculateMagnitude(SDL_Point v1, SDL_Point v2);
//	sf::FloatRect getRect();
//
//	void renderRadar(sf::RenderWindow &window);
//	void checkPlayerBulletColl();
//	bool getAlive();
//
//private:
//	SDL_Point m_pos;
//	SDL_Point m_nextPosX;
//	SDL_Point m_nextPosY;
//
//	float m_width;
//	float m_height;
//
//	SDL_Point m_vel;
//	SDL_Point m_accel;
//	float m_magnitudeAccel;
//	float m_maxAccel;
//
//	float m_distToNextPoint;
//	float m_distToPlayer;
//	bool m_alive;
//	float m_orientation;
//	float m_angleToPlayer;
//
//	sf::Image m_image;
//	sf::Texture m_texture;
//	sf::Sprite m_sprite;
//	sf::Sprite m_radarImage;
//	sf::Texture m_radarTexture;
//
//	Player* m_player;
//	float m_maxSpeed;
//
//	std::vector<Projectile*> m_bullets;
//	float m_fireRange;
//
//	sf::Clock m_fireClock;
//	sf::Time m_fireTime;
//	bool m_canFire;
//	float m_fireRate;
//
//	std::vector<Wall*>& m_walls;
//
//	NodeLayout& m_nodeLayout;
//	std::vector<Node*> m_path;
//
//	AStar* m_astar;
//};