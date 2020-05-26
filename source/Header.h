// ==================================================
// file: Header.h
// project: Interval Timer
// author: Paulina Kalicka
// ==================================================

#ifndef HEADER_H
#define HEADER_H

#include <string>

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>

#include "MyMouse.h"
#include "Button.h"

const float DISPLAY_WIDTH = 800.0;
const float DISPLAY_HEIGHT = 600.0;

const float MAX_VOLUME = 3.0;
const float MIN_VOLUME = 0.0;

enum class STATE
{
	INTRO,
	MAIN,
	TIMER,
	INFO,
	END
};

struct PopUp
{
	float px = 0;
	float py = 0;
	float width = 0;

	std::string text = "!";

	bool draw = false;

	// points which button mouse is actually on
	Button *button_ptr = 0;

	void Draw(ALLEGRO_FONT *font);
	void Update(float mx, float my);
	void New(std::string text, float width, Button *button_ptr);
};

struct App
{
	ALLEGRO_DISPLAY *display = 0;

	ALLEGRO_AUDIO_STREAM *audio_stream = 0;

	ALLEGRO_FONT *font = 0;

	ALLEGRO_BITMAP *bitmap_icon = 0;

	float volume = 1.0;
	float volume_sounds = 1.0;

	int time_exercise = 20;
	int time_rest = 10;
	int sets = 3;

	bool play_sounds = true;

	MyMouse my_mouse = MyMouse();

	PopUp pop_up = PopUp();
};

// resize display in appropriative way
void TransformDisplay(ALLEGRO_DISPLAY *display);
// transform mouse coordinates
void TransformMouse(float *mx, float *my);

void DrawText(ALLEGRO_FONT *font, float width, float height, float theta, float pos_x, float pos_y,
			  bool centre, ALLEGRO_COLOR color, std::string text);

void ChooseMusicFile(App *app, bool is_playing);

bool SaveSettings(App *app);
bool LoadSettings(App *app);

STATE IntroLoop(App *app);
STATE MainLoop(App *app);
STATE InfoLoop(App *app);
STATE TimerLoop(App *app);

#endif // !HEADER_H
