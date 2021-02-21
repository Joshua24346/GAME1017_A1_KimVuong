#include "Engine.h"
#include "SDL_image.h"
#include <ctime>
using namespace std;

Engine* Engine::engineInstance = nullptr;

// Engine
int Engine::Init(const char* title, int xPos, int yPos, int width, int height, int flags)
{
	cout << "Initializing engine..." << endl;
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		m_pWindow = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
		if (m_pWindow != nullptr)
		{	
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, NULL);
			if (m_pRenderer != nullptr)
			{
				if (IMG_Init(IMG_INIT_PNG) != 0)
				{
					// Images
					m_pEnemyTexture = IMG_LoadTexture(m_pRenderer, "Enemy.png");
					m_pTexture = IMG_LoadTexture(m_pRenderer, "Player.png");
					m_pBGTexture = IMG_LoadTexture(m_pRenderer, "Background_1.png");
					m_pObstacles = IMG_LoadTexture(m_pRenderer, "Log.png");
				}
				else return false;
				if (Mix_Init(MIX_INIT_MP3) != 0)
				{
					// Audio Mixer
					Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 2048);
					Mix_AllocateChannels(16);
					m_BackgroundM = Mix_LoadMUS("One Summer's Day.mp3");
					m_Explosion = Mix_LoadWAV("Explosion.wav");
					m_PlayerBulletS = Mix_LoadWAV("Player_Bullet.wav");
					m_EnemyBulletS = Mix_LoadWAV("Enemy_Bullet.wav");
					m_Hit = Mix_LoadWAV("Hit.wav");
				}
			}
			else return false;
		}
		else return false;
	}
	else return false;
	// Background Music | -1 = Loop, 0+ = amount of times music will play
	Mix_PlayMusic(m_BackgroundM, -1); 

	// Sets volume - 0 to 128
	Mix_VolumeMusic(128);

	m_FrameC = 0;
	m_status = true;

	m_fps = (Uint32)round(1.0 / (double)FPS * 1000);
	m_keystates = SDL_GetKeyboardState(nullptr);

	// Setting Rectangles
	m_Player.SetRekts(  {0, 0, 207, 342}, {0, 0, 100, 120} );
	m_bg1.SetRekts(  {0, 0, 1920, 1080}, {0, 0, 1024, 768} );
	m_bg2.SetRekts(  {0, 0, 1920, 1080}, {1024, 0, 1024, 768} );

	// Spawning Enemy
	for (unsigned i = 0; i < m_Enemy.size(); i++)
	m_Enemy[i]->SetRekts ({ 0, 0, 600, 768 }, { 1024,  20 + rand() % 650, 105, 94 });
	cout << "Initialization: Successful" << endl;
	m_running = true;
	return true;
}

// Engine Wake
void Engine::Wake()
{
	m_start = SDL_GetTicks();
}
// Engine Handle Event
void Engine::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_running = false;
			break;

		case SDL_KEYUP:
			if (event.key.keysym.sym == ' ') {
				//Checks player status which also allows the game to run after death - True = alive | False = dead
				if (m_status == true) 
				{
					m_PlayerBullets.push_back(new Bullet({ m_Player.GetDest()->x, m_Player.GetDest()->y }));
					m_PlayerBullets.shrink_to_fit();
					cout << "Player bullet capacity " << m_PlayerBullets.capacity() << endl;
					// -1 = first available
					Mix_PlayChannel(-1, m_PlayerBulletS, 0); 
				}
			}
		}
	}
}

// Key Down
bool Engine::KeyDown(SDL_Scancode c)
{
	if (m_keystates != nullptr)
	{
		if (m_keystates[c] == 1)
			return true;
	}
	return false;
}

// Engine Update
void Engine::Update()
{
	m_FrameC++;

	// Scroll backgrounds
	m_bg1.GetDest()->x -= m_speed / 2;
	m_bg2.GetDest()->x -= m_speed / 2;

	// Wrap the background
	if (m_bg1.GetDest()->x <= -m_bg1.GetDest()->w)
	{
		m_bg1.GetDest()->x = 0;
		m_bg2.GetDest()->x = 1024;
	}

	// Movement
	if (KeyDown(SDL_SCANCODE_W) && m_Player.GetDest()->y > 20)
		m_Player.GetDest()->y -= m_speed;

	if (KeyDown(SDL_SCANCODE_S) && m_Player.GetDest()->y < 750 - m_Player.GetDest()->h)
		m_Player.GetDest()->y += m_speed;

	if (KeyDown(SDL_SCANCODE_A) && m_Player.GetDest()->x > 0)
		m_Player.GetDest()->x -= m_speed;

	if (KeyDown(SDL_SCANCODE_D) && m_Player.GetDest()->x < WIDTH / 4 - m_Player.GetDest()->h)
		m_Player.GetDest()->x += m_speed;

	// Obstacle spawning
	for (unsigned i = 0; i < m_Obstacles.size(); i++)
	{
		m_Obstacles[i]->Update();
	}

	// Player bullet
	for (unsigned i = 0 ; i < m_PlayerBullets.size(); i++)
	{
		m_PlayerBullets[i]->Update();
	}

	for (unsigned i = 0; i < m_PlayerBullets.size(); i++)
	{
		if (m_PlayerBullets[i]->GetRekt()->x <= (m_PlayerBullets[i]->GetRekt()->w))
		{
			delete m_PlayerBullets[i];
			m_PlayerBullets[i] = nullptr;
			m_PlayerBullets.erase(m_PlayerBullets.begin() + i);
			m_PlayerBullets.shrink_to_fit();
			break;
		} 
	}

	// Spawning Obstacles
	if (m_FrameC % 120 == 0 && m_Obstacles.size() == 0)
	{
		srand(time(NULL));
		for (int x = 0; x < 3; x++)
		{
			int t = rand() % 15 + 1;
			m_Obstacles.push_back(new Obstacle());
			m_Obstacles[x]->setYP(t);
		}
	}

	// Deleting Obstacles
	for (unsigned i = 0; i < m_Obstacles.size(); i++)
	{
		if (m_Obstacles[i]->GetDest()->x <= -(m_Obstacles[i]->GetDest()->w))
		{
			delete m_Obstacles[i];
			m_Obstacles[i] = nullptr;
			m_Obstacles.erase(m_Obstacles.begin() + i);
			m_Obstacles.shrink_to_fit();
			break;
		}
	}

	// Spawning Enemies
	if (m_FrameC % 120 == 0)
	{
		m_Enemy.push_back(new Enemy());
		m_Enemy.shrink_to_fit();
		cout << "Enemy capacity " << m_Enemy.capacity() << endl;
	}
	
	for (unsigned i = 0; i < m_Enemy.size(); i++)
	{
		m_Enemy[i]->Update();
	}
	
	for (unsigned i = 0; i < m_Enemy.size(); i++)
	{
		if (m_Enemy[i]->GetDest()->x <= -(m_Enemy[i]->GetDest()->w))
		{
			delete m_Enemy[i];
			m_Enemy[i] = nullptr;
			m_Enemy.erase(m_Enemy.begin() + i);
			m_Enemy.shrink_to_fit();
			break;
		}
	}
	
	// Enemy Bullet
	for (unsigned i = 0; i < m_EnemyBullet.size(); i++)
	{	
		m_EnemyBullet[i]->UpdateE();
	}
	
	// Enemy bullet capacity 
	if (m_FrameC % 50 == 0)
	{
		for (unsigned i = 0; i < m_Enemy.size(); i++)
		{
			m_EnemyBullet.push_back(new Bullet({ m_Enemy[i]->GetDest()->x, m_Enemy[i]->GetDest()->y + 25 }));
			m_EnemyBullet.shrink_to_fit();
			cout << "Enemy bullet capacity " << m_EnemyBullet.capacity() << endl;
			// -1 = first available
			Mix_PlayChannel(-1, m_EnemyBulletS, 0); 
		}
	}
	
	// If enemy bullet goes off screen
	for (unsigned i = 0; i < m_EnemyBullet.size(); i++){
		if (m_EnemyBullet[i]->GetRekt()->x <= -(m_EnemyBullet[i]->GetRekt()->w))
		{
			delete m_EnemyBullet[i];
			m_EnemyBullet[i] = nullptr;
			m_EnemyBullet.erase(m_EnemyBullet.begin() + i);
			m_EnemyBullet.shrink_to_fit();
			break;
		}
	}
	
	// Enemy Explosion
	if (m_PlayerBullets.size() != 0) 
	{
		for (unsigned i = 0; i < m_PlayerBullets.size(); i++)
		{
			for (unsigned j = 0; j < m_Enemy.size(); j++)
			{
				if (SDL_HasIntersection(m_Enemy[j]->GetDest(), m_PlayerBullets[i]->GetRekt()))
				{
					if (m_status == true)
					{
						// -1 = first available
						Mix_PlayChannel(-1, m_Explosion, 0);
					}
					delete m_Enemy[j];
					m_Enemy[j] = nullptr;
					m_Enemy.erase(m_Enemy.begin() + j);
					m_Enemy.shrink_to_fit();

					delete m_PlayerBullets[i];
					m_PlayerBullets[i] = nullptr;
					m_PlayerBullets.erase(m_PlayerBullets.begin() + i);
					m_PlayerBullets.shrink_to_fit();
					break;
				}
			}
		}
	}
	
	// When player bullet hits enemy
	if (m_Enemy.size() != 0 && m_PlayerBullets.size() != 0)
	{
		for (unsigned i = 0; i < m_PlayerBullets.size(); i++)
		{
			for (unsigned j = 0; j < m_Enemy.size(); j++)
			{
				if (SDL_HasIntersection(m_Enemy[j]->GetDest(), m_PlayerBullets[i]->GetRekt()))
				{
					if (m_status == true)
					{
						// -1 = first available
						Mix_PlayChannel(-1, m_Explosion, 0);
					}
					delete m_Enemy[j];
					m_Enemy[j] = nullptr;
					m_Enemy.erase(m_Enemy.begin() + j);
					m_Enemy.shrink_to_fit();

					delete m_PlayerBullets[i];
					m_PlayerBullets[i] = nullptr;
					m_PlayerBullets.erase(m_PlayerBullets.begin() + i);
					m_PlayerBullets.shrink_to_fit();
					break;
				}
			}
		}
	}
	
	// Player explosion
	if (m_EnemyBullet.size() != 0)
	{
		for (unsigned j = 0; j < m_EnemyBullet.size(); j++)
		{
			if (SDL_HasIntersection(m_EnemyBullet[j]->GetRekt(), m_Player.GetDest()))
			{
				if (m_status == true)
				{
					// -1 = first available
					Mix_PlayChannel(-1, m_Explosion, 0);
				}

				delete m_EnemyBullet[j];
				m_EnemyBullet[j] = nullptr;
				m_EnemyBullet.erase(m_EnemyBullet.begin() + j);
				m_EnemyBullet.shrink_to_fit();
				m_status = false;
				break;
			}
		}
	}

	// If player collides with enemy
	if (m_Enemy.size() != 0)
	{
		for (unsigned j = 0; j < m_Enemy.size(); j++)
		{
			if (SDL_HasIntersection(m_Player.GetDest(), m_Enemy[j]->GetDest ()))
			{
				if (m_status == true)
				{
					// -1 = first available
					Mix_PlayChannel(-1, m_Explosion, 0);
				}
				m_status = false;
				break;
			}
		}
	}

	// If player collides with obstacles 
	if (m_Obstacles.size() != 0)
	{
		for (unsigned j = 0; j < m_Obstacles.size(); j++)
		{
			if (SDL_HasIntersection(m_Player.GetDest(), m_Obstacles[j]->GetDest()))
			{
				if (m_status == true)
				{
					// -1 = first available
					Mix_PlayChannel(-1, m_Explosion, 0);
				}
				m_status = false;
				break;
			}
		}
	}
}

// Engine Render
void Engine::Render()
{
	SDL_RenderCopy(m_pRenderer, m_pBGTexture, m_bg1.GetSrc(), m_bg1.GetDest());
	SDL_RenderCopy(m_pRenderer, m_pBGTexture, m_bg2.GetSrc(), m_bg2.GetDest());

	// Render Player
	if (m_status == true)
	{
		SDL_RenderCopyEx(m_pRenderer, m_pTexture, m_Player.GetSrc(), m_Player.GetDest(), 0, NULL, SDL_FLIP_NONE);
	}
	
	// Player Bullet
	if (m_PlayerBullets.size() != 0) 
	{
		for (unsigned i = 0; i < m_PlayerBullets.size(); i++)
		m_PlayerBullets[i]->Render(m_pRenderer);
	}
	
	// Render Enemy
	if (m_Enemy.size() != 0)
	{
		for (unsigned i = 0; i < m_Enemy.size(); i++)
		SDL_RenderCopyEx(m_pRenderer, m_pEnemyTexture, m_Enemy[i]->GetSrc(), m_Enemy[i]->GetDest(), 0, NULL, SDL_FLIP_NONE);
	}

	// Enemy Bullet
	if (m_EnemyBullet.size() != 0)
	{
		for (unsigned i = 0; i < m_EnemyBullet.size(); i++)
		m_EnemyBullet[i]->Render(m_pRenderer);
	}

	// Obstacles
	for (int x = 0; x < m_Obstacles.size(); x++)
	{
		SDL_RenderCopy(m_pRenderer, m_pObstacles, m_Obstacles[x]->GetSrc(), m_Obstacles[x]->GetDest());
	}

	SDL_RenderPresent(m_pRenderer);
}

// Engine Sleep
void Engine::Sleep()
{
	m_end = SDL_GetTicks();
	m_delta = m_end - m_start;
	if (m_delta < m_fps)
	SDL_Delay(m_fps - m_delta);
}

// Running Engine
int Engine::Run()
{
	if (m_running == true)
	{return 1;}

	if (Init("Milestone 4", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, NULL) == false)
	{return 2;}

	while (m_running == true)
	{
		Wake();
		HandleEvents();
		Update();
		Render();

		if (m_running == true)
		Sleep();
	}
	Clean();
	return 0;
}

// Cleaning Engine
void Engine::Clean(){

	cout << "Cleaning engine..." << endl;
	for (unsigned i = 0; i < m_PlayerBullets.size(); i++)
	{
		delete m_PlayerBullets[i];
		m_PlayerBullets[i] = nullptr;
	}

	m_PlayerBullets.clear();
	m_PlayerBullets.shrink_to_fit();
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyTexture(m_pTexture);
	SDL_DestroyTexture(m_pEnemyTexture);
	SDL_DestroyTexture(m_pBGTexture);
	IMG_Quit();
	SDL_Quit();
}