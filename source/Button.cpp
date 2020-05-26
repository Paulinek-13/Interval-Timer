// ==================================================
// file: Button.cpp
// project: Interval Timer
// author: Paulina Kalicka
// ==================================================

#include "Header.h"
#include "Button.h"

#include <allegro5/allegro_primitives.h>

Button::Button()
{
	pos_x = 0;
	pos_y = 0;
	width = 0;
	height = 0;

	on = true;
	hovered = false;

	text = "";
}
Button::Button(float px, float py, float w, float h, bool on, std::string text)
{
	this->pos_x = px;
	this->pos_y = py;
	this->width = w;
	this->height = h;

	this->on = on;
	this->hovered = false;

	this->text = text;
}
Button::~Button() {}

void Button::Draw(ALLEGRO_FONT *font)
{
	al_draw_filled_rectangle(pos_x, pos_y, pos_x + width, pos_y + height,
							 al_map_rgb(0, 0, 0));
	DrawText(font, hovered ? width * 0.9 : width * 0.7, hovered ? height * 0.9 : height * 0.7, 0, pos_x + (width / 2), pos_y + (height / 2), 1,
			 on ? al_map_rgb(255, 200, 0) : al_map_rgb(200, 0, 0), text);

	if (hovered)
		al_draw_filled_rectangle(pos_x, pos_y, pos_x + width, pos_y + height,
								 al_map_rgba(132, 132, 132, 132));
}
void Button::CheckIfHovered(float mx, float my)
{
	if ((mx > pos_x) && (mx < pos_x + width))
	{
		if ((my > pos_y) && (my < pos_y + height))
		{
			this->hovered = true;
			return;
		}
	}
	this->hovered = false;
}