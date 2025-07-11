#include <iostream>
#pragma once

class Obstacle
{
private:
	int X;
	int Y;
public:
	Obstacle(int x, int y);

	void set_x(int new_value);
	void set_y(int new_value);

	int x();
	int y();
};
