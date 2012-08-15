#include <stdio.h>
#include <SDL/SDL.h>

SDL_Surface *screen;

void setpixel(int x, int y, int color){
  unsigned int *pixels = (unsigned int*) screen->pixels;
  int lineoffset = y * screen->w;

//  pixels[x + lineoffset] = ((x&y)<<8) | color;
  pixels[x+lineoffset] = color;
}


void drawscreen(int rand, void (*funcPointer) (int, int, int)){
  int y, x;
  for(y = 0; y < screen->h; y++) {
	for (x=0; x < screen->w; x++) {
	  (*funcPointer)(x,y,rand);
//setpixel(x,y,color + (rand << 8));
	}
  }
}

void draw1 (int x, int y, int rand){
  int color = 0xff000000;
  color = color + (rand << 8);
  color = ((x&y)<<8) | color;
  setpixel(x,y, color);
}

void draw2(int x, int y, int rand){
  int color = 0x00ff0000;
  color = color + (rand << 8);
  color = ((x^y)<<8) & color;
  setpixel(x,y, color);
}

void draw3(int x, int y, int rand){
  int color = 0xffffff;
  color = color + (rand << 24);
  color = ((x&y)<<24) & color;
  setpixel(x,y, color);
}

void draw4(int x, int y, int rand){
  int color = 0x00000000;
  color = color + (rand << 16);
  color = ((x^y)<< 16) ^ color;
  setpixel(x,y, color);
}

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0 ) return 1;

    screen = SDL_SetVideoMode(1024, 768, 32, SDL_SWSURFACE);
	SDL_Event event;

	int count;
	int exit = 0;
	void (*funcPointer)(int, int, int) = &draw1;

	while(!exit){
	  count++;

	  if(SDL_PollEvent(&event)){
		switch(event.type) {
		  case SDL_QUIT:
			exit = 1;
			break;
		  case SDL_KEYDOWN:
			switch(event.key.keysym.sym) {
			  case SDLK_ESCAPE:
			  case SDLK_q:
				exit = 1;
				break;
			  case SDLK_UP:
				funcPointer = &draw1;
				break;
			  case SDLK_DOWN:
				funcPointer = &draw2;
				break;
			  case SDLK_LEFT:
				funcPointer = &draw3;
				break;
			  case SDLK_RIGHT:
				funcPointer = &draw4;
				break;
			}
		  break;
		}
	  }

	  drawscreen(count, funcPointer);
	  SDL_Flip(screen);
	}

    return 0;
}


