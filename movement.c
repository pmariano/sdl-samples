#include <stdio.h>
#include <SDL/SDL.h>

#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 768
#define BPP 32

SDL_Surface *screen, *dot;


int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0 ) return 1;

    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_SWSURFACE);
	dot = SDL_LoadBMP("cube_red.bmp");
	SDL_DisplayFormat(dot);

	SDL_Rect rect;
	SDL_Event event;

	int count;
	int exit = 0;
	int vel = 2;

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
			  case SDLK_EQUALS:
				vel *= 2;
				break;
			  case SDLK_MINUS:
				vel /= 2;
				break;
			  case SDLK_UP:
				rect.y +=2;
				break;
			  case SDLK_DOWN:
				rect.y -=2;
				break;
			  case SDLK_LEFT:
				rect.x -=2;
				break;
			  case SDLK_RIGHT:
				rect.x +=2;
				break;
			}
		  break;
		}
	  }

	  Uint8 *keystate;
	  keystate = SDL_GetKeyState(NULL);

	  if (keystate[SDLK_LEFT] ) {
		rect.x -= vel;
	  }
	  if (keystate[SDLK_RIGHT] ) {
		rect.x += vel;
	  }
	  if (keystate[SDLK_UP] ) {
		rect.y -= vel;
	  }

	  if (keystate[SDLK_DOWN] ) {
		rect.y += vel;
	  }

	  if ( rect.x < 0 ) {
		rect.x = 0;
	  } else if ( rect.x > SCREEN_WIDTH - BPP) {
		rect.x = SCREEN_WIDTH - BPP;
	  }

	  if ( rect.y < 0 ) {
		rect.y = 0;
	  }
	  else if ( rect.y > SCREEN_HEIGHT - BPP) {
		rect.y = SCREEN_HEIGHT -BPP;
	  }


	  SDL_BlitSurface(dot, NULL, screen, &rect);
	  SDL_UpdateRect(screen, 0, 0, 0, 0);
	}

    return 0;
}


