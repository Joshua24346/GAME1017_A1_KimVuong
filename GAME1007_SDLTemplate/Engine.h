#pragma once
#ifndef _ENGINE_H_
#define _ENGINE_H_
#define FPS 60
#define WIDTH 1024
#define HEIGHT 768
#include "SDL.h"
#include "SDL_mixer.h"
#include <iostream>
#include <vector>
#include <ctime>
using namespace std;

class Sprite
{
protected:
	SDL_Rect m_src; //Source rectangle
	SDL_Rect m_dest; // Destination rectangle

public:
	void SetRekts(const SDL_Rect s, const SDL_Rect d)
	{
		m_src = s;
		m_dest = d;
	}
	SDL_Rect* GetSrc() { return &m_src; }
	SDL_Rect* GetDest() { return &m_dest; }
};

//Enemy
class Enemy : public Sprite
{
public:
	//Constructs Enemy
	Enemy()
	{
		cout << "Constructing Enemy" << endl;
		m_src = { 0, 0, 500, 469 };
		m_dest = { 1024, 20 + rand() % 650, 105, 94 };
	}

	void SetLoc(SDL_Point newloc)
	{
		m_dest.x = newloc.x + 20;
		m_dest.y = newloc.y + 50;
	}

	//Enemy moves every x pixels
	void Update()
	{
		m_dest.x -= 2;
	}

	//Deconstructs enemy
	~Enemy()
	{
		cout << "Deconstructing enemy" << endl;
	}
};

//Bullet
class Bullet {

private:
	SDL_Rect m_rect;
public:
	Bullet(SDL_Point spawnLoc = { 512, 384 }) //Non-default constructor
	{
		this->m_rect.x = spawnLoc.x + 20;
		this->m_rect.y = spawnLoc.y + 50;
		this->m_rect.w = 15;
		this->m_rect.h = 5;
	}

	void SetLoc(SDL_Point loc) // Location
	{
		m_rect.x = 200;
		m_rect.y = 400;
	}

	//Player Bullet
	void Update()
	{
		this->m_rect.x += 10;
	}

	//Enemy Bullet
	void UpdateE()
	{
		this->m_rect.x -= 10;
	}

	void Render(SDL_Renderer* rend)
	{
		SDL_SetRenderDrawColor(rend, 50, 50, 50, 255);
		SDL_RenderFillRect(rend, &m_rect);
	}

	SDL_Rect* GetRekt() { return &m_rect; }
};

class Obstacle : public Sprite
{
public:
	Obstacle()
	{
		m_dest.y = 0;
		m_dest.x = 1024;
		m_dest.h = 50;
		m_dest.w = 100;
		m_src = { 0,0, 2400, 514 };
	}

	void Update()
	{
		this->m_dest.x -= 2;
	}

	void setYP(int p)
	{
		m_dest.y = 50 * p;
	}
};

class Engine
{
private:
	bool m_running = false;
	int m_FrameC;

	Engine() {} // Prevents instantiation outside of the class

	//Player Status
	bool m_status;

	const Uint8* m_keystates;
	int m_speed = 10;

	//Sprite
	Sprite  m_Player;
	Sprite	m_bg1, m_bg2;
	Uint32  m_start, m_end, m_delta, m_fps;

	//SDL
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	SDL_Texture* m_pTexture, * m_pEnemyTexture, * m_pBGTexture, * m_pObstacles;

	//Vectors
	vector<Enemy*> m_Enemy;
	vector<Bullet*> m_PlayerBullets;
	vector<Bullet*> m_EnemyBullet;
	vector<Obstacle*> m_Obstacles;


	// Sound effects
	Mix_Chunk* m_Explosion;
	Mix_Chunk* m_PlayerBulletS;
	Mix_Chunk* m_EnemyBulletS;
	Mix_Chunk* m_Hit;

	// Music
	Mix_Music* m_BackgroundM; 

	

private:
	int Init(const char* title, int xPos, int yPos, int width, int height, int flags);
	void Clean();
	void Wake();
	void HandleEvents();
	void Update();
	void Render();
	void Sleep();

public:
	int Run();
	//static Engine* Instance(); // Pointer way.
	static Engine& Instance(); // Static method for object access.
	SDL_Renderer* GetRenderer() { return m_pRenderer; }
	bool KeyDown(SDL_Scancode c);
};
#endif
// Reminder: you can ONLY have declarations in headers, not logical code