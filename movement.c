#include <stdio.h>
#include <SDL/SDL.h>

#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 768
#define BPP 32


#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

SDL_Surface *screen, *dot, *followers;
int num_followers = 1;
// 1 up 2 down 3 left 4 right
int orientation = 0;
SDL_Rect previous_rect;

void blit_follower(SDL_Rect origin_rect, int follower_num){
  SDL_Rect custom_rect;
  custom_rect.x = origin_rect.x;
  custom_rect.y = origin_rect.y;

  switch(orientation){
	case UP:
	  custom_rect.y = origin_rect.y + (15 * follower_num);
	  break;
	case DOWN:
	  custom_rect.y = origin_rect.y - (15* follower_num);
	  break;
	case LEFT:
	  custom_rect.x = origin_rect.x + (15 * follower_num);
	  break;
	case RIGHT:
	  custom_rect.x = origin_rect.x - (15 * follower_num);
	  break;
  }

  SDL_BlitSurface(followers, NULL, screen, &custom_rect);
}

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0 ) return 1;

    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_HWSURFACE);
	dot = SDL_LoadBMP("cube_red.bmp");
	followers = SDL_LoadBMP("cube_green.bmp");

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
			  case SDLK_p:
				num_followers++;
				break;
			  case SDLK_MINUS:
				vel /= 2;
				break;
			  case SDLK_UP:
				rect.y +=2;
				orientation = UP;
				break;
			  case SDLK_DOWN:
				orientation = DOWN;
				rect.y -=2;
				break;
			  case SDLK_LEFT:
				rect.x -=2;
				orientation = LEFT;
				break;
			  case SDLK_RIGHT:
				orientation = RIGHT;
				rect.x +=2;
				break;
			}
		  break;
		}
	  }

	  SDL_FillRect(screen , NULL , 0x00000000);

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
	  } else if ( rect.x > SCREEN_WIDTH - dot->w) {
		rect.x = SCREEN_WIDTH - dot->w;
	  }

	  if ( rect.y < 0 ) {
		rect.y = 0;
	  }
	  else if ( rect.y > SCREEN_HEIGHT - dot->h) {
		rect.y = SCREEN_HEIGHT - dot->h;
	  }

	  SDL_BlitSurface(dot, NULL, screen, &rect);

	  int x;
	  for(x = 1; x<num_followers;x++){
		blit_follower(rect, x);
	  }

	  SDL_UpdateRect(screen, 0, 0, 0, 0);
	}

    return 0;
}


