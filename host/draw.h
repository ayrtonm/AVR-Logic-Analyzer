#ifndef DRAW_H_INCLUDE
#define DRAW_H_INCLUDE
#include <SDL/SDL.h>

//all other size macros are written in terms of scale macros
#define SCALE 8
#define SCALE_Y 4*SCALE
#define LINE_LENGTH SCALE
#define LINE_WIDTH (SCALE/4)
#define OFFSET SCALE*3

#define FULL_SCREEN (SCREEN_WIDTH/LINE_LENGTH)

#define X_LINES (SCREEN_WIDTH/32)
#define Y_LINES (SCREEN_HEIGHT/32)

#define SCREEN_WIDTH SCALE*64
#define SCREEN_HEIGHT 68*SCALE

#define MIN_DELAY 10
#define MAX_DELAY 100000

typedef unsigned char uint8;

//functions
extern void init_draw(void);
extern void redraw_screen(char *shift_buffer);
extern int process_keypresses(void);
//sdl variables
SDL_Surface *disp,*buffer;
SDL_Event event;

//constants
SDL_Rect screen_src;
SDL_Rect screen_dst;

//variables
int k_down,k_up;
int t;
int not_running, paused, draw_delay,step;

#endif
