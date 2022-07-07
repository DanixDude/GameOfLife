#ifndef CELL_H
#define CELL_H

constexpr int CELL_WIDTH = 13;
constexpr int CELL_HEIGHT = 13;

struct Cell {
public:
	bool state = false;
	bool nextState = false;

	Cell();
	Cell(bool state);
};

#endif