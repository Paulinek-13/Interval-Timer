#ifndef GLOBALS_H
#define GLOBALS_H

/* ! INCLUDING ALLEGRO 5 AND C++ STANDARD LIBRARY HEADERS */

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

#include <string>

/* ! IMPORTANT CONSTS */

const float DISPLAY_WIDTH = 800;
const float DISPLAY_HEIGHT = 600;

const double TIMER_SPEED = 1 / 60.0;

/* ! INFO ABOUT APP */

#define ORG_NAME "PAULINEK"
#define APP_NAME "TIMER"

/* ! DEFINISIONS OF COLORS */

#define COLOR_BLACK al_map_rgb(0, 0, 0)
#define COLOR_WHITE al_map_rgb(255, 255, 255)
#define COLOR_CORNSILK al_map_rgb(255, 242, 222)
#define COLOR_PEACH al_map_rgb(255, 225, 181)
#define COLOR_CHARCOAL al_map_rgb(50, 55, 65)
#define COLOR_DARK_CYAN al_map_rgb(6, 133, 140)
#define COLOR_MINT al_map_rgb(69, 196, 139)
#define COLOR_MANGO_TANGO al_map_rgb(244, 121, 66)
#define COLOR_SUNGLOW al_map_rgb(255, 208, 57)

/* ! PATHS TO RESOURCES */

#define SAMPLE_1_ONE "res//samples//sample_1_one.ogg"
#define SAMPLE_2_TWO "res//samples//sample_2_two.ogg"
#define SAMPLE_3_THREE "res//samples//sample_3_three.ogg"
#define SAMPLE_LETS_GO "res//samples//sample_lets_go.ogg"
#define SAMPLE_YOU_WIN "res//samples//sample_you_win.ogg"
#define SAMPLE_APPLAUSE "res//samples//sample_applause.ogg"
#define SAMPLE_CLUNK "res//samples//sample_clunk.ogg"
#define SAMPLE_TICK "res//samples//sampe_tick.ogg"
#define SAMPLE_INTRO "res//samples//sample_intro.ogg"
#define SAMPLE_MAIN "res//samples//sample_main.ogg"
#define SAMPLE_MOUSE_DOWN "res//samples//sample_mouse_down.ogg"

#define BITMAP_PAULINEK_LOGO "res//bitmaps//bitmap_PAULINEK_logo.png"
#define BITMAP_TIMER_LOGO "res//bitmaps//bitmap_timer_logo.png"

#define FONT_CODE_PRO "res//fonts//font_code_pro.ttf"

/* ! ENUMS */

enum MENU
{
	MENU_DEF,
	MENU_START,
	MENU_CHANGE_EXERCISE,
	MENU_CHANGE_REST,
	MENU_CHANGE_SETS
};

enum BUTTON_TYPE
{
	RECTANGLE,
	RECTANGLE_FILLED,
	RECTANGLE_ROUNDED,
	RECTANGLE_ROUNDED_FILLED
};

enum BITMAP_BEHAVE
{
	BITMAP_DEFAULT,
	BITMAP_DRAW_PIECE,
	BITMAP_MOVE,
	BITMAP_SCALE,
	BITMAP_MOVE_SCALE
};

enum TIMER_STATE
{
	COUNTING,
	EXERCISE,
	REST,
	FINISHED
};

/* ! INCLUDING HEADERS WITH MY CLASSES */

#include "ButtonRectangle.h"
#include "MyBitmap.h"
#include "MyMouse.h"
#include "MyText.h"
#include "TimerMenu.h"

#endif // !GLOBALS_H