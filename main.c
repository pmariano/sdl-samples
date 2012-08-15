#include <stdio.h>
#include <SDL/SDL.h>

SDL_Surface *screen;

void setpixel(int x, int y, int color){
  unsigned int *pixels = (unsigned int*) screen->pixels;
  int lineoffset = y * screen->w;

  pixels[x + lineoffset] = ((x&y)<<8) | color;
}


int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0 ) return 1;

    screen = SDL_SetVideoMode(1024, 768, 32, SDL_FULLSCREEN);
	SDL_Event event;

	int count;
	int exit = 0;

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
				exit =1;
				break;
			}
		  break;
		}
	  }

	  int y, x, ytimesw;

	  for(y = 0; y < screen->h; y++) {
		for (x=0; x < screen->w; x++) {
		  setpixel(x,y, 0xFF000000 + (count << 8));
		}
	  }

	  SDL_Flip(screen);
	}

    return 0;
}
