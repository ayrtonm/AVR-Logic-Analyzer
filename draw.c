#include <SDL/SDL.h>
#define YSCALE 16
#define H_WIDTH 4
#define Y_WIDTH 1
#define YOFFSET 4
#define SCREENX 256
#define SCREENY 256
#define MAX_SPEED 100
#define MIN_SPEED 100000

typedef unsigned char uint8;

int main(int argc, char *argv[])
{
  if (argc == 2)
  {
    printf("Digital Logic Analyzer\npress e to increase scrolling speed\npress d to decrease scrolling speed\npress q to quit\n");
    return 0;
  }
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Surface *disp,*buffer;
  disp = SDL_SetVideoMode(SCREENX,SCREENY,32,SDL_HWSURFACE);
  buffer = SDL_CreateRGBSurface(SDL_HWSURFACE,SCREENX,SCREENY,32,0,0,0,0);
  SDL_WM_SetCaption("Digital Logic Analyzer",NULL);
  int i,j,quit = 0;
  int delay = 1000;
  uint8 data[2] = {0,0};
  uint8 bit;
  SDL_Event event;
  SDL_Rect screen_src = {0,0,SCREENX-H_WIDTH,SCREENY};
  SDL_Rect screen_dst = {H_WIDTH,0,SCREENX-H_WIDTH,SCREENY};
  for(;;)
  {
    //copy display to buffer
    SDL_BlitSurface(disp,NULL,buffer,NULL);
    //clear display
    SDL_FillRect(disp,NULL,0x000000);
    for (j = 0; j < 2; j++)
    {
      for (i = 0; i < 8; i++)
      {
        //draw new bits on display
        bit = data[j] & (1 << i);
        //make horizontal line
        SDL_Rect dst = {0,bit ? (j*8+i)*YSCALE+YOFFSET : (j*8+i-.5)*YSCALE+YOFFSET,H_WIDTH,1};
        SDL_FillRect(disp,&dst,SDL_MapRGB(disp->format,0x00,0xff,0x00));
      }
    }
    //copy buffer to display
    SDL_BlitSurface(buffer,&screen_src,disp,&screen_dst);
    SDL_Flip(disp);
    usleep(delay);
    //read keyboard
    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
        case SDL_KEYDOWN:
        {
          switch(event.key.keysym.sym)
          {
            case SDLK_e: {delay = (delay == MAX_SPEED ? delay : delay - 100);break;}
            case SDLK_d: {delay = (delay == MIN_SPEED ? delay : delay + 100);break;}
            case SDLK_q: {quit = 1;break;}
          }
        }
        break;
      }
    }
    if (quit) break;
  }
  SDL_FreeSurface(disp);
  SDL_FreeSurface(buffer);
  return 0;
}
