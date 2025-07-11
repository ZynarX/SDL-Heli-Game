#include "Helicopter.hpp"

Helicopter::Helicopter(int x, int y, int w, int h)
{
	this->X = x;
	this->Y = y;
	this->W = w;
	this->H = h;

	this->Speed = 350.0f;
}

void Helicopter::set_x(int new_value)
{
	this->X = new_value;
}
void Helicopter::set_y(int new_value)
{
	this->Y = new_value;
}

int Helicopter::x()
{
	return this->X;
}
int Helicopter::y()
{
	return this->Y;
}
int Helicopter::w()
{
	return this->W;
}
int Helicopter::h()
{
	return this->H;
}
float Helicopter::speed()
{
	return this->Speed;
}
