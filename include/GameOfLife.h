#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include "SDL/SDL.h"
#include "Cell.h"

class GameOfLife {
private:
	const char* windowTitle = "Game of Life";
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	const int windowWidth = 800;
	const int windowHeight = 600;

	const int cellsXTotal = windowWidth / CELL_WIDTH;
	const int cellsYTotal = windowHeight / CELL_HEIGHT;

	Cell** cells = nullptr;

	bool running = true;
	bool paused = true;

	GameOfLife();
public:
	GameOfLife(const GameOfLife& game) = delete;
	~GameOfLife();

	void HandleEvent(SDL_Event& event);

	void CheckCells();
	void UpdateCells();
	void ClearGrid();

	void DrawCells();
	void DrawGrid();

	bool Initialize();
	void RunLoop();

	static GameOfLife& Get();
};

#endif