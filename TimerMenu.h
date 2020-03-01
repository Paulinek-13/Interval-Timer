#ifndef TIMERMENU_H
#define TIMERMENU_H

class TimerMenu
{
private:

	float m_width;
	float m_height;

	float m_px;
	float m_py;
	float m_px2;
	float m_py2;

	// how many sets has the workout
	int m_sets;

	// how long takes one exercise
	int m_time_exercise;

	//how long takes one rest
	int m_time_rest;

	ButtonRectangle m_button_start;
	ButtonRectangle m_button_exercise;
	ButtonRectangle m_button_rest;
	ButtonRectangle m_button_sets;

public:

	TimerMenu(ALLEGRO_COLOR color_ut, ALLEGRO_COLOR color_t, ALLEGRO_COLOR color_ut2, ALLEGRO_COLOR color_t2, ALLEGRO_COLOR color_ut3, ALLEGRO_COLOR color_t3, ALLEGRO_FONT*font);
	~TimerMenu();

	// check if any button is touched
	void EventMouseAxes(float mx, float my);

	// to put in if condition with event type: ALLEGRO_EVENT_MOUSE_BUTTON_DOWN
	// check if any button is clicked and return which menu state it points
	MENU EventMouseDown();

	void Draw(ALLEGRO_COLOR color_main, ALLEGRO_COLOR color_secd);

	void ChangeExercise(int value);
	void ChangeRest(int value);
	void ChangeSets(int value);

	int GetExercise();
	int GetRest();
	int GetSets();

};

TimerMenu::TimerMenu(ALLEGRO_COLOR color_ut, ALLEGRO_COLOR color_t, ALLEGRO_COLOR color_ut2, ALLEGRO_COLOR color_t2, ALLEGRO_COLOR color_ut3, ALLEGRO_COLOR color_t3, ALLEGRO_FONT*font)
{
	m_px = DISPLAY_WIDTH * 0.05;
	m_py = DISPLAY_HEIGHT * 0.25;
	m_px2 = DISPLAY_WIDTH - m_px;
	m_py2 = DISPLAY_HEIGHT - m_px;

	m_width = m_px2 - m_px;
	m_height = m_py2 - m_py;

	m_sets = 1;

	m_time_exercise = 20;
	m_time_rest = 10;

	m_button_start = ButtonRectangle("START", 10, 10, 10, m_px + (m_width*0.1), m_py, (m_width*0.9) - (m_width*0.1), m_height - (m_height*0.75), color_ut3, color_t3, font, RECTANGLE_ROUNDED_FILLED);
	m_button_exercise = ButtonRectangle("exercise", 10, 10, 10, m_px + (m_width*0.1), m_py + (m_height*0.25), (m_width*0.9) - (m_width*0.1), (m_height*0.75) - (m_height*0.5), color_ut, color_t, font, RECTANGLE_ROUNDED_FILLED);
	m_button_rest = ButtonRectangle("rest", 10, 10, 10, m_px + (m_width*0.1), m_py + (m_height*0.5), (m_width*0.9) - (m_width*0.1), (m_height*0.5) - (m_height*0.25), color_ut2, color_t2, font, RECTANGLE_ROUNDED_FILLED);
	m_button_sets = ButtonRectangle("sets", 10, 10, 10, m_px + (m_width*0.1), m_py + (m_height*0.75), (m_width*0.9) - (m_width*0.1), (m_height*0.25) - (m_height*0.0), color_ut3, color_t3, font, RECTANGLE_ROUNDED_FILLED);

	m_button_exercise.ChangeText("exercise:   " + std::to_string(m_time_exercise) + " sec");
	m_button_rest.ChangeText("rest:   " + std::to_string(m_time_rest) + " sec");
	m_button_sets.ChangeText("sets:   " + std::to_string(m_sets) + " set(s)");
}
TimerMenu::~TimerMenu()
{
}

void TimerMenu::EventMouseAxes(float mx, float my)
{
	m_button_exercise.CheckIfTouched(mx, my);
	m_button_rest.CheckIfTouched(mx, my);
	m_button_sets.CheckIfTouched(mx, my);
	m_button_start.CheckIfTouched(mx, my);
}
MENU TimerMenu::EventMouseDown()
{
	if (m_button_start.GetIfTouched()) { return MENU_START; }
	else if (m_button_exercise.GetIfTouched()) { return MENU_CHANGE_EXERCISE; }
	else if (m_button_rest.GetIfTouched()) { return MENU_CHANGE_REST; }
	else if (m_button_sets.GetIfTouched()) { return MENU_CHANGE_SETS; }
	else { return MENU_DEF; }
}

void TimerMenu::Draw(ALLEGRO_COLOR color_main, ALLEGRO_COLOR color_secd)
{
	al_draw_filled_rounded_rectangle(m_px, m_py, m_px2, m_py2, 25, 25, color_main);

	m_button_exercise.Draw();
	m_button_rest.Draw();
	m_button_sets.Draw();
	m_button_start.Draw();
}

void TimerMenu::ChangeExercise(int value) { m_time_exercise = value; m_button_exercise.ChangeText("exercise:   " + std::to_string(m_time_exercise) + " sec"); }
void TimerMenu::ChangeRest(int value) { m_time_rest = value; m_button_rest.ChangeText("rest:   " + std::to_string(m_time_rest) + " sec"); }
void TimerMenu::ChangeSets(int value) { m_sets = value; m_button_sets.ChangeText("sets:   " + std::to_string(m_sets) + " set(s)"); }

int TimerMenu::GetExercise() { return m_time_exercise; }
int TimerMenu::GetRest() { return m_time_rest; }
int TimerMenu::GetSets() { return m_sets; }

#endif // !TIMER_MENU_H