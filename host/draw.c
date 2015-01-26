#include <SDL/SDL.h>
#include "draw.h"
#define MIN(a,b) (a > b ? b : a)
#define MAX(a,b) (a > b ? a : b)

void init_draw(void)
{
  SDL_Init(SDL_INIT_EVERYTHING);
  disp = SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,32,SDL_HWSURFACE);
  buffer = SDL_CreateRGBSurface(SDL_HWSURFACE,SCREEN_WIDTH,SCREEN_HEIGHT,32,0,0,0,0);
  SDL_WM_SetCaption("Logic Analyzer",NULL);
  screen_src.x = 0;
  screen_src.y = 0;
  screen_src.w = SCREEN_WIDTH-LINE_LENGTH;
  screen_src.h = SCREEN_HEIGHT;
  screen_dst.x = LINE_LENGTH;
  screen_dst.y = 0;
  screen_dst.w = SCREEN_WIDTH-LINE_LENGTH;
  screen_dst.h = SCREEN_HEIGHT;
  not_running = 0;
  k_down = 0;
  k_up = 0;
  paused = 0;
  step = 1;
  t = 0;
  draw_delay = MAX_DELAY;
}
void redraw_screen(char *shift_buffer)
{
  int i,j,bit,old_bit;
  //copy display to buffer
  SDL_BlitSurface(disp,NULL,buffer,NULL);
  //clear display
  SDL_FillRect(disp,NULL,0x000000);
  t++;
  if (t == FULL_SCREEN) {SDL_FillRect(buffer,NULL,0x000000);t = 0;}
  /*
  for (i = 0; i < 8; i++)
  {
	  SDL_Rect x_line = {0,X_LINES*i,SCREEN_WIDTH,2};
	  SDL_Rect y_line = {Y_LINES*i,0,2,SCREEN_HEIGHT};
	  SDL_FillRect(disp,&x_line,SDL_MapRGB(disp->format,0xcc,0xcc,0xcc));
	  SDL_FillRect(disp,&y_line,SDL_MapRGB(disp->format,0xcc,0xcc,0xcc));
  }
  */
  for (j = 0; j < 2; j++)
  {
    for (i = 0; i < 8; i++)
    {
      //draw new bits on display
      bit = shift_buffer[j] & (1 << i);
      //old bit
      old_bit = shift_buffer[j+2] & (1 << i);
      //make horizontal line
      SDL_Rect dst = {0,bit ? (j*8+i)*SCALE_Y+OFFSET : (j*8+i-.5)*SCALE_Y+OFFSET,LINE_LENGTH,LINE_WIDTH};
      if (i < 4) {j ? SDL_FillRect(disp,&dst,SDL_MapRGB(disp->format,0x00,0xff,0x00)) : SDL_FillRect(disp,&dst,SDL_MapRGB(disp->format,0xff,0xff,0x00));}
      else {j ? SDL_FillRect(disp,&dst,SDL_MapRGB(disp->format,0x00,0x00,0xff)) : SDL_FillRect(disp,&dst,SDL_MapRGB(disp->format,0xff,0x00,0x00));}
      //if bit changed
      if (bit ^ old_bit)
      {
        SDL_Rect vert = {LINE_LENGTH-1,(j*8+i-.5)*SCALE_Y+OFFSET,LINE_WIDTH,.5*SCALE_Y};
        if (i < 4) {j ? SDL_FillRect(disp,&vert,SDL_MapRGB(disp->format,0x00,0xff,0x00)) : SDL_FillRect(disp,&vert,SDL_MapRGB(disp->format,0xff,0xff,0x00));}
        else {j ? SDL_FillRect(disp,&vert,SDL_MapRGB(disp->format,0x00,0x00,0xff)) : SDL_FillRect(disp,&vert,SDL_MapRGB(disp->format,0xff,0x00,0x00));}
      }
    }
  }
  //copy buffer to display
  SDL_BlitSurface(buffer,&screen_src,disp,&screen_dst);
  if (t == FULL_SCREEN - 1) {SDL_Flip(disp);usleep(draw_delay);}
}
int process_keypresses(void)
{
  if (not_running)
  {
    SDL_FreeSurface(disp);
    SDL_FreeSurface(buffer);
    return 1;
  }
  //read keyboard
  while(SDL_PollEvent(&event))
  {
    switch(event.type)
    {
      case SDL_KEYDOWN:
      {
        switch(event.key.keysym.sym)
        {
          case SDLK_e: {k_up = 1;k_down = 0;return 0;}
          case SDLK_d: {k_down = 1;k_up = 0;return 0;}
          case SDLK_r: {step = MIN(step + 1,4);return 0;}
          case SDLK_f: {step = MAX(step - 1,1);return 0;}
          case SDLK_p: {paused ^= 1;break;}
          case SDLK_q: {not_running = 1;return 0;}
        }
      } break;
      case SDL_KEYUP:
      {
        switch(event.key.keysym.sym)
        {
          case SDLK_e: {k_up = 0;return 0;}
          case SDLK_d: {k_down = 0;return 0;}
        }
      } break;
    }
  }
}
