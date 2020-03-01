#ifndef BUTTONRECTANGLE_H
#define BUTTONRECTANGLE_H

class ButtonRectangle
{
private:

	int m_index;
	int m_font_height;
	int m_thickness;
	int m_rx;
	int m_ry;

	float m_pos_x;
	float m_pos_y;
	float m_width;
	float m_height;
	float m_pos_x2;
	float m_pos_y2;
	float m_pos_xc;
	float m_pos_yc;

	bool m_touched;
	bool m_clicked;

	std::string m_text;

	ALLEGRO_COLOR m_color_untouched;
	ALLEGRO_COLOR m_color_touched;
	ALLEGRO_COLOR m_color_clicked;
	ALLEGRO_COLOR m_color_clicked_t;

	ALLEGRO_FONT*m_font;

	BUTTON_TYPE m_type;

public:

	ButtonRectangle();
	ButtonRectangle(std::string text, int thickness, int rx, int ry, float px, float py, float w, float h, ALLEGRO_COLOR color_ut, ALLEGRO_COLOR color_t, ALLEGRO_FONT*font, BUTTON_TYPE type);
	~ButtonRectangle();

	// check if mouse cursor touches the button
	bool CheckIfTouched(float mx, float my);

	bool ChangeClicked();

	bool GetIfTouched();
	bool GetIfClicked();

	void Draw();

	// change text to draw
	void ChangeText(std::string text);

};

ButtonRectangle::ButtonRectangle()
{
}
ButtonRectangle::ButtonRectangle(std::string text, int thickness, int rx, int ry, float px, float py, float w, float h, ALLEGRO_COLOR color_ut, ALLEGRO_COLOR color_t, ALLEGRO_FONT*font, BUTTON_TYPE type)
{
	m_text = text;
	m_font_height = al_get_font_line_height(font);
	m_thickness = thickness;
	m_rx = rx;
	m_ry = ry;

	m_pos_x = px;
	m_pos_y = py;
	m_width = w;
	m_height = h;
	m_pos_x2 = m_pos_x + m_width;
	m_pos_y2 = m_pos_y + m_height;
	m_pos_xc = m_pos_x + m_width / 2.0;
	m_pos_yc = m_pos_y + m_height / 2.0;

	m_touched = false;
	m_clicked = false;

	m_color_untouched = color_ut;
	m_color_touched = color_t;
	m_color_clicked = al_map_rgb(100, 100, 100);
	m_color_clicked_t = al_map_rgb(0, 0, 0);

	m_font = font;

	m_type = type;
}
ButtonRectangle::~ButtonRectangle()
{
}

bool ButtonRectangle::CheckIfTouched(float mx, float my)
{
	if (mx > m_pos_x && mx < m_pos_x2 && my > m_pos_y && my < m_pos_y2)
	{
		m_touched = true;
		return true;
	}
	else
	{
		m_touched = false;
		return false;
	}
}

bool ButtonRectangle::ChangeClicked() { m_clicked = !m_clicked; return m_clicked; }

bool ButtonRectangle::GetIfTouched() { return m_touched; }
bool ButtonRectangle::GetIfClicked() { return m_clicked; }

void ButtonRectangle::Draw()
{
	switch (m_clicked)
	{
	case true:
		switch (m_type)
		{
		case RECTANGLE:
			switch (m_touched)
			{
			case true:
				al_draw_rectangle(m_pos_x, m_pos_y, m_pos_x2, m_pos_y2, m_color_clicked_t, m_thickness);
				al_draw_textf(m_font, m_color_clicked, m_pos_xc, m_pos_yc - m_font_height / 2, ALLEGRO_ALIGN_CENTRE, "%s", m_text.c_str());
				break;
			case false:
				al_draw_rectangle(m_pos_x, m_pos_y, m_pos_x2, m_pos_y2, m_color_clicked, m_thickness);
				al_draw_textf(m_font, m_color_clicked_t, m_pos_xc, m_pos_yc - m_font_height / 2, ALLEGRO_ALIGN_CENTRE, "%s", m_text.c_str());
				break;
			}
			break;
		case RECTANGLE_FILLED:
			switch (m_touched)
			{
			case true:
				al_draw_filled_rectangle(m_pos_x, m_pos_y, m_pos_x2, m_pos_y2, m_color_clicked_t);
				al_draw_textf(m_font, m_color_clicked, m_pos_xc, m_pos_yc - m_font_height / 2, ALLEGRO_ALIGN_CENTRE, "%s", m_text.c_str());
				break;
			case false:
				al_draw_filled_rectangle(m_pos_x, m_pos_y, m_pos_x2, m_pos_y2, m_color_clicked);
				al_draw_textf(m_font, m_color_clicked_t, m_pos_xc, m_pos_yc - m_font_height / 2, ALLEGRO_ALIGN_CENTRE, "%s", m_text.c_str());
				break;
			}
			break;
		case RECTANGLE_ROUNDED:
			switch (m_touched)
			{
			case true:
				al_draw_rounded_rectangle(m_pos_x, m_pos_y, m_pos_x2, m_pos_y2, m_rx, m_ry, m_color_clicked_t, m_thickness);
				al_draw_textf(m_font, m_color_clicked, m_pos_xc, m_pos_yc - m_font_height / 2, ALLEGRO_ALIGN_CENTRE, "%s", m_text.c_str());
				break;
			case false:
				al_draw_rounded_rectangle(m_pos_x, m_pos_y, m_pos_x2, m_pos_y2, m_rx, m_ry, m_color_clicked, m_thickness);
				al_draw_textf(m_font, m_color_clicked_t, m_pos_xc, m_pos_yc - m_font_height / 2, ALLEGRO_ALIGN_CENTRE, "%s", m_text.c_str());
				break;
			}
			break;
		case RECTANGLE_ROUNDED_FILLED:
			switch (m_touched)
			{
			case true:
				al_draw_filled_rounded_rectangle(m_pos_x, m_pos_y, m_pos_x2, m_pos_y2, m_rx, m_ry, m_color_clicked_t);
				al_draw_textf(m_font, m_color_clicked, m_pos_xc, m_pos_yc - m_font_height / 2, ALLEGRO_ALIGN_CENTRE, "%s", m_text.c_str());
				break;
			case false:
				al_draw_filled_rounded_rectangle(m_pos_x, m_pos_y, m_pos_x2, m_pos_y2, m_rx, m_ry, m_color_clicked);
				al_draw_textf(m_font, m_color_clicked_t, m_pos_xc, m_pos_yc - m_font_height / 2, ALLEGRO_ALIGN_CENTRE, "%s", m_text.c_str());
				break;
			}
			break;
		default:
			switch (m_touched)
			{
			case true:
				al_draw_filled_rectangle(m_pos_x, m_pos_y, m_pos_x2, m_pos_y2, m_color_clicked_t);
				al_draw_textf(m_font, m_color_clicked, m_pos_xc, m_pos_yc - m_font_height / 2, ALLEGRO_ALIGN_CENTRE, "%s", m_text.c_str());
				break;
			case false:
				al_draw_filled_rectangle(m_pos_x, m_pos_y, m_pos_x2, m_pos_y2, m_color_clicked);
				al_draw_textf(m_font, m_color_clicked_t, m_pos_xc, m_pos_yc - m_font_height / 2, ALLEGRO_ALIGN_CENTRE, "%s", m_text.c_str());
				break;
			}
			break;
		}
		break;
	case false:
		switch (m_type)
		{
		case RECTANGLE:
			switch (m_touched)
			{
			case true:
				al_draw_rectangle(m_pos_x, m_pos_y, m_pos_x2, m_pos_y2, m_color_touched, m_thickness);
				al_draw_textf(m_font, m_color_untouched, m_pos_xc, m_pos_yc - m_font_height / 2, ALLEGRO_ALIGN_CENTRE, "%s", m_text.c_str());
				break;
			case false:
				al_draw_filled_rectangle(m_pos_x, m_pos_y, m_pos_x2, m_pos_y2, m_color_untouched);
				al_draw_textf(m_font, m_color_touched, m_pos_xc, m_pos_yc - m_font_height / 2, ALLEGRO_ALIGN_CENTRE, "%s", m_text.c_str());
				break;
			}
			break;
		case RECTANGLE_FILLED:
			switch (m_touched)
			{
			case true:
				al_draw_filled_rectangle(m_pos_x, m_pos_y, m_pos_x2, m_pos_y2, m_color_touched);
				al_draw_textf(m_font, m_color_untouched, m_pos_xc, m_pos_yc - m_font_height / 2, ALLEGRO_ALIGN_CENTRE, "%s", m_text.c_str());
				break;
			case false:
				al_draw_filled_rectangle(m_pos_x, m_pos_y, m_pos_x2, m_pos_y2, m_color_untouched);
				al_draw_textf(m_font, m_color_touched, m_pos_xc, m_pos_yc - m_font_height / 2, ALLEGRO_ALIGN_CENTRE, "%s", m_text.c_str());
				break;
			}
			break;
		case RECTANGLE_ROUNDED:
			switch (m_touched)
			{
			case true:
				al_draw_rounded_rectangle(m_pos_x, m_pos_y, m_pos_x2, m_pos_y2, m_rx, m_ry, m_color_touched, m_thickness);
				al_draw_textf(m_font, m_color_untouched, m_pos_xc, m_pos_yc - m_font_height / 2, ALLEGRO_ALIGN_CENTRE, "%s", m_text.c_str());
				break;
			case false:
				al_draw_rounded_rectangle(m_pos_x, m_pos_y, m_pos_x2, m_pos_y2, m_rx, m_ry, m_color_untouched, m_thickness);
				al_draw_textf(m_font, m_color_touched, m_pos_xc, m_pos_yc - m_font_height / 2, ALLEGRO_ALIGN_CENTRE, "%s", m_text.c_str());
				break;
			}
			break;
		case RECTANGLE_ROUNDED_FILLED:
			switch (m_touched)
			{
			case true:
				al_draw_filled_rounded_rectangle(m_pos_x, m_pos_y, m_pos_x2, m_pos_y2, m_rx, m_ry, m_color_touched);
				al_draw_textf(m_font, m_color_untouched, m_pos_xc, m_pos_yc - m_font_height / 2, ALLEGRO_ALIGN_CENTRE, "%s", m_text.c_str());
				break;
			case false:
				al_draw_filled_rounded_rectangle(m_pos_x, m_pos_y, m_pos_x2, m_pos_y2, m_rx, m_ry, m_color_untouched);
				al_draw_textf(m_font, m_color_touched, m_pos_xc, m_pos_yc - m_font_height / 2, ALLEGRO_ALIGN_CENTRE, "%s", m_text.c_str());
				break;
			}
			break;
		default:
			switch (m_touched)
			{
			case true:
				al_draw_filled_rectangle(m_pos_x, m_pos_y, m_pos_x2, m_pos_y2, m_color_touched);
				al_draw_textf(m_font, m_color_untouched, m_pos_xc, m_pos_yc - m_font_height / 2, ALLEGRO_ALIGN_CENTRE, "%s", m_text.c_str());
				break;
			case false:
				al_draw_filled_rectangle(m_pos_x, m_pos_y, m_pos_x2, m_pos_y2, m_color_untouched);
				al_draw_textf(m_font, m_color_touched, m_pos_xc, m_pos_yc - m_font_height / 2, ALLEGRO_ALIGN_CENTRE, "%s", m_text.c_str());
				break;
			}
			break;
		}
		break;
	}
}

void ButtonRectangle::ChangeText(std::string text) { m_text = text; }

#endif // !BUTTONRECTANGLE_H