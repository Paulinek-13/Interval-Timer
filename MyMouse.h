#ifndef MYMOUSE_H
#define MYMOUSE_H

class MyMouse
{
private:

	ALLEGRO_SAMPLE*m_sample_button_down;

	ALLEGRO_COLOR m_color_last_pos;
	ALLEGRO_COLOR m_color_curr_pos;

	float m_curr_px;
	float m_curr_py;

	// positions x and y of the 20 last positions of a mouse
	float m_last_positions[20][2];

	// from 0 to 19 (so 20 numbers)
	unsigned m_counter;

	// from 0 to 5
	unsigned m_msec_counter;

public:

	MyMouse(ALLEGRO_SAMPLE*sample_button_down, ALLEGRO_COLOR color_last_pos, ALLEGRO_COLOR color_curr_pos);
	~MyMouse();

	// to put in if condition with event type: ALLEGRO_EVENT_TIMER
	// increase the counter
	void EventTimer(unsigned msec);

	// save mouse's current positions
	void EventMouseAxes(float mx, float my);

	// check if mouse does not move too long and if true - reset the last positions
	void EventMouseAxesElse(float mx, float my);

	// play the sound
	void EventMouseButtonDown();

	void DrawMouseLastPositions();
	void DrawMouseCurrPositions();

};

MyMouse::MyMouse(ALLEGRO_SAMPLE*sample_button_down, ALLEGRO_COLOR color_last_pos, ALLEGRO_COLOR color_curr_pos)
{
	m_sample_button_down = sample_button_down;

	m_color_last_pos = color_last_pos;
	m_color_curr_pos = color_curr_pos;

	m_curr_px = -100;
	m_curr_py = -100;
}
MyMouse::~MyMouse()
{
}

void MyMouse::EventTimer(unsigned msec)
{
	m_counter = msec % 20;
}

void MyMouse::EventMouseAxes(float mx, float my)
{
	m_msec_counter = 0;
	m_last_positions[m_counter][0] = mx;
	m_last_positions[m_counter][1] = my;
	m_curr_px = mx;
	m_curr_py = my;
}

void MyMouse::EventMouseAxesElse(float mx, float my)
{
	m_msec_counter++;
	if (m_msec_counter > 5)
	{
		for (int i = 0; i < 20; i++)
		{
			m_last_positions[i][0] = mx;
			m_last_positions[i][1] = my;
		}
	}
}

void MyMouse::EventMouseButtonDown()
{
	al_play_sample(m_sample_button_down, 5.0, 1.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
}

void MyMouse::DrawMouseLastPositions()
{
	for (int i = 0; i < 20; i++)
	{
		al_draw_filled_circle(m_last_positions[i][0], m_last_positions[i][1], 7, m_color_last_pos);
	}
}
void MyMouse::DrawMouseCurrPositions()
{
	al_draw_filled_circle(m_curr_px, m_curr_py, 10, m_color_curr_pos);
}

#endif // !MYMOUSE_H