// ___ ==================================================
// ___ file: main.cpp
// ___ project: Interval Timer
// ___ author: Paulina Kalicka
// ___ ==================================================

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "Header.h"

int main()
{
	srand(time(NULL));

	if(!al_init())
	{
		al_show_native_message_box(0, "ERROR", "Failed to init Allegro 5", "", 0, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	if(!al_init_native_dialog_addon())
	{
		al_show_native_message_box(0, "ERROR", "Failed to init native dialog addon", "", 0, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	if(!al_init_primitives_addon())
	{
		al_show_native_message_box(0, "ERROR", "Failed to init primitives addon", "", 0, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	if(!al_init_font_addon())
	{
		al_show_native_message_box(0, "ERROR", "Failed to init font addon", "", 0, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	if(!al_init_ttf_addon())
	{
		al_show_native_message_box(0, "ERROR", "Failed to init ttf addon", "", 0, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	if(!al_init_image_addon())
	{
		al_show_native_message_box(0, "ERROR", "Failed to init image addon", "", 0, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	if(!al_install_keyboard())
	{
		al_show_native_message_box(0, "ERROR", "Failed to install keyboard", "", 0, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	if(!al_install_mouse())
	{
		al_show_native_message_box(0, "ERROR", "Failed to install mouse", "", 0, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	if(!al_install_audio())
	{
		al_show_native_message_box(0, "ERROR", "Failed to install audio", "", 0, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	if(!al_init_acodec_addon())
	{
		al_show_native_message_box(0, "ERROR", "Failed to init acodec addon", "", 0, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	al_set_app_name("Interval Timer");
	al_set_org_name("PAULINEK");
	al_reserve_samples(5);
	al_set_new_display_flags(ALLEGRO_RESIZABLE);

	// note: centre display
	{
		ALLEGRO_MONITOR_INFO info;
		al_get_monitor_info(0, &info);
		al_set_new_window_position((info.x2 / 2) - (DISPLAY_WIDTH / 2), (info.y2 / 2) - (DISPLAY_HEIGHT / 2));
	}

	bool error = false;

	App app;

	app.display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	if(!app.display)
	{
		al_show_native_message_box(0, "ERROR", "Failed to create display", "", 0, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	app.bitmap_icon = al_load_bitmap("res//icon.png");
	if(!app.bitmap_icon)
	{
		al_show_native_message_box(0, "ERROR", "Failed to load icon.png", "", 0, ALLEGRO_MESSAGEBOX_ERROR);
		error = true;
	}

	app.font = al_load_font("res//fonts//font_kenny_pixel_square.ttf", 20, 0);
	if(!app.font)
	{
		al_show_native_message_box(0, "ERROR", "Failed to load font_kenny_pixel_square.ttf", "", 0, ALLEGRO_MESSAGEBOX_ERROR);
		error = true;
	}

	if(!error)
	{
		al_hide_mouse_cursor(app.display);
		al_set_display_icon(app.display, app.bitmap_icon);
		al_set_window_title(app.display, "Interval Timer");

		// pointer to a function that loops state of app
		STATE(*state_ptr)(App*) = IntroLoop;

		bool end_all = false;

		while(!end_all)
		{
			switch(state_ptr(&app))
			{
				case STATE::END:
					end_all = true;
					break;
				case STATE::INTRO:
					state_ptr = IntroLoop;
					break;
				case STATE::MAIN:
					state_ptr = MainLoop;
					break;
				case STATE::INFO:
					state_ptr = InfoLoop;
					break;
				case STATE::TIMER:
					state_ptr = TimerLoop;
					break;
				default:
					// note: it should not be there...
					end_all = true;
					break;
			}
		}
	}

	al_destroy_display(app.display);

	al_destroy_bitmap(app.bitmap_icon);

	al_destroy_font(app.font);

	al_destroy_audio_stream(app.audio_stream);

	return 0;
}