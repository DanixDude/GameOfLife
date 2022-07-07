#include "GameOfLife.h"

#include <string>
#include <cmath>

GameOfLife::GameOfLife() {

}

GameOfLife::~GameOfLife() {
	for (int x = 0; x < cellsXTotal; ++x) {
		delete[] cells[x];
	}

	delete[] cells;
}

bool GameOfLife::Initialize() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL: %s", SDL_GetError());
		return false;
	}

	window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, 0);

	if (window == nullptr) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create SDL window: %s", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (renderer == nullptr) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create SDL renderer: %s", SDL_GetError());
		return false;
	}

	return true;
}

void GameOfLife::HandleEvent(SDL_Event& event) {
	switch (event.type) {
	case SDL_QUIT:
		running = false;
		break;

	case SDL_MOUSEMOTION:
	{
		int xIdx = event.button.x / CELL_WIDTH;
		int yIdx = event.button.y / CELL_HEIGHT;

		if (xIdx < 0) xIdx = 0;
		if (yIdx < 0) yIdx = 0;

		if (xIdx >= cellsXTotal) xIdx = cellsXTotal - 1;
		if (yIdx >= cellsYTotal) yIdx = cellsYTotal - 1;

		Uint32 mouseState = SDL_GetMouseState(nullptr, nullptr);
		Cell& cell = cells[xIdx][yIdx];

		Uint32 lbuttonPressed = (mouseState & SDL_BUTTON_LMASK);
		Uint32 rbuttonPressed = (mouseState & SDL_BUTTON_RMASK);

		if (lbuttonPressed || rbuttonPressed) {
			if (lbuttonPressed) cell.state = true;
			else if (rbuttonPressed) cell.state = false;
		}

		break;
	}

	case SDL_KEYUP:
		if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
			paused = !paused;
		}
		else if (event.key.keysym.scancode == SDL_SCANCODE_DELETE) {
			ClearGrid();
		}

		break;
	}
}

void GameOfLife::CheckCells() {
	for (int x = 0; x < cellsXTotal; ++x) {
		for (int y = 0; y < cellsYTotal; ++y) {
			Cell& cell = cells[x][y];
			int neighbours = 0;

			// Left side
			if ((x - 1) >= 0) {
				if (cells[x - 1][y].state == true) ++neighbours;
				if ((y - 1) >= 0 && cells[x - 1][y - 1].state == true) ++neighbours;
				if ((y + 1) < cellsYTotal && cells[x - 1][y + 1].state == true) ++neighbours;
			}

			// Right side
			if ((x + 1) < cellsXTotal) {
				if (cells[x + 1][y].state == true) ++neighbours;
				if ((y - 1) >= 0 && cells[x + 1][y - 1].state == true) ++neighbours;
				if ((y + 1) < cellsYTotal && cells[x + 1][y + 1].state == true) ++neighbours;
			}

			// Up and down
			if ((y - 1) >= 0 && cells[x][y - 1].state == true) ++neighbours;
			if ((y + 1) < cellsYTotal && cells[x][y + 1].state == true) ++neighbours;

			if (cell.state == false && neighbours == 3) {
				cell.nextState = true;
			}
			else if (cell.state == true && (neighbours != 2 && neighbours != 3)) {
				cell.nextState = false;
			}
			else {
				cell.nextState = cell.state;
			}
		}
	}
}

void GameOfLife::UpdateCells() {
	for (int x = 0; x < cellsXTotal; ++x) {
		for (int y = 0; y < cellsYTotal; ++y) {
			Cell& cell = cells[x][y];
			cell.state = cell.nextState;
		}
	}
}

void GameOfLife::ClearGrid() {
	for (int x = 0; x < cellsXTotal; ++x) {
		for (int y = 0; y < cellsYTotal; ++y) {
			Cell& cell = cells[x][y];
			cell.state = false;
			cell.nextState = false;
		}
	}
}

void GameOfLife::DrawCells() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	for (int x = 0; x < cellsXTotal; ++x) {
		for (int y = 0; y < cellsYTotal; ++y) {
			if (cells[x][y].state != false) {
				SDL_Rect rect{ x * CELL_WIDTH, y * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT };
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}
}

void GameOfLife::DrawGrid() {
	SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);

	for (int x = 0; x < cellsXTotal; ++x) {
		for (int y = 0; y < cellsYTotal; ++y) {
			SDL_Rect rect{ x * CELL_WIDTH, y * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT };
			SDL_RenderDrawRect(renderer, &rect);
		}
	}
}

void GameOfLife::RunLoop() {
	SDL_Event event;
	cells = new Cell*[cellsXTotal];

	for (int x = 0; x < cellsXTotal; ++x) {
		cells[x] = new Cell[cellsYTotal];
	}

	SDL_ShowSimpleMessageBox(
		SDL_MESSAGEBOX_INFORMATION,
		"Game of Life",
		"Keybinds:\n\nPause: ESC\nLeft mouse button: make cell alive\nRight mouse button: make cell dead",
		window
	);

	while (running) {
		while (SDL_PollEvent(&event)) {
			HandleEvent(event);
		}

		std::string title { windowTitle };
		if (paused) title += " (Paused)";
		SDL_SetWindowTitle(window, title.c_str());

		if (!paused) {
			CheckCells();
			UpdateCells();
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		DrawCells();
		DrawGrid();

		SDL_RenderPresent(renderer);
	}

	running = true;
	paused = true;
}

GameOfLife& GameOfLife::Get() {
	static GameOfLife game;
	return game;
}