// ==================================================
// file: IntroLoop.cpp
// project: Interval Timer
// author: Paulina Kalicka
// ==================================================

#include "Header.h"

#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>

STATE IntroLoop(App *app)
{
	STATE next_state = STATE::END;

	bool error = false;

	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	if (!event_queue)
	{
		al_show_native_message_box(0, "ERROR", "Failed to create event queue", "", 0, ALLEGRO_MESSAGEBOX_ERROR);
		error = true;
	}

	ALLEGRO_TIMER *timer = al_create_timer(1 / 60.0);
	if (!timer)
	{
		al_show_native_message_box(0, "ERROR", "Failed to create timer", "", 0, ALLEGRO_MESSAGEBOX_ERROR);
		error = true;
	}

	ALLEGRO_SAMPLE *sample_intro = al_load_sample("resources//samples//intro.ogg");
	if (!sample_intro)
	{
		al_show_native_message_box(0, "ERROR", "Failed to load intro.ogg", "", 0, ALLEGRO_MESSAGEBOX_ERROR);
		error = true;
	}

	ALLEGRO_BITMAP *bitmap_timer_logo = al_load_bitmap("resources//bitmaps//timer.png");
	if (!bitmap_timer_logo)
	{
		al_show_native_message_box(0, "ERROR", "Failed to load timer.png", "", 0, ALLEGRO_MESSAGEBOX_ERROR);
		error = true;
	}

	if (!error)
	{
		al_register_event_source(event_queue, al_get_display_event_source(app->display));
		al_register_event_source(event_queue, al_get_timer_event_source(timer));
		al_register_event_source(event_queue, al_get_mouse_event_source());

		bool end_intro_loop = false;
		bool draw_intro_loop = false;

		unsigned timer_ticks = 0;

		// variables for drawing timer logo
		float logo_a = 0;
		float logo_cx = al_get_bitmap_width(bitmap_timer_logo) / 2;
		float logo_cy = al_get_bitmap_height(bitmap_timer_logo) / 2;
		float logo_px = DISPLAY_WIDTH / 2;
		float logo_py = DISPLAY_HEIGHT / 2;
		float logo_scale = 0.01;
		float logo_angle = 0;

		// variables for drawing text 'click to continue'
		bool text_draw = false;
		bool text_add_a = false;
		unsigned char text_a = 0;

		al_play_sample(sample_intro, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);

		al_start_timer(timer);
		while (!end_intro_loop)
		{
			ALLEGRO_EVENT event;
			al_wait_for_event(event_queue, &event);

			switch (event.type)
			{
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
			{
				next_state = STATE::END;
				end_intro_loop = true;
			}
			break;
			case ALLEGRO_EVENT_DISPLAY_RESIZE:
			{
				TransformDisplay(app->display);
			}
			break;
			case ALLEGRO_EVENT_TIMER:
			{
				timer_ticks++;
				draw_intro_loop = true;

				app->my_mouse.UpdateCounter(timer_ticks);

				if (!text_draw)
				{
					if (logo_a < 1.0)
						logo_a += 0.01;
					else
						logo_a = 1.0;
					if (logo_scale < 1.0)
						logo_scale += 0.01;
					if (logo_angle < 12.5)
						logo_angle += 0.1;
					else
					{
						logo_angle = 12.56637;
						text_draw = true;
					}
				}
				else
				{
					if (logo_py > 200)
						logo_py--;
					if (logo_scale > 0.5)
						logo_scale -= 0.02;
					else
						logo_scale = 0.5;
				}

				if (text_draw)
				{
					if ((text_a == 0) || (text_a == 255))
						text_add_a = !text_add_a;
					if (text_add_a)
						text_a += 3;
					else
						text_a -= 3;
				}
			}
			break;
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			{
				next_state = STATE::MAIN;
				end_intro_loop = true;
			}
			break;
			case ALLEGRO_EVENT_MOUSE_AXES:
			{
				float mx = event.mouse.x;
				float my = event.mouse.y;
				TransformMouse(&mx, &my);

				app->my_mouse.UpdatePositions(mx, my);
			}
			break;

			default:
				break;
			}

			if (draw_intro_loop && al_is_event_queue_empty(event_queue))
			{
				draw_intro_loop = false;

				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_filled_rectangle(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, al_map_rgb(200, 255, 0));

				al_draw_tinted_scaled_rotated_bitmap(bitmap_timer_logo,
													 al_map_rgba_f(0, 0, 0, logo_a), logo_cx, logo_cy,
													 logo_px, logo_py,
													 logo_scale, logo_scale, logo_angle, 0);

				al_draw_text(app->font, al_map_rgba(0, 0, 0, text_a), DISPLAY_WIDTH / 2, 450,
							 ALLEGRO_ALIGN_CENTER, "click to continue");

				app->my_mouse.Draw();

				al_flip_display();
			}
		}
		al_stop_timer(timer);
	}

	al_destroy_event_queue(event_queue);

	al_destroy_timer(timer);

	al_destroy_sample(sample_intro);

	al_destroy_bitmap(bitmap_timer_logo);

	return next_state;
}