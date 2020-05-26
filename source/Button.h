// ==================================================
// file: Button.h
// project: Interval Timer
// author: Paulina Kalicka
// ==================================================

#ifndef BUTTON_H
#define BUTTON_H

#include <string>

#include <allegro5/allegro_font.h>

class Button
{
public:
	Button();
	Button(float px, float py, float w, float h, bool on, std::string text);
	~Button();

	float pos_x;
	float pos_y;
	float width;
	float height;

	bool on;
	bool hovered;

	std::string text;

	virtual void Draw(ALLEGRO_FONT* font);
	void CheckIfHovered(float mx, float my);
};

#endif // !BUTTON_H
