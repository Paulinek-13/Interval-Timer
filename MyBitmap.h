#ifndef MYBITMAP_HPP
#define MYBITMAP_HPP

class MyBitmap
{
private:

	ALLEGRO_BITMAP*m_bitmap;

	// what the bitmap should do (ex. move right, scale)
	BITMAP_BEHAVE m_behave;

	float m_curr_px;
	float m_curr_py;
	float m_sx;
	float m_sy;
	float m_s_width;
	float m_s_height;
	float m_sw_draw;
	float m_sh_draw;
	float m_d_width;
	float m_d_height;
	float m_draw_speed;
	float m_scale_speed;

	bool m_draw;

	// whether should change the properties of the bitmap (ex. positions, size)
	// if false - the bitmap does nothing
	bool m_change_args;

public:

	MyBitmap(ALLEGRO_BITMAP*bitmap);
	~MyBitmap();

	// just draw the bitmap
	void DrawIntro();

	// to put in if condition with event type: ALLEGRO_EVENT_TIMER
	// change the properties of the bitmap (ex. positions, size)
	void TimerIntro();

	// as above but determines when switch to next behave and start change it
	void TimerIntro_2(bool when);

	// it sets the first behave of the bitmap
	void SetIntro();

	// it sets the second behave of the bitmap
	void SetIntro_2();

};

MyBitmap::MyBitmap(ALLEGRO_BITMAP*bitmap)
{
	m_bitmap = bitmap;

	m_behave = BITMAP_DEFAULT;

	m_sx = 0;
	m_sy = 0;
	m_s_width = al_get_bitmap_width(bitmap);
	m_s_height = al_get_bitmap_height(bitmap);
	m_curr_px = DISPLAY_WIDTH / 2 - m_s_height / 2;
	m_curr_py = DISPLAY_HEIGHT / 2 - m_s_height / 2;

	m_draw = false;
	m_change_args = true;
}
MyBitmap::~MyBitmap()
{
}

void MyBitmap::DrawIntro()
{
	if (m_draw)
	{
		switch (m_behave)
		{
		case BITMAP_DEFAULT:
			al_draw_scaled_bitmap(m_bitmap, m_sx, m_sy, m_s_width, m_s_height, m_curr_px, m_curr_py, m_d_width, m_d_height, 0);
			break;
		case BITMAP_DRAW_PIECE:
			al_draw_bitmap_region(m_bitmap, m_sx, m_sy, m_sw_draw, m_sh_draw, m_curr_px, m_curr_py, 0);
			break;
		case BITMAP_MOVE:
			break;
		case BITMAP_SCALE:
			al_draw_scaled_bitmap(m_bitmap, m_sx, m_sy, m_s_width, m_s_height, m_curr_px, m_curr_py, m_d_width, m_d_height, 0);
			break;
		case BITMAP_MOVE_SCALE:
			al_draw_scaled_bitmap(m_bitmap, m_sx, m_sy, m_s_width, m_s_height, m_curr_px, m_curr_py, m_d_width, m_d_height, 0);
			break;
		default:
			al_draw_scaled_bitmap(m_bitmap, m_sx, m_sy, m_s_width, m_s_height, m_curr_px, m_curr_py, m_d_width, m_d_height, 0);
			break;
		}
	}
}
void MyBitmap::TimerIntro()
{
	if (m_draw)
	{
		switch (m_behave)
		{
		case BITMAP_DEFAULT:
			break;
		case BITMAP_DRAW_PIECE:
			if (m_change_args)
			{
				m_sx = 0;
				m_sy = 0;
				m_sw_draw = 0;
				m_sh_draw = m_s_height;
				m_curr_px = DISPLAY_WIDTH / 2 - m_s_width / 2;
				m_curr_py = DISPLAY_HEIGHT / 2 - m_s_height / 2;
				m_draw_speed = 10;

				m_change_args = false;
			}
			if (m_sw_draw < m_s_width)
			{
				m_sw_draw += m_draw_speed;
			}
			else
			{
				m_change_args = true;
				m_behave = BITMAP_MOVE_SCALE;
			}
			break;
		case BITMAP_MOVE:
			break;
		case BITMAP_SCALE:
			break;
		case BITMAP_MOVE_SCALE:
			if (m_change_args)
			{
				m_d_width = m_s_width;
				m_d_height = m_s_height;
				m_scale_speed = 5;

				m_change_args = false;
			}
			if (m_d_width > m_s_width / 2 && m_d_height > m_s_height / 2)
			{
				m_d_width -= m_scale_speed;
				m_d_height -= m_scale_speed;
				m_curr_px += m_scale_speed / 2;
				m_curr_py -= m_scale_speed / 4;
			}
			else
			{
				m_change_args = true;
				m_behave = BITMAP_DEFAULT;
			}
			break;
		default:
			break;
		}
	}
}
void MyBitmap::TimerIntro_2(bool when)
{
	if (when)SetIntro_2();
	if (m_draw)
	{
		switch (m_behave)
		{
		case BITMAP_DEFAULT:
			break;
		case BITMAP_DRAW_PIECE:
			break;
		case BITMAP_MOVE:
			break;
		case BITMAP_SCALE:
			if (m_change_args)
			{
				m_d_width = 0;
				m_d_height = 0;
				m_scale_speed = 2;
				m_curr_px = DISPLAY_WIDTH / 2;
				m_curr_py = DISPLAY_HEIGHT / 2;

				m_change_args = false;
			}
			if (m_d_width <= m_s_width || m_d_height <= m_s_height)
			{
				if (m_d_width <= m_s_width)m_d_width += m_scale_speed;
				if (m_d_height <= m_s_height)m_d_height += m_scale_speed;
			}
			break;
		case BITMAP_MOVE_SCALE:
			break;
		default:
			break;
		}
	}
}
void MyBitmap::SetIntro() { m_behave = BITMAP_DRAW_PIECE; m_draw = true; }
void MyBitmap::SetIntro_2() { m_behave = BITMAP_SCALE; m_draw = true; }

#endif // MYBITMAP_H
