#include <iostream>
#include <SDL.h>
#pragma once

class Helicopter
{
private:
	int X;
	int Y;
	int W;
	int H;

	float Speed;
public:
	bool is_right = false;
	bool is_left = false;

	Helicopter(int x, int y, int w, int h);

	void set_x(int new_value);
	void set_y(int new_value);

	int x();
	int y();
	int w();
	int h();

	float speed();
};
