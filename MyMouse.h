// ___ ==================================================
// ___ file: MyMouse.h
// ___ project: Interval Timer
// ___ author: Paulina Kalicka
// ___ ==================================================

#ifndef MY_MOUSE_H
#define MY_MOUSE_H

class MyMouse
{
public:
	MyMouse();
	~MyMouse();

	float last_positions[20][2] = {};
	float curr_px;
	float curr_py;

	// it can be 0-19 (so for 20 positions)
	unsigned counter;
	// it increments when mouse is not moving
	// it says when all positions should be reset
	unsigned timer_counter;

	void Draw();
	void UpdatePositions(float mx, float my);
	void UpdateCounter(unsigned timer_ticks);
};

#endif // !MY_MOUSE_H
