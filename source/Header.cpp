// ==================================================
// file: Header.cpp
// project: Interval Timer
// author: Paulina Kalicka
// ==================================================

#include "Header.h"

#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>

#include <fstream>

void PopUp::Draw(ALLEGRO_FONT *font)
{
	if (draw)
	{
		if (px < 400)
		{
			al_draw_filled_rectangle(px, py, px + width, py + 100, al_map_rgb(132, 0, 0));
			DrawText(font, width * 0.9, 50, 0, px + (width / 2), py + 50, 1, al_map_rgb(255, 255, 255), text);
		}
		else
		{
			al_draw_filled_rectangle(px - width, py, px, py + 100, al_map_rgb(132, 0, 0));
			DrawText(font, width * 0.9, 50, 0, px - (width / 2), py + 50, 1, al_map_rgb(255, 255, 255), text);
		}
	}
}
void PopUp::Update(float mx, float my)
{
	this->px = mx;
	this->py = my;

	if (button_ptr && (button_ptr->hovered))
		return;
	draw = false;
}
void PopUp::New(std::string text, float width, Button *button_ptr)
{
	this->text = text;
	this->draw = true;
	this->width = width;
	this->button_ptr = button_ptr;
}

void DrawText(ALLEGRO_FONT *font, float width, float height, float theta, float pos_x, float pos_y,
			  bool centre, ALLEGRO_COLOR color, std::string text)
{
	float text_width = al_get_text_width(font, text.c_str());
	float text_height = al_get_font_line_height(font);

	float scalex = width / text_width;
	float scaley = height / text_height;

	float scale = scalex < scaley ? scalex : scaley;

	ALLEGRO_TRANSFORM prev_t;
	ALLEGRO_TRANSFORM t;

	al_copy_transform(&prev_t, al_get_current_transform());

	al_identity_transform(&t);
	al_translate_transform(&t, -pos_x, -pos_y);
	al_scale_transform(&t, scale, scale);
	al_rotate_transform(&t, theta);
	al_translate_transform(&t, pos_x, pos_y);

	al_compose_transform(&t, &prev_t);
	al_use_transform(&t);

	if (centre)
	{
		float font_h = al_get_font_line_height(font);
		al_draw_textf(font, color, pos_x, pos_y - (font_h / 2), ALLEGRO_ALIGN_CENTER, "%s", text.c_str());
	}
	else
	{
		al_draw_textf(font, color, pos_x, pos_y, ALLEGRO_ALIGN_LEFT, "%s", text.c_str());
	}

	al_use_transform(&prev_t);
}

// resize display in appropriative way
void TransformDisplay(ALLEGRO_DISPLAY *display)
{
	al_acknowledge_resize(display);

	ALLEGRO_TRANSFORM t;

	float scaleX = al_get_display_width(display) / DISPLAY_WIDTH;
	float scaleY = al_get_display_height(display) / DISPLAY_HEIGHT;
	float scale = scaleX < scaleY ? scaleX : scaleY;

	al_identity_transform(&t);
	al_scale_transform(&t, scale, scale);
	if (scale == scaleX)
	{
		float y = (al_get_display_height(display) / 2) - (DISPLAY_HEIGHT * scale * 0.5f);
		al_translate_transform(&t, 0, y);
	}
	else if (scale == scaleY)
	{
		float x = (al_get_display_width(display) / 2) - (DISPLAY_WIDTH * scale * 0.5f);
		al_translate_transform(&t, x, 0);
	}
	al_use_transform(&t);
}
// transform mouse coordinates
void TransformMouse(float *mx, float *my)
{
	ALLEGRO_TRANSFORM t;
	al_copy_transform(&t, al_get_current_transform());
	al_invert_transform(&t);
	al_transform_coordinates(&t, mx, my);
}

void ChooseMusicFile(App *app, bool is_playing)
{
	ALLEGRO_FILECHOOSER *filechooser;

	filechooser = al_create_native_file_dialog(0, "Choose your music file (.ogg)", "*.*;*.ogg;", 1);
	if (!filechooser)
	{
		al_show_native_message_box(0, "ERROR", "Failed to create native file dialog", "", 0, ALLEGRO_MESSAGEBOX_ERROR);
	}
	else
	{
		al_show_native_file_dialog(app->display, filechooser);
		unsigned num_files = al_get_native_file_dialog_count(filechooser);
		if (num_files == 1)
		{
			const char *path = al_get_native_file_dialog_path(filechooser, 0);
			al_destroy_audio_stream(app->audio_stream);
			app->audio_stream = 0;
			app->audio_stream = al_load_audio_stream(path, 2, 2048);
			if (!app->audio_stream)
			{
				al_show_native_message_box(0, "ERROR", "", "Failed to load your music file", 0, ALLEGRO_MESSAGEBOX_ERROR);
			}
			else
			{
				al_set_audio_stream_playmode(app->audio_stream, ALLEGRO_PLAYMODE_LOOP);
				al_attach_audio_stream_to_mixer(app->audio_stream, al_get_default_mixer());
				al_set_audio_stream_gain(app->audio_stream, app->volume);
				al_set_audio_stream_playing(app->audio_stream, is_playing);
			}
		}
		else if (num_files > 1)
			al_show_native_message_box(0, "WARN", "", "You can pick only one music file", 0, ALLEGRO_MESSAGEBOX_WARN);
		else
			al_show_native_message_box(0, "OK", "", "If you do not have .ogg music\nyou can always convert it", 0, ALLEGRO_MESSAGEBOX_OK_CANCEL);
	}

	al_destroy_native_file_dialog(filechooser);
}

bool SaveSettings(App *app)
{
	if (app)
	{
		std::ofstream out_file;
		out_file.open("data//settings.txt");
		if (out_file.is_open())
		{
			out_file << app->volume << "\n";
			out_file << app->volume_sounds << "\n";
			out_file << app->play_sounds << "\n";
			out_file << app->time_exercise << "\n";
			out_file << app->time_rest << "\n";
			out_file << app->sets << "\n";

			out_file.close();

			return true;
		}
	}

	return false;
}
bool LoadSettings(App *app)
{
	if (app)
	{
		std::ifstream in_file;
		in_file.open("data//settings.txt");
		if (in_file.is_open())
		{
			{
				float volume;
				in_file >> volume;
				if ((volume >= MIN_VOLUME) && (volume <= MAX_VOLUME))
					app->volume = volume;
			}
			{
				float volume_sounds;
				in_file >> volume_sounds;
				if ((volume_sounds >= MIN_VOLUME) && (volume_sounds <= MAX_VOLUME))
					app->volume_sounds = volume_sounds;
			}
			{
				bool play_sounds;
				in_file >> play_sounds;
				app->play_sounds = play_sounds;
			}
			{
				int time_exercise;
				in_file >> time_exercise;
				if ((time_exercise >= 0) && (time_exercise < 9999))
					app->time_exercise = time_exercise;
			}
			{
				int time_rest;
				in_file >> time_rest;
				if ((time_rest >= 0) && (time_rest < 9999))
					app->time_rest = time_rest;
			}
			{
				int sets;
				in_file >> sets;
				if ((sets >= 0) && (sets < 9999))
					app->sets = sets;
			}

			in_file.close();

			return true;
		}
	}

	return false;
}