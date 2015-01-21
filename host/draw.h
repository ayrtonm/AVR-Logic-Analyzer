#ifndef DRAW_H_INCLUDE
#define DRAW_H_INCLUDE
#include <SDL/SDL.h>

//all other size macros are written in terms of scale macros
#define SCALE 6
#define SCALE_Y 4*SCALE
#define LINE_LENGTH SCALE
#define LINE_WIDTH (SCALE/4)
#define OFFSET SCALE
#define SCREEN_WIDTH 64*SCALE
#define SCREEN_HEIGHT 64*SCALE

#define MIN_DELAY 100
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
int not_running;
int draw_delay;

#endif
