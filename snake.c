#include <stdio.h>
#include <SDL/SDL.h>

#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 768
#define BPP 32


#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

SDL_Surface *screen, *head_img, *tail_img;
int num_tail_img = 1;
// 1 up 2 down 3 left 4 right
int orientation = 4;
int vel = 2;
int pieces_qtd = 1;
SDL_Rect pieces[1000] = {};

void move(){
	int j;

	for(j = 0; j <= pieces_qtd - 1;j++){
	  SDL_Rect* piece = &pieces[j];
	  if(j == 0){
		switch(orientation){
		  case UP:
			piece->y -= vel;
			break;
		  case DOWN:
			piece->y += vel;
			break;
		  case LEFT:
			piece->x -= vel;
			break;
		  case RIGHT:
			piece->x += vel;
			break;
		 }
		SDL_BlitSurface(head_img, NULL, screen, piece);
	  } else {
		SDL_Rect nextpiece = pieces[j-1];
		piece->x = nextpiece.x;
		piece->y = nextpiece.y;
		SDL_BlitSurface(tail_img, NULL, screen, piece);
	  }
	}
}

void add_a_piece(){
	int len = sizeof(pieces)/sizeof(SDL_Rect);

	int x = pieces[pieces_qtd - 1].x + 15;
	int y = pieces[pieces_qtd - 1].y + 15;

	int j;
	for(j = 0; j<5;j++){
	  SDL_Rect* next_piece = &pieces[pieces_qtd];
	  next_piece->x = x;
	  next_piece->y = y;
	  pieces_qtd++;
	}
}

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0 ) return 1;

    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_HWSURFACE);
	head_img = SDL_LoadBMP("cube_red.bmp");
	tail_img = SDL_LoadBMP("cube_green.bmp");

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
				exit = 1;
				break;
			  case SDLK_EQUALS:
				vel *= 2;
				break;
			  case SDLK_p:
				add_a_piece();
				break;
			  case SDLK_MINUS:
				vel /= 2;
				break;
			  case SDLK_UP:
				orientation = UP;
				break;
			  case SDLK_DOWN:
				orientation = DOWN;
				break;
			  case SDLK_LEFT:
				orientation = LEFT;
				break;
			  case SDLK_RIGHT:
				orientation = RIGHT;
				break;
			}
		  break;
		}
	  }
	  SDL_FillRect(screen , NULL , 0x00000000);

	  move();

	  SDL_UpdateRect(screen, 0, 0, 0, 0);
	}

    return 0;
}


