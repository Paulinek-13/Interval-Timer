/* ! ################################################## */

/*
	TIMER

	This is a simple interval timer.

	*> CREATED:
	-> 31st January 2020
	-> PAULINEK ( Paulina Kalicka )

	*> IMPROVED:
	-> 1st March 2020
	-> PAULINEK (Paulina Kalicka )
*/

/*
	Sound effects obtained from www.zapsplat.com
	Icon made by Gregor Cresnar from www.flaticon.com
*/

/*
	If you have any questions, complaints
	or you've had an error, and in every situation
	when you just want to contact me,
	please write an email to address
	kalicka.paulina@outlook.com
*/

/* ! ################################################## */

/* ! INCLUDING */

#include "Globals.h"

/* ! PROTOTYPES */

// enable user to type values (numbers only)
std::string TypeValue(ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font, ALLEGRO_EVENT_QUEUE *event_queue2);

// show a page with 'about' content
bool ShowAbout(ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font, ALLEGRO_BITMAP *bitmap_logo);

// start timer with set values
bool GoTimer(ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font, int time_exercise, int time_rest, int sets);

/* ! MAIN FUNCTION */

int main()
{
	srand(time(NULL));

	/* ! CHECKING IF ALLEGRO WAS INIT AND INSTALL PROPERLY */

	if (!al_init())
	{
		al_show_native_message_box(NULL,
								   "ERROR", "An error has occurred while trying to initializing Allegro 5",
								   "Contact the author ( see README.txt )", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return false;
	}
	if (!al_init_native_dialog_addon())
	{
		al_show_native_message_box(NULL,
								   "ERROR", "An error has occurred while trying to initializing native dialog addon",
								   "Contact the author ( see README.txt )", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return false;
	}
	if (!al_init_primitives_addon())
	{
		al_show_native_message_box(NULL,
								   "ERROR", "An error has occurred while trying to initializing primitives addon",
								   "Contact the author ( see README.txt )", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return false;
	}
	if (!al_init_font_addon())
	{
		al_show_native_message_box(NULL,
								   "ERROR", "An error has occurred while trying to initializing font addon",
								   "Contact the author ( see README.txt )", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return false;
	}
	if (!al_init_ttf_addon())
	{
		al_show_native_message_box(NULL,
								   "ERROR", "An error has occurred while trying to initializing ttf addon",
								   "Contact the author ( see README.txt )", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return false;
	}
	if (!al_install_audio())
	{
		al_show_native_message_box(NULL,
								   "ERROR", "An error has occurred while trying to installing audio",
								   "Contact the author ( see README.txt )", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return false;
	}
	if (!al_init_acodec_addon())
	{
		al_show_native_message_box(NULL,
								   "ERROR", "An error has occurred while trying to initializing acodec addon",
								   "Contact the author ( see README.txt )", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return false;
	}
	if (!al_install_mouse())
	{
		al_show_native_message_box(NULL,
								   "ERROR", "An error has occurred while trying to installing mouse",
								   "Contact the author ( see README.txt )", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return false;
	}
	if (!al_init_image_addon())
	{
		al_show_native_message_box(NULL,
								   "ERROR", "An error has occurred while trying to initializing image addon",
								   "Contact the author ( see README.txt )", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return false;
	}
	if (!al_install_keyboard())
	{
		al_show_native_message_box(NULL,
								   "ERROR", "An error has occurred while trying to installing keyboard",
								   "Contact the author ( see README.txt )", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return false;
	}

	/* ! CREATING DISPLAY */

	// get info about screen width, height and default display positions
	ALLEGRO_MONITOR_INFO info;
	al_get_monitor_info(0, &info);

	ALLEGRO_DISPLAY *display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	if (!display)
	{
		al_show_native_message_box(NULL,
								   "ERROR", "An error has occurred while trying to create a display",
								   "Contact the author ( see README.txt )", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return false;
	}
	else
	{
		al_set_window_position(display, info.x2 / 2 - DISPLAY_WIDTH / 2, info.y2 / 2 - DISPLAY_HEIGHT / 2);
	}

	/* ! CREATING THE BASE OF THE APP */

	ALLEGRO_TIMER *timer = al_create_timer(TIMER_SPEED);

	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	ALLEGRO_EVENT_QUEUE *event_queue_key = al_create_event_queue();

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_mouse_event_source());

	al_register_event_source(event_queue_key, al_get_display_event_source(display));
	al_register_event_source(event_queue_key, al_get_keyboard_event_source());

	/* ! LOADING RESOURCES */

	ALLEGRO_FONT *font_code_pro_32 = al_load_font(FONT_CODE_PRO, 32, NULL);
	ALLEGRO_FONT *font_code_pro_16 = al_load_font(FONT_CODE_PRO, 16, NULL);

	ALLEGRO_BITMAP *bitmap_PAULINEK_logo = al_load_bitmap(BITMAP_PAULINEK_LOGO);
	ALLEGRO_BITMAP *bitmap_timer_logo = al_load_bitmap(BITMAP_TIMER_LOGO);

	ALLEGRO_SAMPLE *sample_mouse_down = al_load_sample(SAMPLE_MOUSE_DOWN);
	ALLEGRO_SAMPLE *sample_intro = al_load_sample(SAMPLE_INTRO);
	ALLEGRO_SAMPLE *sample_main = al_load_sample(SAMPLE_MAIN);
	ALLEGRO_SAMPLE *sample_tick = al_load_sample(SAMPLE_TICK);

	ALLEGRO_SAMPLE_INSTANCE *sample_instance_main = al_create_sample_instance(sample_main);
	ALLEGRO_SAMPLE_INSTANCE *sample_instance_tick = al_create_sample_instance(sample_tick);

	/* ! IMPORTANT */

	al_reserve_samples(10);

	al_attach_sample_instance_to_mixer(sample_instance_main, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(sample_instance_tick, al_get_default_mixer());

	al_set_sample_instance_playmode(sample_instance_main, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_playmode(sample_instance_tick, ALLEGRO_PLAYMODE_LOOP);

	al_hide_mouse_cursor(display);

	al_set_window_title(display, APP_NAME);
	al_set_display_icon(display, bitmap_timer_logo);
	al_set_app_name(APP_NAME);
	al_set_org_name(ORG_NAME);

	/* ! NECESSARY VARIABLES */

	bool END_INTRO = false;
	bool END_MAIN = false;

	unsigned mseconds = 0;

	float volume = 1.0;

	MENU menu_behave = MENU_DEF;

	std::string typed_value;

	/* ! CREATING OBJECTS */

	MyMouse my_mouse = MyMouse(sample_mouse_down, COLOR_WHITE, COLOR_BLACK);

	MyBitmap my_bitmap_timer_logo = MyBitmap(bitmap_timer_logo);
	my_bitmap_timer_logo.SetIntro();
	MyBitmap my_bitmap_PAULINEK_logo = MyBitmap(bitmap_PAULINEK_logo);

	MyText my_text_intro_click = MyText(font_code_pro_32, COLOR_BLACK, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT * 0.75, "click to continue", 5);
	my_text_intro_click.SetRGBA(0, 0, 0, 255);
	MyText my_text_intro_timer = MyText(font_code_pro_32, COLOR_BLACK, 0, 0, std::string(APP_NAME), 0);

	ButtonRectangle button_volume = ButtonRectangle("music", 10, 40, 20, DISPLAY_WIDTH * 0.05, DISPLAY_HEIGHT * 0.05, 80, 40,
													COLOR_SUNGLOW, COLOR_BLACK, font_code_pro_16, RECTANGLE_FILLED);
	ButtonRectangle button_volume_up = ButtonRectangle("+", 10, 20, 20, DISPLAY_WIDTH * 0.3, DISPLAY_HEIGHT * 0.05, 40, 40,
													   COLOR_MINT, COLOR_BLACK, font_code_pro_16, RECTANGLE_FILLED);
	ButtonRectangle button_volume_down = ButtonRectangle("-", 10, 20, 20, DISPLAY_WIDTH * 0.4, DISPLAY_HEIGHT * 0.05, 40, 40,
														 COLOR_MANGO_TANGO, COLOR_BLACK, font_code_pro_16, RECTANGLE_FILLED);
	ButtonRectangle button_about = ButtonRectangle("about", 10, 10, 10, DISPLAY_WIDTH * 0.8, DISPLAY_HEIGHT * 0.05, 80, 40,
												   COLOR_DARK_CYAN, COLOR_BLACK, font_code_pro_16, RECTANGLE_FILLED);

	TimerMenu timer_menu = TimerMenu(COLOR_MANGO_TANGO, COLOR_BLACK,
									 COLOR_MINT, COLOR_BLACK,
									 COLOR_SUNGLOW, COLOR_BLACK,
									 font_code_pro_16);

	/* ! LETS GO WITH INTRO */

	al_play_sample(sample_intro, 1.0, 1.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

	al_start_timer(timer);

	while (!END_INTRO)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			END_INTRO = true;
			END_MAIN = true;
		}
		if (event.type == ALLEGRO_EVENT_TIMER)
		{
			mseconds++;

			my_text_intro_click.EventTimerBlick();
			my_text_intro_click.SetDraw(mseconds > 180);
			my_text_intro_timer.EventTimerMove(mseconds > 80);

			my_bitmap_timer_logo.TimerIntro();
			my_bitmap_PAULINEK_logo.TimerIntro_2(mseconds > 180);

			my_mouse.EventTimer(mseconds);
		}
		if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			my_mouse.EventMouseAxes(event.mouse.x, event.mouse.y);
		}
		else
		{
			my_mouse.EventMouseAxesElse(event.mouse.x, event.mouse.y);
		}
		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			my_mouse.EventMouseButtonDown();

			if (mseconds > 180)
			{
				END_INTRO = true;
				END_MAIN = false;
			}
		}

		al_clear_to_color(COLOR_PEACH);

		my_bitmap_timer_logo.DrawIntro();
		my_bitmap_PAULINEK_logo.DrawIntro();

		my_text_intro_click.DrawText();
		my_text_intro_timer.DrawText();

		my_mouse.DrawMouseLastPositions();
		my_mouse.DrawMouseCurrPositions();

		al_flip_display();
	}

	/* ! LETS GO WITH MAIN APP */

	mseconds = 0;

	while (!END_MAIN)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);

		al_flush_event_queue(event_queue_key);

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			END_MAIN = true;
		}
		if (event.type == ALLEGRO_EVENT_TIMER)
		{
			mseconds++;

			my_mouse.EventTimer(mseconds);
		}
		if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			my_mouse.EventMouseAxes(event.mouse.x, event.mouse.y);

			button_volume.CheckIfTouched(event.mouse.x, event.mouse.y);
			button_volume_up.CheckIfTouched(event.mouse.x, event.mouse.y);
			button_volume_down.CheckIfTouched(event.mouse.x, event.mouse.y);
			button_about.CheckIfTouched(event.mouse.x, event.mouse.y);

			timer_menu.EventMouseAxes(event.mouse.x, event.mouse.y);
		}
		else
		{
			my_mouse.EventMouseAxesElse(event.mouse.x, event.mouse.y);
		}
		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			my_mouse.EventMouseButtonDown();

			if (button_volume.GetIfTouched())
			{
				button_volume.ChangeClicked();
				switch (button_volume.GetIfClicked())
				{
				case true:
					al_set_sample_instance_gain(sample_instance_main, 0);
					break;
				case false:
					al_set_sample_instance_gain(sample_instance_main, volume);
					break;
				}
			}
			switch (button_volume.GetIfClicked())
			{
			case true:
				break;
			case false:
				if (button_volume_up.GetIfTouched())
				{
					if (volume < 3)
						volume += 0.2;
					al_set_sample_instance_gain(sample_instance_main, volume);
				}
				else if (button_volume_down.GetIfTouched())
				{
					if (volume > 0.2)
						volume -= 0.2;
					al_set_sample_instance_gain(sample_instance_main, volume);
				}
				break;
			}
			if (button_about.GetIfTouched())
			{
				END_MAIN = ShowAbout(event_queue, font_code_pro_16, bitmap_PAULINEK_logo);
			}

			menu_behave = timer_menu.EventMouseDown();

			switch (menu_behave)
			{
			case MENU_DEF:
				break;
			case MENU_START:
				if (timer_menu.GetSets() != 0 && (timer_menu.GetExercise() != 0 || timer_menu.GetRest() != 0))
					GoTimer(event_queue, font_code_pro_32, timer_menu.GetExercise(), timer_menu.GetRest(), timer_menu.GetSets());
				break;
			case MENU_CHANGE_EXERCISE:
				typed_value = TypeValue(event_queue_key, font_code_pro_32, event_queue);
				timer_menu.ChangeExercise(stoi(typed_value));
				break;
			case MENU_CHANGE_REST:
				typed_value = TypeValue(event_queue_key, font_code_pro_32, event_queue);
				timer_menu.ChangeRest(stoi(typed_value));
				break;
			case MENU_CHANGE_SETS:
				typed_value = TypeValue(event_queue_key, font_code_pro_32, event_queue);
				timer_menu.ChangeSets(stoi(typed_value));
				break;
			default:
				break;
			}
		}

		al_set_sample_instance_playing(sample_instance_main, true);

		al_clear_to_color(COLOR_CHARCOAL);

		button_volume.Draw();
		button_volume_up.Draw();
		button_volume_down.Draw();
		button_about.Draw();

		timer_menu.Draw(COLOR_DARK_CYAN, COLOR_CHARCOAL);

		my_mouse.DrawMouseLastPositions();
		my_mouse.DrawMouseCurrPositions();

		al_flip_display();
	}

	/* ! DESTROYING */

	al_destroy_font(font_code_pro_16);
	al_destroy_font(font_code_pro_32);

	al_destroy_sample_instance(sample_instance_main);
	al_destroy_sample_instance(sample_instance_tick);

	al_destroy_sample(sample_tick);
	al_destroy_sample(sample_main);
	al_destroy_sample(sample_intro);
	al_destroy_sample(sample_mouse_down);

	al_destroy_bitmap(bitmap_timer_logo);
	al_destroy_bitmap(bitmap_PAULINEK_logo);

	al_destroy_timer(timer);

	al_destroy_event_queue(event_queue_key);
	al_destroy_event_queue(event_queue);

	al_destroy_display(display);

	return 0;
}

/* ! DEFINITIONS OF FUNCTIONS */

std::string TypeValue(ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font, ALLEGRO_EVENT_QUEUE *event_queue2)
{
	/* ! VARIABLES */

	bool end = false;

	int value = 0;

	std::string str_value;

	/* ! START TYPING */

	while (!end)
	{
		al_clear_to_color(COLOR_BLACK);

		al_draw_textf(font, COLOR_WHITE, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "type value: %s", str_value.c_str());

		al_flip_display();

		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);

		// do not allow that event queue to have any impact now
		al_flush_event_queue(event_queue2);

		if (event.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (event.keyboard.keycode)
			{
			case ALLEGRO_KEY_0:
				str_value.append("0");
				break;
			case ALLEGRO_KEY_1:
				str_value.append("1");
				break;
			case ALLEGRO_KEY_2:
				str_value.append("2");
				break;
			case ALLEGRO_KEY_3:
				str_value.append("3");
				break;
			case ALLEGRO_KEY_4:
				str_value.append("4");
				break;
			case ALLEGRO_KEY_5:
				str_value.append("5");
				break;
			case ALLEGRO_KEY_6:
				str_value.append("6");
				break;
			case ALLEGRO_KEY_7:
				str_value.append("7");
				break;
			case ALLEGRO_KEY_8:
				str_value.append("8");
				break;
			case ALLEGRO_KEY_9:
				str_value.append("9");
				break;
			case ALLEGRO_KEY_BACKSPACE:
				str_value.pop_back();
				break;
			case ALLEGRO_KEY_ENTER:
				end = true;
				break;
			default:
				break;
			}
		}

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			end = true;
		}
	}

	if (str_value.size() == 0)
	{
		str_value.append("0");
	}

	return str_value;
}

bool ShowAbout(ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font, ALLEGRO_BITMAP *bitmap_logo)
{
	/* ! NECESSARY THINGS */

	ALLEGRO_EVENT_QUEUE *tem_event_queue = al_create_event_queue();

	al_register_event_source(tem_event_queue, al_get_display_event_source(al_get_current_display()));
	al_register_event_source(tem_event_queue, al_get_mouse_event_source());

	al_show_mouse_cursor(al_get_current_display());

	/* ! VARIABLES */

	bool end = false;

	// if true: app should be closed
	bool if_close = false;

	/* ! START SHOWING */

	while (!end)
	{
		al_clear_to_color(COLOR_BLACK);

		al_draw_text(font, COLOR_SUNGLOW, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT * 0.1, ALLEGRO_ALIGN_CENTRE, APP_NAME);
		al_draw_text(font, COLOR_SUNGLOW, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT * 0.2, ALLEGRO_ALIGN_CENTRE, "by PAULINEK");
		al_draw_text(font, COLOR_SUNGLOW, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT * 0.3, ALLEGRO_ALIGN_CENTRE, "This is a simple interval timer.");
		al_draw_text(font, COLOR_SUNGLOW, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT * 0.4, ALLEGRO_ALIGN_CENTRE, "I hope you'll enjoy it!");
		al_draw_text(font, COLOR_SUNGLOW, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT * 0.5, ALLEGRO_ALIGN_CENTRE, "PAULINEK 31st January 2020");
		al_draw_text(font, COLOR_SUNGLOW, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT * 0.8, ALLEGRO_ALIGN_CENTRE, "Sound effects obtained from www.zapsplat.com");
		al_draw_text(font, COLOR_SUNGLOW, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT * 0.9, ALLEGRO_ALIGN_CENTRE, "Icon made by Gregor Cresnar from www.flaticon.com");

		al_draw_bitmap(bitmap_logo, DISPLAY_WIDTH / 2 - al_get_bitmap_width(bitmap_logo) / 2, DISPLAY_HEIGHT * 0.6, 0);

		al_flip_display();

		ALLEGRO_EVENT event;
		al_wait_for_event(tem_event_queue, &event);

		// do not allow that event queue to have any impact now
		al_flush_event_queue(event_queue);

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			if_close = true;
			end = true;
		}

		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			end = true;
		}
	}

	al_destroy_event_queue(tem_event_queue);

	al_hide_mouse_cursor(al_get_current_display());

	return if_close;
}

bool GoTimer(ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font, int time_exercise, int time_rest, int sets)
{
	/* ! NECESSARY THINGS */

	TIMER_STATE state = COUNTING;

	al_show_mouse_cursor(al_get_current_display());

	ALLEGRO_EVENT_QUEUE *event_queue_timer = al_create_event_queue();

	ALLEGRO_TIMER *timer = al_create_timer(1 / 60.0);

	ALLEGRO_SAMPLE *s0 = al_load_sample(SAMPLE_LETS_GO);
	ALLEGRO_SAMPLE *s1 = al_load_sample(SAMPLE_1_ONE);
	ALLEGRO_SAMPLE *s2 = al_load_sample(SAMPLE_2_TWO);
	ALLEGRO_SAMPLE *s3 = al_load_sample(SAMPLE_3_THREE);
	ALLEGRO_SAMPLE *win = al_load_sample(SAMPLE_YOU_WIN);
	ALLEGRO_SAMPLE *applause = al_load_sample(SAMPLE_APPLAUSE);
	ALLEGRO_SAMPLE *clunk = al_load_sample(SAMPLE_CLUNK);

	ButtonRectangle button_menu = ButtonRectangle("menu", 10, 10, 10, DISPLAY_WIDTH / 4 - 100, DISPLAY_HEIGHT * 0.1, 200, 50, COLOR_CHARCOAL, COLOR_BLACK, font, RECTANGLE_FILLED);
	ButtonRectangle button_restart = ButtonRectangle("restart", 10, 10, 10, DISPLAY_WIDTH * 0.75 - 100, DISPLAY_HEIGHT * 0.1, 200, 50, COLOR_DARK_CYAN, COLOR_BLACK, font, RECTANGLE_FILLED);

	al_register_event_source(event_queue_timer, al_get_display_event_source(al_get_current_display()));
	al_register_event_source(event_queue_timer, al_get_timer_event_source(timer));
	al_register_event_source(event_queue_timer, al_get_mouse_event_source());

	/* ! VARIABLES */

	// if true: app should be closed
	bool end = false;

	// whether workout is finished
	bool finished = false;

	// if can play the sample
	bool play[] = {true, true, true, true, true, true, true};

	bool change_play = true;

	double start_time = al_get_time();
	double curr_time = start_time;

	// total difference between current time and time when timer has started
	double diff_time = start_time - curr_time;

	// 3...2...1...0...GO!
	double counting = 3;

	// how many seconds is completed from exercise time
	int done_time_exercise = 0;

	// how many seconds is completed from rest time
	int done_time_rest = 0;

	int done_exercises = 0;
	int done_rests = 0;
	int done_sets = 0;

	/* ! START TIMER */

	al_start_timer(timer);

	while (!finished)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue_timer, &event);

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			end = true;
			finished = true;
		}

		/* ! TIMER SECTION */

		if (event.type == ALLEGRO_EVENT_TIMER)
		{
			// change time variables
			curr_time = al_get_time();
			diff_time = curr_time - start_time;

			switch (state)
			{
			case COUNTING:
				break;
			case EXERCISE:
				done_time_exercise = diff_time - 3 - (time_exercise * done_exercises) - (time_rest * done_rests);
				if (done_time_exercise > time_exercise)
				{
					done_time_exercise = 0;
					done_exercises++;
					change_play = true;
					if (done_exercises != sets)
						state = REST;
					else
						state = FINISHED;
				}
				break;
			case REST:
				done_time_rest = diff_time - 3 - (time_exercise * done_exercises) - (time_rest * done_rests);
				if (done_time_rest > time_rest)
				{
					done_time_rest = 0;
					done_rests++;
					done_sets++;
					if (done_sets == sets)
						state = FINISHED;
					else
					{
						change_play = true;
						state = EXERCISE;
					}
				}
				break;
			case FINISHED:
				break;
			default:
				break;
			}
		}

		/* ! MOUSE SECTION */

		if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			button_menu.CheckIfTouched(event.mouse.x, event.mouse.y);
			button_restart.CheckIfTouched(event.mouse.x, event.mouse.y);
		}
		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (button_menu.GetIfTouched())
			{
				end = false;
				finished = true;
			}
			else if (button_restart.GetIfTouched())
			{
				for (bool &x : play)
					x = true;
				change_play = true;

				start_time = al_get_time();
				curr_time = start_time;
				diff_time = start_time - curr_time;
				counting = 3;

				done_time_exercise = 0;
				done_time_rest = 0;

				done_exercises = 0;
				done_rests = 0;
				done_sets = 0;

				al_stop_samples();

				state = COUNTING;
			}
		}

		/* ! DRAW SECTION */

		switch (state)
		{
		case COUNTING:
			al_clear_to_color(COLOR_SUNGLOW);

			al_draw_textf(font, COLOR_BLACK, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 4, ALLEGRO_ALIGN_CENTRE, "total time: %i seconds", (int)diff_time);
			al_draw_textf(font, COLOR_BLACK, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "%i", (int)counting);

			button_menu.Draw();
			button_restart.Draw();

			al_flip_display();
			break;
		case EXERCISE:
			al_clear_to_color(COLOR_MANGO_TANGO);

			al_draw_textf(font, COLOR_BLACK, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 4, ALLEGRO_ALIGN_CENTRE, "total time: %i seconds", (int)diff_time);
			al_draw_textf(font, COLOR_BLACK, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "%i", (int)done_time_exercise);

			button_menu.Draw();
			button_restart.Draw();

			al_flip_display();
			break;
		case REST:
			al_clear_to_color(COLOR_MINT);

			al_draw_textf(font, COLOR_BLACK, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 4, ALLEGRO_ALIGN_CENTRE, "total time: %i seconds", (int)diff_time);
			al_draw_textf(font, COLOR_BLACK, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "%i", (int)done_time_rest);

			button_menu.Draw();
			button_restart.Draw();

			al_flip_display();
			break;
		case FINISHED:
			al_clear_to_color(COLOR_CORNSILK);

			al_draw_textf(font, COLOR_BLACK, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 4, ALLEGRO_ALIGN_CENTRE, "total time: %i seconds", (int)diff_time);
			al_draw_text(font, COLOR_BLACK, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "Congratulations!");

			button_menu.Draw();
			button_restart.Draw();

			al_flip_display();
			break;
		default:
			break;
		}

		/* ! SOUND SECTION */

		switch (state)
		{
		case COUNTING:
			if (diff_time >= 0 && diff_time < 1)
			{
				counting = 3;
				if (play[3])
				{
					al_play_sample(s3, 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
					play[3] = false;
				}
			}
			else if (diff_time >= 1 && diff_time < 2)
			{
				counting = 2;
				if (play[2])
				{
					al_play_sample(s2, 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
					play[2] = false;
				}
			}
			else if (diff_time >= 2 && diff_time < 3)
			{
				counting = 1;
				if (play[1])
				{
					al_play_sample(s1, 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
					play[1] = false;
				}
			}
			else if (diff_time >= 3 && diff_time < 4)
			{
				counting = 0;
				if (play[0])
				{
					al_play_sample(s0, 3, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
					play[0] = false;
				}
			}
			else
				state = EXERCISE;
			break;
		case EXERCISE:
			if (change_play)
			{
				for (bool &x : play)
					x = true;
				change_play = false;
			}
			if (time_rest != 0)
			{
				if (time_exercise > 3)
				{
					if (done_time_exercise >= time_exercise - 3 && play[3])
					{
						al_play_sample(s3, 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
						play[3] = false;
					}
					else if (done_time_exercise >= time_exercise - 2 && play[2])
					{
						al_play_sample(s2, 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
						play[2] = false;
					}
					else if (done_time_exercise >= time_exercise - 1 && play[1])
					{
						al_play_sample(s1, 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
						play[1] = false;
					}
					else if (done_time_exercise >= time_exercise - 0 && play[6])
					{
						al_play_sample(clunk, 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
						play[6] = false;
					}
				}
				else
				{
					if (done_time_exercise >= time_exercise - 0 && play[6])
					{
						al_play_sample(clunk, 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
						play[6] = false;
					}
				}
			}
			else
			{
				if (time_exercise > 3)
				{
					if (done_time_exercise >= time_exercise - 3 && play[3])
					{
						al_play_sample(s3, 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
						play[3] = false;
					}
					else if (done_time_exercise >= time_exercise - 2 && play[2])
					{
						al_play_sample(s2, 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
						play[2] = false;
					}
					else if (done_time_exercise >= time_exercise - 1 && play[1])
					{
						al_play_sample(s1, 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
						play[1] = false;
					}
					else if (done_time_exercise >= time_exercise - 0 && play[0])
					{
						al_play_sample(s0, 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
						play[0] = false;
					}
				}
				else
				{
					if (done_time_exercise >= time_exercise - 0 && play[0])
					{
						al_play_sample(s0, 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
						play[0] = false;
					}
				}
			}
			break;
		case REST:
			if (change_play)
			{
				for (bool &x : play)
					x = true;
				change_play = false;
			}
			if (time_rest != 0)
			{
				if (time_rest > 3)
				{
					if (done_time_rest >= time_rest - 3 && play[3])
					{
						al_play_sample(s3, 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
						play[3] = false;
					}
					else if (done_time_rest >= time_rest - 2 && play[2])
					{
						al_play_sample(s2, 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
						play[2] = false;
					}
					else if (done_time_rest >= time_rest - 1 && play[1])
					{
						al_play_sample(s1, 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
						play[1] = false;
					}
					else if (done_time_rest >= time_rest - 0 && play[0])
					{
						al_play_sample(s0, 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
						play[0] = false;
					}
				}
				else
				{
					if (done_time_rest >= time_rest - 0 && play[0])
					{
						al_play_sample(s0, 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
						play[0] = false;
					}
				}
			}
			break;
		case FINISHED:
			if (play[4])
			{
				al_play_sample(win, 5, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
				play[4] = false;
			}
			if (play[5])
			{
				al_play_sample(applause, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
				play[5] = false;
			}
			break;
		default:
			break;
		}

		// do not allow that event queue to have any impact now
		al_flush_event_queue(event_queue);
	}

	al_stop_timer(timer);

	button_menu.~ButtonRectangle();
	button_restart.~ButtonRectangle();

	al_destroy_sample(s0);
	al_destroy_sample(s1);
	al_destroy_sample(s2);
	al_destroy_sample(s3);
	al_destroy_sample(win);
	al_destroy_sample(applause);
	al_destroy_sample(clunk);

	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue_timer);

	al_hide_mouse_cursor(al_get_current_display());

	return end;
}