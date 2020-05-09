// ___ ==================================================
// ___ file: MyMouse.cpp
// ___ project: Interval Timer
// ___ author: Paulina Kalicka
// ___ ==================================================

#include "MyMouse.h"

#include <allegro5/allegro_primitives.h>

MyMouse::MyMouse()
{
	for(unsigned i = 0; i < 20; ++i)
		last_positions[i][0] = last_positions[i][1] = 0;
	curr_px = curr_py = 0;

	counter = timer_counter = 0;
}
MyMouse::~MyMouse() {}

void MyMouse::Draw()
{
	for(unsigned i = 0; i < 20; ++i)
		al_draw_filled_circle(last_positions[i][0], last_positions[i][1], 7, al_map_rgba(200, 0, 0, 132));
	al_draw_filled_circle(curr_px, curr_py, 10, al_map_rgba(0, 0, 0, 132));
}
void MyMouse::UpdatePositions(float mx, float my)
{
	timer_counter = 0;
	last_positions[counter][0] = mx;
	last_positions[counter][1] = my;
	curr_px = mx;
	curr_py = my;
}
void MyMouse::UpdateCounter(unsigned timer_ticks)
{
	counter = timer_ticks % 20;

	timer_counter++;
	if(timer_counter > 5)
	{
		for(unsigned i = 0; i < 20; ++i)
		{
			last_positions[i][0] = curr_px;
			last_positions[i][1] = curr_py;
		}
	}
}