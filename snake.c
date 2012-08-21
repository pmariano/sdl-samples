#include <stdio.h>
#include <SDL/SDL.h>

#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 768
#define FPS 30


#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

SDL_Surface *screen, *head_img, *tail_img;
int num_tail_img = 1;
// 1 up 2 down 3 left 4 right
int orientation = 4;
int vel = 2;
int bpi = 15;
int pieces_qtd = 1;
SDL_Rect pieces[50000] = {};

void move(){
	int j , sumx, sumy;

	for(j = pieces_qtd-1; j >= 0;j--){
	  SDL_Rect* piece = &pieces[j];

	  if(j == 0){
		printf("HEAD POS X: %i Y: %i \n", piece->x, piece->y);
		switch(orientation){
		  case UP:
			piece->y -= bpi;
			break;
		  case DOWN:
			piece->y += bpi;
			break;
		  case LEFT:
			piece->x -= bpi;
			break;
		  case RIGHT:
			piece->x += bpi;
			break;
		 }

		SDL_BlitSurface(head_img, NULL, screen, piece);
	  } else {
		SDL_Rect before = pieces[j-1];
		printf("BEFORE POS X: %i Y: %i \n", before.x, before.y);
		piece->x = before.x;
		piece->y = before.y;
		SDL_BlitSurface(tail_img, NULL, screen, piece);
	  }
	}
}

void add_a_piece(){
	int len = sizeof(pieces)/sizeof(SDL_Rect);
	int x = pieces[pieces_qtd - 1].x;
	int y = pieces[pieces_qtd - 1].y;

	SDL_Rect* next_piece = &pieces[pieces_qtd];
	next_piece->x = x;
	next_piece->y = y;
	pieces_qtd++;
}

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0 ) return 1;

	//round fail
	printf("FPS, %i \n", 1000/FPS);

    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_HWSURFACE);
	head_img = SDL_LoadBMP("cube_red.bmp");
	tail_img = SDL_LoadBMP("cube_green.bmp");

	Uint32 last_update = SDL_GetTicks();
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
				if(!(orientation == DOWN)){
				  orientation = UP;
				}
				break;
			  case SDLK_DOWN:
				if(!(orientation == UP)){
				  orientation = DOWN;
				}
				break;
			  case SDLK_LEFT:
				if(!(orientation == RIGHT)){
				  orientation = LEFT;
				}
				break;
			  case SDLK_RIGHT:
				if(!(orientation == LEFT)){
				  orientation = RIGHT;
				}
				break;
			}
		  break;
		}
	  }

	  Uint32 current_time = SDL_GetTicks();
	  //arredondamento fail
	  if((current_time - last_update) >= 1000/FPS){
		SDL_FillRect(screen , NULL , 0x00000000);
		move();
		last_update = current_time;
		SDL_UpdateRect(screen, 0, 0, 0, 0);
	  }

	}

    return 0;
}


