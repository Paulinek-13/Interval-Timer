// ___ ==================================================
// ___ file: Input.h
// ___ project: Interval Timer
// ___ author: Paulina Kalicka
// ___ ==================================================

#ifndef INPUT_H
#define INPUT_H

#include <allegro5/allegro_audio.h>

#include "Button.h"

class Input :public Button
{
public:
	Input();
	Input(float px, float py, float w, float h, bool on, std::string text, int val);
	~Input();

	int value;

	std::string str_value;

	void Draw(ALLEGRO_FONT* font);

	// returns true if typing is finished
	bool KeysInput(ALLEGRO_EVENT& event, ALLEGRO_SAMPLE* sample);
	// returns false if value is not valid
	bool ConfirmInput();
};

#endif // !INPUT_H
