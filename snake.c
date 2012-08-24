#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL/SDL.h>

#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 768
#define FPS 30

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

SDL_Surface *screen, *head_img, *tail_img, *food_img;
int num_tail_img = 1;
// 1 up 2 down 3 left 4 right
int orientation = 4;
int vel = 2;
int bpi = 15;
int pieces_qtd = 1;
int food_qtd = 0;
SDL_Rect pieces[50000] = {};
SDL_Rect food ;

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
		piece->x = before.x;
		piece->y = before.y;
		SDL_BlitSurface(tail_img, NULL, screen, piece);
	  }
	}
}

void add_piece(){
	int len = sizeof(pieces)/sizeof(SDL_Rect);
	int x = pieces[pieces_qtd - 1].x;
	int y = pieces[pieces_qtd - 1].y;

	SDL_Rect* next_piece = &pieces[pieces_qtd];
	next_piece->x = x;
	next_piece->y = y;
	pieces_qtd++;
}

int generate_random(int min, int max){
  int r = (rand() + max + min) % max ;

  //can I remove this if?
  if(r < min ){
	r += min;
  }

  return r;
}

void draw_food(){
  if(food_qtd == 0){
	food.x = generate_random(15,SCREEN_WIDTH);
	food.y = generate_random(15,SCREEN_HEIGHT);

	food_qtd++;
  }

  SDL_BlitSurface(food_img, NULL, screen, &food);
}


void handle_colision(){
  SDL_Rect head = pieces[0];
  if(food_qtd > 0){
	int xdiff = abs(head.x - food.x);
	int ydiff = abs(head.y - food.y);
	if(xdiff <= 15 && ydiff <= 15){
	  printf("RONALDOOOO");
	  food_qtd--;
	  add_piece();
	}
  }
}


int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0 ) return 1;

	//round fail
	printf("FPS, %i \n", 1000/FPS);

    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_HWSURFACE);
	head_img = SDL_LoadBMP("cube_red.bmp");
	tail_img = SDL_LoadBMP("cube_green.bmp");
	food_img = SDL_LoadBMP("cube_yellow.bmp");

	SDL_Event event;
	int count;
	int exit = 0;

	while(!exit){
	  count++;
	  Uint32 start_time= SDL_GetTicks();

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
				add_piece();
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

	  //arredondamento fail
	  SDL_FillRect(screen , NULL , 0x00000000);
	  move();
	  handle_colision();
	  draw_food();

	  SDL_UpdateRect(screen, 0, 0, 0, 0);

	  Uint32 current_time = SDL_GetTicks();
	  int expected_time =  1000/FPS;
	  unsigned int diff = current_time - start_time;

	  printf("DELAY: %i\n", expected_time - diff);
	  int delay = expected_time - diff;

	  if(delay < 0){
		delay = 0;
	  }

	  SDL_Delay(delay);
	}

    return 0;
}

