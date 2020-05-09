// ___ ==================================================
// ___ file: Input.cpp
// ___ project: Interval Timer
// ___ author: Paulina Kalicka
// ___ ==================================================

#include "Header.h"
#include "Input.h"

#include <allegro5/allegro_primitives.h>

Input::Input()
{
	pos_x = 0;
	pos_y = 0;
	width = 0;
	height = 0;

	on = true;
	hovered = false;

	text = "";

	value = 0;

	str_value = "0";
}
Input::Input(float px, float py, float w, float h, bool on, std::string text, int val)
{
	this->pos_x = px;
	this->pos_y = py;
	this->width = w;
	this->height = h;

	this->on = on;
	this->hovered = false;

	this->text = text;

	this->value = val;
	this->str_value = std::to_string(val);
}
Input::~Input() {}

void Input::Draw(ALLEGRO_FONT* font)
{
	al_draw_filled_rectangle(pos_x, pos_y, pos_x + width, pos_y + height,
							 al_map_rgb(0, 0, 100));
	DrawText(font, hovered ? width * 0.9 : width * 0.7, hovered ? height * 0.9 : height * 0.7, 0, pos_x + (width / 2), pos_y + (height / 2), 1,
			 on ? al_map_rgb(255, 200, 0) : al_map_rgb(200, 0, 0), text + str_value);

	if(hovered)
		al_draw_filled_rectangle(pos_x, pos_y, pos_x + width, pos_y + height,
								 al_map_rgba(132, 132, 132, 132));
}

// returns true if typing is finished
bool Input::KeysInput(ALLEGRO_EVENT& event, ALLEGRO_SAMPLE* sample)
{
	// whether defined key was typed
	bool result = 1;

	switch(event.keyboard.keycode)
	{
		case ALLEGRO_KEY_0:
			str_value += "0";
			break;
		case ALLEGRO_KEY_1:
			str_value += "1";
			break;
		case ALLEGRO_KEY_2:
			str_value += "2";
			break;
		case ALLEGRO_KEY_3:
			str_value += "3";
			break;
		case ALLEGRO_KEY_4:
			str_value += "4";
			break;
		case ALLEGRO_KEY_5:
			str_value += "5";
			break;
		case ALLEGRO_KEY_6:
			str_value += "6";
			break;
		case ALLEGRO_KEY_7:
			str_value += "7";
			break;
		case ALLEGRO_KEY_8:
			str_value += "8";
			break;
		case ALLEGRO_KEY_9:
			str_value += "9";
			break;
		case ALLEGRO_KEY_BACKSPACE:
			if(str_value.size() > 0)
				str_value.pop_back();
			break;
		case ALLEGRO_KEY_ENTER:
			ConfirmInput();
			al_play_sample(sample, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
			return true;
			break;

		default:result = 0; break;
	}

	if(result)al_play_sample(sample, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);

	return false;
}
// returns false if value is not valid
bool Input::ConfirmInput()
{
	on = true;

	if((str_value.size() > 0) && (str_value.size() < 5))
	{
		value = std::stoi(str_value);
		return true;
	}
	else
	{
		value = 0;
		str_value = "0";
		return false;
	}
}