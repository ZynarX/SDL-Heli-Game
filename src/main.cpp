#define SDL_MAIN_HANDLED
#include <iostream>
#include <random>
#include "Helicopter.hpp"
#include "Obstacle.hpp"
#include <SDL.h>

void HandleInput(SDL_Event& event, bool& is_running, Helicopter& player);
bool CheckCollision(SDL_Rect& a, SDL_Rect& b);
void HandleMovement(Helicopter& player, float TIME_STEP);
void HandleRender(SDL_Renderer* renderer, Helicopter& player);
void CheckEndGame(bool& is_running, bool& is_lose, Helicopter& player);

constexpr float FIXED_TIME_STEP = 1.0f / 60.0f;
constexpr float FRAME_DELAY = 1000 / 60.0f;

constexpr int WIDTH = 600;
constexpr int HEIGHT = 800;

Uint32 lastPlacement = 0;

std::vector<Obstacle> Obstacles = {};
int survivedObstacles = 0;

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("Helicopter Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (!renderer)
	{
		std::cerr << "Couldn't instantiate renderer...\n";
		return -1;
	}

	bool is_running = true;
	bool is_lose = false;
	SDL_Event event;

	Uint32 previousTick = SDL_GetTicks();
	float accumulator = 0.0f;

	Helicopter player(250, 600, 100, 100);

	while (is_running)
	{
		HandleInput(event, is_running, player);
	
		CheckEndGame(is_running, is_lose, player);

		Uint32 currentTick = SDL_GetTicks();
		float deltaTime = (currentTick - previousTick) / 1000.0f;
		previousTick = currentTick;

		accumulator += deltaTime;

		while (accumulator >= FIXED_TIME_STEP)
		{
			HandleMovement(player, FIXED_TIME_STEP);
			accumulator -= FIXED_TIME_STEP;
		}

		HandleRender(renderer, player);

		Uint32 finalTick = SDL_GetTicks();
		
		if ((float)finalTick - currentTick < FRAME_DELAY)
		{
			SDL_Delay(FRAME_DELAY - (finalTick - currentTick));
		}
	}

	if (is_lose)
	{
		SDL_Delay(500);

		std::cout << "You have lost!" << std::endl;
	}

	std::cout << "You have survived " << survivedObstacles << " obstacles!" << std::endl;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

void HandleInput(SDL_Event& event, bool& is_running, Helicopter& player)
{
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			std::cout << "Quitting game...\n";
			is_running = false;
		}
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
				case SDLK_a:
					player.is_left = true;
					break;
				case SDLK_LEFT:
					player.is_left = true;
					break;
				case SDLK_d:
					player.is_right = true;
					break;
				case SDLK_RIGHT:
					player.is_right = true;
					break;
			}
		}
		if (event.type == SDL_KEYUP)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_a:
				player.is_left = false;
				break;
			case SDLK_LEFT:
				player.is_left = false;
				break;
			case SDLK_d:
				player.is_right = false;
				break;
			case SDLK_RIGHT:
				player.is_right = false;
				break;
			}
		}
	}
}

bool CheckCollision(SDL_Rect& a, SDL_Rect& b)
{
	int aLeft = a.x;
	int aRight = a.x + a.w;
	int aTop = a.y;
	int aBottom = a.y + a.h;

	int bLeft = b.x;
	int bRight = b.x + b.w;
	int bTop = b.y;
	int bBottom = b.y + b.h;

	return ((aLeft < bRight && aRight > bLeft) && (aBottom > bTop && aTop < bBottom));
}

void HandleMovement(Helicopter& player, float TIME_STEP)
{
	if (player.is_left)
	{
		if (player.x() - (player.speed() * TIME_STEP) >= 0)
		{
			player.set_x(player.x() - (player.speed() * TIME_STEP));
		}
	}
	if (player.is_right)
	{
		if (player.x() + (player.speed() * TIME_STEP) + player.w() <= WIDTH)
		{
			player.set_x(player.x() + (player.speed() * TIME_STEP));
		}
	}

	for (size_t i = 0; i < Obstacles.size(); i++)
	{
		Obstacles[i].set_y(Obstacles[i].y() + (125.0f * TIME_STEP));

		if (Obstacles[i].y() > HEIGHT)
		{
			Obstacles.erase(Obstacles.begin() + i);
			survivedObstacles++;
		}
	}
}

void HandleRender(SDL_Renderer* renderer, Helicopter& player)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	SDL_SetRenderDrawColor(renderer, 100, 225, 100, 255);
	SDL_RenderClear(renderer);

	SDL_Rect plr = { player.x(), player.y(), player.w(), player.h() };

	SDL_SetRenderDrawColor(renderer, 100, 100, 225, 255);
	SDL_RenderFillRect(renderer, &plr);

	Uint32 curTime = SDL_GetTicks();

	if (curTime - lastPlacement >= 1500)
	{
		std::uniform_int_distribution<> dist(0, WIDTH - 100);

		int x = dist(gen);
		int y = -50;

		Obstacle obstacle(x, y);

		Obstacles.push_back(obstacle);

		lastPlacement = curTime;
	}

	for (Obstacle obstacle : Obstacles)
	{
		SDL_Rect thing = { obstacle.x(), obstacle.y(), 100, 100 };

		SDL_SetRenderDrawColor(renderer, 225, 100, 100, 255);
		SDL_RenderFillRect(renderer, &thing);
	}

	SDL_RenderPresent(renderer);
}

void CheckEndGame(bool& is_running, bool& is_lose, Helicopter& player)
{
	SDL_Rect playerRect = { player.x(), player.y(), player.w(), player.h() };

	for (Obstacle& obst : Obstacles)
	{
		SDL_Rect curRect = { obst.x(), obst.y(), 100, 100 };

		if (CheckCollision(curRect, playerRect))
		{
			is_running = false;
			is_lose = true;
		}
	}
}
