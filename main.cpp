#include "GameOfLife.h"

int main(int argc, char** args) {
	GameOfLife& game = GameOfLife::Get();
	
	if (!game.Initialize()) {
		return -1;
	}

	game.RunLoop();
	return 0;
}