// ___ ==================================================
// ___ file: TimerLoop.cpp
// ___ project: Interval Timer
// ___ author: Paulina Kalicka
// ___ ==================================================

#include "Header.h"
#include "Button.h"

#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>

enum class TIMER_STATE
{
	COUNTING,
	EXERCISE,
	REST,
	FINISHED,
	STOPPED
};

class Interval
{
public:
	enum class INTERVAL_STATE
	{
		COUNTING,
		EXERCISE,
		REST
	};

	INTERVAL_STATE state;

	double secs_interval;
	double secs_elapsed;
	double secs_stopped;
	double secs_last_change;
	double time_last_change;
	double to_elapse;
	double elapsed;

	bool stopped = false;

	Interval(double secs_interval, INTERVAL_STATE state)
	{
		this->state = state;

		this->secs_interval = secs_interval;
		this->secs_elapsed = 0.0;
		this->secs_stopped = 0.0;
		this->secs_last_change = 0.0;
		this->time_last_change = al_get_time();
		this->to_elapse = secs_interval;
		this->elapsed = 0.0;

		this->stopped = 0;
	}
	~Interval() {}

	void Continue()
	{
		secs_stopped += secs_last_change;
		secs_last_change = 0.0;
		time_last_change = al_get_time();
		stopped = false;
	}
	void Stop()
	{
		secs_elapsed += secs_last_change;
		secs_last_change = 0.0;
		time_last_change = al_get_time();
		stopped = true;
	}

	bool IfDone()
	{
		secs_last_change = al_get_time() - time_last_change;
		elapsed = secs_interval - to_elapse;

		if(!stopped)
		{
			to_elapse = secs_interval - secs_elapsed - secs_last_change;

			if(secs_elapsed + secs_last_change >= secs_interval)
			{
				return true;
			}
		}

		return false;
	}
};

STATE TimerLoop(App* app)
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

	ALLEGRO_SAMPLE* sample_exercise = al_load_sample("res//samples//sample_exercise.ogg");
	if(!sample_exercise)
	{
		al_show_native_message_box(0, "ERROR", "Failed to load sample_exercise.ogg", "", 0, ALLEGRO_MESSAGEBOX_ERROR);
		error = true;
	}
	ALLEGRO_SAMPLE* sample_finished = al_load_sample("res//samples//sample_finished.ogg");
	if(!sample_finished)
	{
		al_show_native_message_box(0, "ERROR", "Failed to load sample_finished.ogg", "", 0, ALLEGRO_MESSAGEBOX_ERROR);
		error = true;
	}
	ALLEGRO_SAMPLE* sample_rest = al_load_sample("res//samples//sample_rest.ogg");
	if(!sample_rest)
	{
		al_show_native_message_box(0, "ERROR", "Failed to load sample_rest.ogg", "", 0, ALLEGRO_MESSAGEBOX_ERROR);
		error = true;
	}

	if(!error)
	{
		al_register_event_source(event_queue, al_get_display_event_source(app->display));
		al_register_event_source(event_queue, al_get_timer_event_source(timer));
		al_register_event_source(event_queue, al_get_mouse_event_source());

		bool end_timer_loop = false;
		bool draw_timer_loop = false;

		unsigned timer_ticks = 0;
		unsigned done_sets = 0;

		Button buttons[] =
		{
			Button(500, 10, 300, 50, 1, "MENU"),
			Button(200, 100, 300, 50, 1, "RESTART"),
			Button(500, 100, 300, 50, 1, "STOP"),
			Button(0, 10, 200, 50, app->audio_stream ? al_get_audio_stream_playing(app->audio_stream) : 1, "MUSIC"),
			Button(200, 10, 50, 50, app->volume >= MAX_VOLUME ? 0 : 1, "+"),
			Button(250, 10, 50, 50, app->volume <= MIN_VOLUME ? 0 : 1, "-"),
			Button(300, 10, 200, 50, 1, "CHOOSE YOUR MUSIC"),
			Button(0, 100, 100, 50, app->play_sounds, "SOUNDS"),
			Button(100, 100, 50, 50, app->volume_sounds >= MAX_VOLUME ? 0 : 1, "+"),
			Button(150, 100, 50, 50, app->volume_sounds <= MIN_VOLUME ? 0 : 1, "-")
		};

		Interval interval = Interval(3.0, Interval::INTERVAL_STATE::COUNTING);

		TIMER_STATE timer_state = TIMER_STATE::COUNTING;
		TIMER_STATE prev_timer_state = TIMER_STATE::COUNTING;

		al_start_timer(timer);
		while(!end_timer_loop)
		{
			ALLEGRO_EVENT event;
			al_wait_for_event(event_queue, &event);

			switch(event.type)
			{
				case ALLEGRO_EVENT_DISPLAY_CLOSE: { next_state = STATE::END; end_timer_loop = true; }break;
				case ALLEGRO_EVENT_DISPLAY_RESIZE: { TransformDisplay(app->display); }break;
				case ALLEGRO_EVENT_TIMER:
				{
					timer_ticks++;
					draw_timer_loop = true;

					if(timer_state != TIMER_STATE::FINISHED)
					{
						if(interval.IfDone())
						{
							draw_timer_loop = false;

							switch(interval.state)
							{
								case Interval::INTERVAL_STATE::COUNTING:
								{
									if(app->play_sounds)
										al_play_sample(sample_exercise, app->volume_sounds, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
									interval = Interval(app->time_exercise, Interval::INTERVAL_STATE::EXERCISE);
									timer_state = TIMER_STATE::EXERCISE;
								}break;
								case Interval::INTERVAL_STATE::EXERCISE:
								{
									if((++done_sets) < app->sets)
									{
										if(app->play_sounds)
											al_play_sample(sample_rest, app->volume_sounds, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
										interval = Interval(app->time_rest, Interval::INTERVAL_STATE::REST);
										timer_state = TIMER_STATE::REST;
									}
									else
									{
										if(app->play_sounds)
											al_play_sample(sample_finished, app->volume_sounds, 1, 1, ALLEGRO_PLAYMODE_ONCE, 0);
										timer_state = TIMER_STATE::FINISHED;
									}
								}break;
								case Interval::INTERVAL_STATE::REST:
								{
									if(app->play_sounds)
										al_play_sample(sample_exercise, app->volume_sounds, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
									interval = Interval(app->time_exercise, Interval::INTERVAL_STATE::EXERCISE);
									timer_state = TIMER_STATE::EXERCISE;
								}break;

								default:break;
							}
						}
					}

					app->my_mouse.UpdateCounter(timer_ticks);
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
				}break;
				case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
				{
					if(buttons[0].hovered)
					{
						next_state = STATE::MAIN;
						end_timer_loop = true;
					}

					// note: restart timer
					else if(buttons[1].hovered)
					{
						done_sets = 0;
						interval = Interval(3.0, Interval::INTERVAL_STATE::COUNTING);
						timer_state = TIMER_STATE::COUNTING;
						buttons[2].on = true;
					}

					// note: stop/continue timer
					else if(buttons[2].hovered)
					{
						if(timer_state == TIMER_STATE::STOPPED)
						{
							interval.Continue();
							buttons[2].on = true;
							timer_state = prev_timer_state;
						}
						else
						{
							interval.Stop();
							buttons[2].on = false;
							prev_timer_state = timer_state;
							timer_state = TIMER_STATE::STOPPED;
						}
					}

					// note: buttons that affect sounds
					else if(buttons[7].hovered) { buttons[7].on = !buttons[7].on; app->play_sounds = buttons[7].on; }
					else if(buttons[8].hovered)
					{
						if(buttons[8].on)
						{
							app->volume_sounds += 0.5;
							if(app->volume_sounds >= MAX_VOLUME) { app->volume_sounds = MAX_VOLUME; buttons[8].on = false; }
							buttons[9].on = true;
						}
						else app->pop_up.New("max volume!", 400, &buttons[8]);
					}
					else if(buttons[9].hovered)
					{
						if(buttons[9].on)
						{
							app->volume_sounds -= 0.5;
							if(app->volume_sounds <= MIN_VOLUME) { app->volume_sounds = MIN_VOLUME; buttons[9].on = false; }
							buttons[8].on = true;
						}
						else app->pop_up.New("min volume!", 400, &buttons[9]);
					}

					// note: file-chooser
					else if(buttons[6].hovered)
					{
						ChooseMusicFile(app, buttons[3].on);
					}

					// note: buttons that affect audio stream
					if(app->audio_stream)
					{
						if(buttons[3].hovered) { buttons[3].on = !buttons[3].on; al_set_audio_stream_playing(app->audio_stream, buttons[3].on); }
						else if(buttons[4].hovered)
						{
							if(buttons[4].on)
							{
								app->volume += 0.5;
								if(app->volume >= MAX_VOLUME) { app->volume = MAX_VOLUME; buttons[4].on = false; }
								buttons[5].on = true;
								al_set_audio_stream_gain(app->audio_stream, app->volume);
							}
							else app->pop_up.New("max volume!", 400, &buttons[4]);
						}
						else if(buttons[5].hovered)
						{
							if(buttons[5].on)
							{
								app->volume -= 0.5;
								if(app->volume <= MIN_VOLUME) { app->volume = MIN_VOLUME; buttons[5].on = false; }
								buttons[4].on = true;
								al_set_audio_stream_gain(app->audio_stream, app->volume);
							}
							else app->pop_up.New("min volume!", 400, &buttons[5]);
						}
					}
					else if(buttons[3].hovered)app->pop_up.New("choose music to turn on/off!", 400, &buttons[3]);
					else if(buttons[4].hovered)app->pop_up.New("choose music to turn up!", 400, &buttons[4]);
					else if(buttons[5].hovered)app->pop_up.New("choose music to turn down!", 400, &buttons[5]);
				}break;

				default:break;
			}

			if(draw_timer_loop && al_is_event_queue_empty(event_queue))
			{
				draw_timer_loop = false;

				al_clear_to_color(al_map_rgb(0, 0, 0));

				switch(timer_state)
				{
					case TIMER_STATE::COUNTING:
					{
						al_draw_filled_rectangle(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT,
												 al_map_rgb(200, 255, 0));
						DrawText(app->font, 500, 100, 0, 400, 250, 1,
								 al_map_rgb(0, 0, 0), "GET READY!");
					}break;
					case TIMER_STATE::EXERCISE:
					{
						al_draw_filled_rectangle(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT,
												 al_map_rgb(255, 120, 0));
						DrawText(app->font, 500, 100, 0, 400, 250, 1,
								 al_map_rgb(0, 0, 0), "EXERCISE");
					}break;
					case TIMER_STATE::REST:
					{
						al_draw_filled_rectangle(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT,
												 al_map_rgb(20, 160, 0));
						DrawText(app->font, 500, 100, 0, 400, 250, 1,
								 al_map_rgb(0, 0, 0), "REST");
					}break;
					case TIMER_STATE::FINISHED:
					{
						al_draw_filled_rectangle(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT,
												 al_map_rgb(200, 255, 0));
						DrawText(app->font, 500, 100, 0, 400, 300, 1,
								 al_map_rgb(0, 0, 0), "FINISHED!");
					}break;
					case TIMER_STATE::STOPPED:
					{
						al_draw_filled_rectangle(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT,
												 al_map_rgb(200, 255, 0));
						al_draw_filled_rectangle(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT,
												 al_map_rgba(132, 132, 132, 132));
						DrawText(app->font, 500, 100, 0, 400, 250, 1,
								 al_map_rgb(0, 0, 0), "STOPPED");
					}break;

					default:break;
				}

				// note: show time and done sets
				if(timer_state != TIMER_STATE::FINISHED)
				{
					// note: show time to elapse as: interval.to_elapse+1 so we have 1-20 instead of 0-19
					DrawText(app->font, 500, 100, 0, 400, 350, 1,
							 al_map_rgb(0, 0, 0), std::to_string(int(interval.to_elapse) + 1));
					DrawText(app->font, 500, 50, 0, 300, 400, 0, al_map_rgb(0, 0, 0),
							 std::to_string((interval.elapsed)));
					DrawText(app->font, 500, 100, 0, 400, 500, 1,
							 al_map_rgb(0, 0, 0), std::to_string(done_sets));
				}

				for(Button& button : buttons)
					button.Draw(app->font);

				// note: music play status bar
				al_draw_filled_rectangle(0, 70, DISPLAY_WIDTH, 90, al_map_rgb(0, 0, 0));
				if(app->audio_stream)
				{
					unsigned length = (al_get_audio_stream_position_secs(app->audio_stream) / al_get_audio_stream_length_secs(app->audio_stream)) * DISPLAY_WIDTH;
					al_draw_filled_rectangle(0, 75, length, 85, al_map_rgb(255, 200, 0));
				}

				app->my_mouse.Draw();
				app->pop_up.Draw(app->font);

				al_flip_display();
			}
		}
		al_stop_timer(timer);
	}

	al_destroy_event_queue(event_queue);

	al_destroy_timer(timer);

	al_destroy_sample(sample_exercise);
	al_destroy_sample(sample_finished);
	al_destroy_sample(sample_rest);

	return next_state;
}