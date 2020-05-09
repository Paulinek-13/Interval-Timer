// ___ ==================================================
// ___ file: MainLoop.cpp
// ___ project: Interval Timer
// ___ author: Paulina Kalicka
// ___ ==================================================

#include "Header.h"
#include "Button.h"
#include "Input.h"

#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>

STATE MainLoop(App* app)
{
	STATE next_state = STATE::END;

	bool error = false;

	ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
	if(!event_queue)
	{
		al_show_native_message_box(0, "ERROR", "Failed to create event queue", "", 0, ALLEGRO_MESSAGEBOX_ERROR);
		error = true;
	}

	ALLEGRO_TIMER* timer = al_create_timer(1 / 60.0);
	if(!timer)
	{
		al_show_native_message_box(0, "ERROR", "Failed to create timer", "", 0, ALLEGRO_MESSAGEBOX_ERROR);
		error = true;
	}

	ALLEGRO_SAMPLE* sample_key_down = al_load_sample("res//samples//sample_key_down.ogg");
	if(!sample_key_down)
	{
		al_show_native_message_box(0, "ERROR", "Failed to load sample_key_down.ogg", "", 0, ALLEGRO_MESSAGEBOX_ERROR);
		error = true;
	}

	if(!error)
	{
		al_register_event_source(event_queue, al_get_display_event_source(app->display));
		al_register_event_source(event_queue, al_get_timer_event_source(timer));
		al_register_event_source(event_queue, al_get_mouse_event_source());
		al_register_event_source(event_queue, al_get_keyboard_event_source());

		bool end_main_loop = false;
		bool draw_main_loop = false;

		unsigned timer_ticks = 0;

		Button buttons[] =
		{
			Button(0, 10, 200, 50, app->audio_stream ? al_get_audio_stream_playing(app->audio_stream) : 1, "MUSIC"),
			Button(200, 10, 50, 50, app->volume >= MAX_VOLUME ? 0 : 1, "+"),
			Button(250, 10, 50, 50, app->volume <= MIN_VOLUME ? 0 : 1, "-"),
			Button(500, 10, 300, 50, 1, "INFO & CREDITS"),
			Button(200, 100, 400, 100, 1, "START"),
			Button(300, 10, 200, 50, 1, "CHOOSE YOUR MUSIC"),
			Button(0, 540, 400, 50, 1, "LOAD SETTINGS"),
			Button(400, 540, 400, 50, 1, "SAVE SETTINGS")
		};

		Input inputs[] =
		{
			Input(200, 210, 400, 100, 1, "exercise: ", app->time_exercise),
			Input(200, 320, 400, 100, 1, "rest: ", app->time_rest),
			Input(200, 430, 400, 100, 1, "sets: ", app->sets)
		};
		// pointer to input button that is actually active for typing
		Input* input_active = 0;
		// pointer to input button that can "hovered" using key up/down
		Input* input_hovered = &inputs[0];

		al_start_timer(timer);
		while(!end_main_loop)
		{
			ALLEGRO_EVENT event;
			al_wait_for_event(event_queue, &event);

			switch(event.type)
			{
				case ALLEGRO_EVENT_DISPLAY_CLOSE: { next_state = STATE::END; end_main_loop = true; }break;
				case ALLEGRO_EVENT_DISPLAY_RESIZE: { TransformDisplay(app->display); }break;
				case ALLEGRO_EVENT_TIMER:
				{
					timer_ticks++;
					draw_main_loop = true;

					app->my_mouse.UpdateCounter(timer_ticks);
				}break;
				case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
				{
					// note: when not typing to input buttons
					if(!input_active)
					{
						for(Input& input : inputs)
						{
							if(input.hovered)
							{
								// note: start taking keyboard input to activated button
								input_active = &input;
								input.on = false;
								input.str_value = "";
								break;
							}
						}

						if(buttons[3].hovered)
						{
							next_state = STATE::INFO;
							end_main_loop = true;
						}
						else if(buttons[4].hovered)
						{
							// note: prevent from starting timer with sets set to 0 or time exercise set to 0
							if((inputs[0].value != 0) && (inputs[2].value != 0))
							{
								app->time_exercise = inputs[0].value;
								app->time_rest = inputs[1].value;
								app->sets = inputs[2].value;

								next_state = STATE::TIMER;
								end_main_loop = true;
							}
							else if(inputs[0].value <= 0) app->pop_up.New("set time exercise!", 300, &buttons[4]);
							else if(inputs[2].value <= 0) app->pop_up.New("set number of sets!", 300, &buttons[4]);
						}

						// note: file-chooser
						else if(buttons[5].hovered)
						{
							ChooseMusicFile(app, buttons[0].on);
						}

						// note: load settings
						else if(buttons[6].hovered)
						{
							int result = al_show_native_message_box(app->display, "QUESTION", "",
																	"Do you want to load settings that have been saved?",
																	0, ALLEGRO_MESSAGEBOX_QUESTION);
							if(result == 1)
								if(!LoadSettings(app))
									al_show_native_message_box(app->display, "ERROR", "",
															   "Failed to load settings",
															   0, ALLEGRO_MESSAGEBOX_ERROR);
						}

						// note: save settings
						else if(buttons[7].hovered)
						{
							int result = al_show_native_message_box(app->display, "QUESTION", "",
																	"Do you want to save your current settings?",
																	0, ALLEGRO_MESSAGEBOX_QUESTION);
							if(result == 1)
								if(!SaveSettings(app))
									al_show_native_message_box(app->display, "ERROR", "",
															   "Failed to save your current settings",
															   0, ALLEGRO_MESSAGEBOX_ERROR);
						}

						// note: buttons that affect audio stream
						if(app->audio_stream)
						{
							if(buttons[0].hovered) { buttons[0].on = !buttons[0].on; al_set_audio_stream_playing(app->audio_stream, buttons[0].on); }
							else if(buttons[1].hovered)
							{
								if(buttons[1].on)
								{
									app->volume += 0.5;
									if(app->volume >= MAX_VOLUME) { app->volume = MAX_VOLUME; buttons[1].on = false; }
									buttons[2].on = true;
									al_set_audio_stream_gain(app->audio_stream, app->volume);
								}
								else app->pop_up.New("max volume!", 400, &buttons[1]);
							}
							else if(buttons[2].hovered)
							{
								if(buttons[2].on)
								{
									app->volume -= 0.5;
									if(app->volume <= MIN_VOLUME) { app->volume = MIN_VOLUME; buttons[2].on = false; }
									buttons[1].on = true;
									al_set_audio_stream_gain(app->audio_stream, app->volume);
								}
								else app->pop_up.New("min volume!", 400, &buttons[2]);
							}
						}
						else if(buttons[0].hovered)app->pop_up.New("choose music to turn on/off!", 400, &buttons[0]);
						else if(buttons[1].hovered)app->pop_up.New("choose music to turn up!", 400, &buttons[1]);
						else if(buttons[2].hovered)app->pop_up.New("choose music to turn down!", 400, &buttons[2]);
					}
					else
					{
						// note: as it enter key was typed
						input_active->ConfirmInput();
						input_active = 0;
					}
				}break;
				case ALLEGRO_EVENT_MOUSE_AXES:
				{
					float mx = event.mouse.x;
					float my = event.mouse.y;
					TransformMouse(&mx, &my);

					app->my_mouse.UpdatePositions(mx, my);
					app->pop_up.Update(mx, my);

					for(Button& button : buttons)
						button.CheckIfHovered(mx, my);
					for(Input& input : inputs)
						input.CheckIfHovered(mx, my);
				}break;
				case ALLEGRO_EVENT_KEY_DOWN:
				{
					// note: take keyboard input only when one input button is activated
					if(input_active)
					{
						input_active->hovered = false;
						if(input_active->KeysInput(event, sample_key_down))
							input_active = 0;
					}
					else
					{
						// whether defined key was typed
						bool result = 1;

						input_hovered->hovered = false;
						switch(event.keyboard.keycode)
						{
							case ALLEGRO_KEY_UP:
								if(input_hovered != &inputs[0])
									input_hovered--;
								break;
							case ALLEGRO_KEY_DOWN:
								if(input_hovered != &inputs[2])
									input_hovered++;
								break;
							case ALLEGRO_KEY_ENTER:
								input_active = input_hovered;
								input_active->on = false;
								input_active->str_value = "";
								break;

							default:result = 0; break;
						}
						if(input_hovered->on && result)input_hovered->hovered = true;
						if(result)al_play_sample(sample_key_down, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
					}
				}break;

				default:break;
			}

			if(draw_main_loop && al_is_event_queue_empty(event_queue))
			{
				draw_main_loop = false;

				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_filled_rectangle(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, al_map_rgb(200, 255, 0));

				for(Button& button : buttons)
					button.Draw(app->font);

				// note: music play status bar
				al_draw_filled_rectangle(0, 70, DISPLAY_WIDTH, 90, al_map_rgb(0, 0, 0));
				if(app->audio_stream)
				{
					unsigned length = (al_get_audio_stream_position_secs(app->audio_stream) / al_get_audio_stream_length_secs(app->audio_stream)) * DISPLAY_WIDTH;
					al_draw_filled_rectangle(0, 75, length, 85, al_map_rgb(255, 200, 0));
				}

				if(input_active)
				{
					al_draw_filled_rectangle(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, al_map_rgba(32, 32, 32, 132));
					al_draw_rectangle(input_active->pos_x, input_active->pos_y,
									  input_active->pos_x + input_active->width,
									  input_active->pos_y + input_active->height,
									  al_map_rgb(200, 255, 0), 20);
				}

				for(Input& input : inputs)
					input.Draw(app->font);

				app->my_mouse.Draw();
				app->pop_up.Draw(app->font);

				al_flip_display();
			}
		}
		al_stop_timer(timer);
	}

	al_destroy_event_queue(event_queue);

	al_destroy_timer(timer);

	al_destroy_sample(sample_key_down);

	return next_state;
}