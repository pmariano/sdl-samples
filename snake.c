#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL/SDL.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define FPS 20

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

SDL_Surface *screen, *head_img, *tail_img, *food_img;
int num_tail_img = 1;
// 1 up 2 down 3 left 4 right
int orientation = 4;
int vel = 2;
int ppi = 15;
int pieces_qtd = 1;
int food_qtd = 0;
int gameover = 0;
SDL_Rect pieces[50000] = {};
SDL_Rect food ;

void move(){
	int j , sumx, sumy;

	for(j = pieces_qtd-1; j >= 0;j--){
	  SDL_Rect* piece = &pieces[j];

	  if(j == 0){
		switch(orientation){
		  case UP:
			piece->y -= ppi;
			break;
		  case DOWN:
			piece->y += ppi;
			break;
		  case LEFT:
			piece->x -= ppi;
			break;
		  case RIGHT:
			piece->x += ppi;
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
  int r = rand() % max;

  //can I remove this if?
  if(r < min ){
	r += min;
  }

  return r;
}

void draw_food(){
  if(food_qtd == 0){
	food.x = generate_random(head_img->w,SCREEN_WIDTH);
	food.y = generate_random(head_img->h,SCREEN_HEIGHT);

	food_qtd++;
  }

  SDL_BlitSurface(food_img, NULL, screen, &food);
}

int collide(SDL_Rect a, SDL_Rect b, int hit_obj_area){
	int xdiff = abs(a.x - b.x);
	int ydiff = abs(a.y - b.y);

	if(xdiff <= hit_obj_area && ydiff <= hit_obj_area){
	  return 1;
	}
	return 0;
}

void feeding(SDL_Rect head){
  if(food_qtd > 0){
	if(collide(head, food, head_img->w)){
	  food_qtd--;
	  add_piece();
	}
  }
}

void hit_the_wall(SDL_Rect head){
  if ( head.x < head_img->w || head.x > (SCREEN_WIDTH - head_img->w)) {
	gameover = 1;
  }

  if ( head.y < head_img->h || head.y > (SCREEN_HEIGHT - head_img->h)) {
	gameover = 1;
  }
}

void hit_itself(SDL_Rect head){
  //start from the first tail
  int i;

  for(i = 1; i < pieces_qtd; i++){
	SDL_Rect piece = pieces[i];
	if(collide(head, piece, head_img->w - 1)){
	  gameover = 1;
	  break;
	}
  }
}

void handle_collision(){
  SDL_Rect head = pieces[0];
  hit_the_wall(head);
  hit_itself(head);
  feeding(head);
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

	//start point
	pieces[0].x = SCREEN_WIDTH / 2;
	pieces[0].y = SCREEN_HEIGHT / 2;


	while(!gameover){
	  count++;
	  Uint32 start_time= SDL_GetTicks();

	  if(SDL_PollEvent(&event)){
		switch(event.type) {
		  case SDL_QUIT:
			gameover= 1;
			break;
		  case SDL_KEYDOWN:
			switch(event.key.keysym.sym) {
			  case SDLK_ESCAPE:
			  case SDLK_q:
				gameover = 1;
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
	  handle_collision();
	  draw_food();

	  SDL_UpdateRect(screen, 0, 0, 0, 0);

	  Uint32 current_time = SDL_GetTicks();
	  int expected_time =  1000/FPS;
	  int diff = current_time - start_time;
	  int delay = abs(expected_time - diff);

	  SDL_Delay(delay);
	}

    return 0;
}

