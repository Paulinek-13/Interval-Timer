#ifndef MYTEXT_H
#define MYTEXT_H

class MyText
{
private:

	ALLEGRO_FONT*m_font;

	ALLEGRO_COLOR m_color;

	bool m_draw;

	// sets transparency
	bool m_increase_a;

	bool m_move_up;

	float m_px;
	float m_py;

	std::string m_text;

	// red
	unsigned char m_r;

	// green
	unsigned char m_g;

	// blue
	unsigned char m_b;

	// alpha
	unsigned char m_a;

	unsigned char m_blick_speed;

public:

	MyText(ALLEGRO_FONT*font, ALLEGRO_COLOR color, float px, float py, std::string text, unsigned char blick_speed);
	~MyText();

	void SetDraw(bool tORf);
	void SetRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	// change alpha 0-255
	void EventTimerBlick();

	// to put in if condition with event type: ALLEGRO_EVENT_TIMER
	// move the text
	void EventTimerMove(bool when_start);

	void DrawText();

	float GetPosX();
	float GetPosY();

	bool GetIfMoveUp();

};

MyText::MyText(ALLEGRO_FONT*font, ALLEGRO_COLOR color, float px, float py, std::string text, unsigned char blick_speed)
{
	m_font = font;

	m_color = color;

	m_px = px;
	m_py = py;

	m_text = text;

	m_draw = false;
	m_increase_a = false;
	m_move_up = true;

	m_blick_speed = blick_speed;
}
MyText::~MyText()
{
}

void MyText::SetDraw(bool tORf) { m_draw = tORf; }
void MyText::SetRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { m_r = r; m_g = g; m_b = b; m_a = a; m_color = al_map_rgba(m_r, m_g, m_b, m_a); }

void MyText::EventTimerBlick()
{
	switch (m_increase_a)
	{
	case true:
		m_a += m_blick_speed;
		if (m_a >= 255)m_increase_a = false;
		break;
	case false:
		m_a -= m_blick_speed;
		if (m_a <= 0)m_increase_a = true;
		break;
	}
	m_color = al_map_rgba(m_r, m_g, m_b, m_a);
}

void MyText::EventTimerMove(bool when_start)
{
	if (when_start)
	{
		m_draw = true;

		switch (m_move_up)
		{
		case true:
			m_px += 3;
			m_py = m_px;
			if (m_py >= DISPLAY_HEIGHT / 2)m_move_up = false;
			break;
		case false:
			if (m_px > DISPLAY_WIDTH*0.3)m_px -= 2;
			break;
		}
	}
}

void MyText::DrawText() { if (m_draw)al_draw_textf(m_font, m_color, m_px, m_py, ALLEGRO_ALIGN_CENTRE, "%s", m_text.c_str()); }

float MyText::GetPosX() { return m_px; }
float MyText::GetPosY() { return m_py; }

bool MyText::GetIfMoveUp() { return m_move_up; }

#endif // !MYTEXT_H