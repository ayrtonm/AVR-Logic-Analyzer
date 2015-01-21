#include <SDL/SDL.h>
#include "draw.h"

void init_draw(void)
{
  SDL_Init(SDL_INIT_EVERYTHING);
  disp = SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,32,SDL_HWSURFACE);
  buffer = SDL_CreateRGBSurface(SDL_HWSURFACE,SCREEN_WIDTH,SCREEN_HEIGHT,32,0,0,0,0);
  SDL_WM_SetCaption("Digital Logic Analyzer",NULL);
  screen_src.x = 0;
  screen_src.y = 0;
  screen_src.w = SCREEN_WIDTH-LINE_LENGTH;
  screen_src.h = SCREEN_HEIGHT;
  screen_dst.x = LINE_LENGTH;
  screen_dst.y = 0;
  screen_dst.w = SCREEN_WIDTH-LINE_LENGTH;
  screen_dst.h = SCREEN_HEIGHT;
  not_running = 0;
  draw_delay = 1000;
}
void redraw_screen(char *shift_buffer)
{
  int i,j,bit;
  //copy display to buffer
  SDL_BlitSurface(disp,NULL,buffer,NULL);
  //clear display
  SDL_FillRect(disp,NULL,0x000000);
  for (j = 0; j < 2; j++)
  {
    for (i = 0; i < 8; i++)
    {
      //draw new bits on display
      bit = shift_buffer[j] & (1 << i);
      //make horizontal line
      SDL_Rect dst = {0,bit ? (j*8+i)*SCALE_Y+OFFSET : (j*8+i-.5)*SCALE_Y+OFFSET,LINE_LENGTH,LINE_WIDTH};
      SDL_FillRect(disp,&dst,SDL_MapRGB(disp->format,0x00,0xff,0x00));
    }
  }
  //copy buffer to display
  SDL_BlitSurface(buffer,&screen_src,disp,&screen_dst);
  SDL_Flip(disp);
  usleep(draw_delay);
}
int process_keypresses(void)
{
  //read keyboard
  while(SDL_PollEvent(&event))
  {
    switch(event.type)
    {
      case SDL_KEYDOWN:
      {
        switch(event.key.keysym.sym)
        {
          case SDLK_e: {draw_delay = (draw_delay == MIN_DELAY ? draw_delay : draw_delay - 100);return 0;}
          case SDLK_d: {draw_delay = (draw_delay == MAX_DELAY ? draw_delay : draw_delay + 100);return 0;}
          case SDLK_q: {not_running = 1;return 0;}
        }
      } break;
    }
  }
  if (not_running)
  {
    SDL_FreeSurface(disp);
    SDL_FreeSurface(buffer);
    return 1;
  }
}
