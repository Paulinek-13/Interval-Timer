// ==================================================
// file: InfoLoop.cpp
// project: Interval Timer
// author: Paulina Kalicka
// ==================================================

#include "Header.h"
#include "Button.h"

#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>

STATE InfoLoop(App *app)
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

	ALLEGRO_BITMAP *bitmap_PAULINEK_logo = al_load_bitmap("resources//bitmaps//paulinek.png");
	if (!bitmap_PAULINEK_logo)
	{
		al_show_native_message_box(0, "ERROR", "Failed to load paulinek.png", "", 0, ALLEGRO_MESSAGEBOX_ERROR);
		error = true;
	}

	if (!error)
	{
		al_register_event_source(event_queue, al_get_display_event_source(app->display));
		al_register_event_source(event_queue, al_get_timer_event_source(timer));
		al_register_event_source(event_queue, al_get_mouse_event_source());

		bool end_info_loop = false;
		bool draw_info_loop = false;

		unsigned timer_ticks = 0;

		Button buttons[] =
			{
				Button(0, 300, DISPLAY_WIDTH, 50, 1, "MENU"),
				Button(0, 350, DISPLAY_WIDTH, 50, 1, "RUN INTRO")};

		al_start_timer(timer);
		while (!end_info_loop)
		{
			ALLEGRO_EVENT event;
			al_wait_for_event(event_queue, &event);

			switch (event.type)
			{
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
			{
				next_state = STATE::END;
				end_info_loop = true;
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
				draw_info_loop = true;

				app->my_mouse.UpdateCounter(timer_ticks);
			}
			break;
			case ALLEGRO_EVENT_MOUSE_AXES:
			{
				float mx = event.mouse.x;
				float my = event.mouse.y;
				TransformMouse(&mx, &my);

				app->my_mouse.UpdatePositions(mx, my);

				for (Button &button : buttons)
					button.CheckIfHovered(mx, my);
			}
			break;
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			{
				if (buttons[0].hovered)
				{
					next_state = STATE::MAIN;
					end_info_loop = true;
				}
				else if (buttons[1].hovered)
				{
					next_state = STATE::INTRO;
					end_info_loop = true;
				}
			}
			break;

			default:
				break;
			}

			if (draw_info_loop && al_is_event_queue_empty(event_queue))
			{
				draw_info_loop = false;

				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_filled_rectangle(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, al_map_rgb(200, 255, 0));

				{
					DrawText(app->font, 900, 50, 0, DISPLAY_WIDTH / 2, 90, 1, al_map_rgb(0, 0, 0),
							 "See README.md for credits info");
				}

				al_draw_bitmap(bitmap_PAULINEK_logo, 290, 200, 0);

				for (Button &button : buttons)
					button.Draw(app->font);

				app->my_mouse.Draw();

				al_flip_display();
			}
		}
		al_stop_timer(timer);
	}

	al_destroy_event_queue(event_queue);

	al_destroy_timer(timer);

	al_destroy_bitmap(bitmap_PAULINEK_logo);

	return next_state;
}