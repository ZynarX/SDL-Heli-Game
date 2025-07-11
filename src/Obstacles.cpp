#include "Obstacle.hpp"

Obstacle::Obstacle(int x, int y)
{
	this->X = x;
	this->Y = y;
}

void Obstacle::set_x(int new_value)
{
	this->X = new_value;
}
void Obstacle::set_y(int new_value)
{
	this->Y = new_value;
}

int Obstacle::x()
{
	return this->X;
}
int Obstacle::y()
{
	return this->Y;
}
