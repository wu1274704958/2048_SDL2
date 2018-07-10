#define _CRT_SECURE_NO_WARNINGS
#include <SDL.h>
//#undef main
#include <SDL_ttf.h>
#include "Mat.h"
#include <iostream>
#include <map>
#include <vector>
#include <Windows.h>
#include <time.h>

#define K_W 140
#define Width 600
#define Height 700

int score = 0;

static bool NEED_DRAW = false;

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = nullptr;
static SDL_Event event;

static TTF_Font *font = NULL;
static SDL_Color white = { 255,255,255 };
static SDL_Color score_color = {64,0,64};

static SDL_Texture *bg = NULL;
static SDL_Texture *yuan_jiao_box = NULL;
static SDL_Texture *fg_box = NULL;

static SDL_Rect rect_bg = { 0,	0,Width ,Height };

static SDL_Rect rect_k[16];
static SDL_Rect rect_k_copy[16];


static float rect_word1[4] = {	0.14285714285714285714285714285714f,
								0.14285714285714285714285714285714f,
								0.71428571428571428571428571428571f,
								0.71428571428571428571428571428571f};

static float rect_word2[4] = {	0.12857142857142857142857142857143f,
								0.14285714285714285714285714285714f,
								0.74285714285714285714285714285714f,
								0.71428571428571428571428571428571f};

static float rect_word3[4] = {	0.11428571428571428571428571428571f,
								0.14285714285714285714285714285714f,
								0.77142857142857142857142857142857f,
								0.71428571428571428571428571428571f};

static float rect_word4[4] = {	0.1f,
								0.14285714285714285714285714285714f,
								0.8f,
								0.71428571428571428571428571428571f};

static SDL_mutex* mutex = NULL;

struct word
{
	float *rect;
	SDL_Texture *texture;
	SDL_Color color;
};

static Mat<int> data(4,4,0);
static Mat<int> last_data;
static int last_score_op;

static std::map<int,word *> map;


static int first[] = {	0,0,0,0,
						4,0,0,0,
						4,0,0,0,
						4,4,4,4
};


static int last_score = 0;
static SDL_Texture* last_score_texture = NULL;



int toBig(void *data)
{
	int i = (int)data;
	::SDL_LockMutex(mutex);

	rect_k[i].x += 60;
	rect_k[i].y += 60;
	rect_k[i].w = 20;
	rect_k[i].h = 20;

	::SDL_UnlockMutex(mutex);
	float s = 0.0f;

	while(rect_k[i].x > rect_k_copy[i].x)
	{
		float a = sinf(s) * 10;
		
		::SDL_LockMutex(mutex);
		rect_k[i].x -= a;
		rect_k[i].y -= a;
		rect_k[i].w += 2*a;
		rect_k[i].h += 2*a;
		NEED_DRAW = true;
		::SDL_UnlockMutex(mutex);

		

		s += 0.1f;
		::SDL_Delay(12);
	}

	::SDL_LockMutex(mutex);

	rect_k[i].x = rect_k_copy[i].x;
	rect_k[i].y = rect_k_copy[i].y;
	rect_k[i].w = rect_k_copy[i].w;
	rect_k[i].h = rect_k_copy[i].h;
	NEED_DRAW = true;
	::SDL_UnlockMutex(mutex);

	return 0;
}

int toBig_toSmall(void *data)
{
	int i = (int)data;
	float s = 1.5707963267948966192313216916398f;

	for (int j = 0;j < 20;j++)
	{
		float a = sinf(s);

		::SDL_LockMutex(mutex);
		rect_k[i].x -= (a*0.5f);
		rect_k[i].y -= (a*0.5f);
		rect_k[i].w += a*4.0f;
		rect_k[i].h += a*4.0f;
		NEED_DRAW = true;
		::SDL_UnlockMutex(mutex);



		s += 0.15707963267948966192313216916398f;
		::SDL_Delay(16);
	}

	::SDL_LockMutex(mutex);

	rect_k[i].x = rect_k_copy[i].x;
	rect_k[i].y = rect_k_copy[i].y;
	rect_k[i].w = rect_k_copy[i].w;
	rect_k[i].h = rect_k_copy[i].h;
	NEED_DRAW = true;
	::SDL_UnlockMutex(mutex);

	return 0;
}


//std::vector<SDL_Thread*> threads;

void createThread_toBig(int i)
{
	SDL_Thread* thread = SDL_CreateThread(toBig, "toBig", (void *)i);
	if (thread)
	{
		std::cout << thread << std::endl;
		//threads.push_back(thread);
		SDL_DetachThread(thread);
	}
}

void createThread_toBig_toSmall(int i)
{
	SDL_Thread* thread = SDL_CreateThread(toBig_toSmall, "toBig_toSmall", (void *)i);
	if (thread)
	{
		std::cout << thread << std::endl;
		//threads.push_back(thread);
		SDL_DetachThread(thread);
	}
}

int init()
{
	//data.setData(first,4,4);

	int x1 = 8,y1 = 108;
	for(int i = 0;i < 16;i++)
	{
		rect_k[i].x = x1;
		rect_k[i].y = y1;
		rect_k[i].h = K_W;
		rect_k[i].w = K_W;

		rect_k_copy[i].x = x1;
		rect_k_copy[i].y = y1;
		rect_k_copy[i].h = K_W;
		rect_k_copy[i].w = K_W;

		x1 += 148;
		if((i + 1) % 4 == 0)
		{
			x1 = 8;
			y1 += 148;
		}
	}
	
	if (0 != SDL_Init(SDL_INIT_EVERYTHING))
		return -1;
	window = SDL_CreateWindow("first", 30, 30, Width, Height, 0);
	if (window == NULL)
		return -2;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	/*if(renderer == 0)
	{
		renderer = SDL_CreateRenderer(window, -1,SDL_RENDERER_SOFTWARE);
	}*/

	mutex = ::SDL_CreateMutex();

	SDL_Surface *temp = SDL_LoadBMP("img/yuan_jiao_box.bmp");
	SDL_SetColorKey(temp, SDL_TRUE, SDL_MapRGB(temp->format, 0, 0, 0));
	yuan_jiao_box = SDL_CreateTextureFromSurface(renderer,temp);
	SDL_FreeSurface(temp);

	temp = SDL_LoadBMP("img/bg2.bmp");
	bg = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);

	temp = SDL_LoadBMP("img/fg_box.bmp");
	SDL_SetColorKey(temp, SDL_TRUE, SDL_MapRGB(temp->format, 255, 0, 0));
	fg_box = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);
	
	TTF_Init();
	font = TTF_OpenFont("C:\\WINDOWS\\Fonts\\SIMHEI.TTF", 140);

	char str[5] = {0};
	int color_g = 220;
	for (int i = 2; i <= 4096;)
	{
		word *w = new word();
		if (i < 10)
		{
			w->rect = rect_word1;
		}
		else if (i < 100)
		{
			w->rect = rect_word2;
		}
		else if (i < 1000)
		{
			w->rect = rect_word3;
		}
		else if (i < 10000)
		{
			w->rect = rect_word4;
		}
		
		w->color.r = 238;
		w->color.g = color_g;
		w->color.b = 0;

		color_g -= 17;

		sprintf(str, "%d", i);

		temp = ::TTF_RenderText_Blended(font, str, white);
		w->texture = SDL_CreateTextureFromSurface(renderer, temp);
		SDL_FreeSurface(temp);

		map.insert(std::make_pair(i, w));
		
		i *= 2;
	}
	return 0;
}



void destroy()
{
	if(last_score_texture)
	{
		SDL_DestroyTexture(last_score_texture);
	}
	auto it = map.begin();
	for(;it != map.end();it++)
	{
		if(it->second)
		{
			std::cout << it->first << std::endl;
			SDL_DestroyTexture(it->second->texture);
			delete it->second;
		}
	}
	
	::SDL_DestroyMutex(mutex);
	//Sleep(1000);

	TTF_CloseFont(font);
	TTF_Quit();
	
	SDL_DestroyTexture(fg_box);
	SDL_DestroyTexture(yuan_jiao_box);
	SDL_DestroyTexture(bg);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


void draw()
{
	SDL_RenderCopy(renderer, bg, NULL, &rect_bg);

	SDL_Rect r;

	r.y = 8;
	r.h = 84;
	//draw score
	if(score < 10) //1
	{
		r.x = 522;
		r.w = 58;
	}else if(score < 100)//2
	{
		r.x = 464;
		r.w = 116;
	}else if(score < 1000)//3
	{
		r.x = 406;
		r.w = 174;
	}else if(score < 10000)//4
	{
		r.x = 348;
		r.w = 232;
	}else if(score < 100000)//5
	{
		r.x = 300;
		r.w = 290;
	}

	if(score != last_score || last_score == 0 && last_score_texture == NULL )
	{
		
		if (last_score_texture)
		{
			::SDL_DestroyTexture(last_score_texture);
			last_score_texture = NULL;
		}
		
		
		char str[6] = {0};
		sprintf(str,"%d",score);

		last_score = score;
		SDL_Surface *temp = ::TTF_RenderText_Blended(font,str,score_color);

		last_score_texture = SDL_CreateTextureFromSurface(renderer,temp);

		SDL_FreeSurface(temp);
	}

	SDL_RenderCopy(renderer,last_score_texture,NULL,&r);
	//draw word and background
	
	word *w = NULL;

	for(int i = 0;i < 16;i++)
	{																				
		word *w = map[data[i]];														
		if(w == NULL)																	
		{																				
			//SDL_SetTextureColorMod(yuan_jiao_box, white.r, white.g, white.b);			
			//SDL_RenderCopy(renderer, yuan_jiao_box, NULL, &rect_k[i]);				
		}else{																			
			SDL_SetTextureColorMod(yuan_jiao_box, w->color.r, w->color.g, w->color.b);	
			SDL_RenderCopy(renderer, yuan_jiao_box, NULL, &rect_k[i]);
			
			SDL_LockMutex(mutex);

			r.x = rect_k[i].x + (rect_k[i].w * (w->rect)[0]);											
			r.y = rect_k[i].y + (rect_k[i].w * (w->rect)[1]);											
			r.w = rect_k[i].w * (w->rect)[2];																	
			r.h = rect_k[i].w * (w->rect)[3];	
			
			SDL_UnlockMutex(mutex);

			SDL_RenderCopy(renderer, w->texture, NULL, &r);	
			SDL_RenderCopy(renderer, fg_box, NULL, &rect_k[i]);
		}																				
																					
	}	

	SDL_RenderPresent(renderer);
}



#include "game.h"

typedef bool(*OperationFuncType)(Mat<int> &data, void(*draw)(), void(*createThread_toBig_toSmall)(int));

void to_step(Mat<int> &data, OperationFuncType func)
{
	last_score_op = score;
	Mat<int> temp(data);
	if (func(data,draw,createThread_toBig_toSmall))
	{
		last_data = std::move(temp);
		rand_add(data,createThread_toBig);
		draw();
	}
	else if (!rand_add_test(data))
	{
		std::cout << "die" << std::endl;
	}

}

int main(int args, char *argv[])
{
	if (::init() != 0)
	{
		return -1;
	}
	
	srand(time(NULL));


	rand_add(data,createThread_toBig);
	rand_add(data, createThread_toBig);

	draw();

	
	while (true) {
		if (SDL_PollEvent(&event)) 
		{
			if (event.type == SDL_QUIT) 
			{
				break;
			}
			if(event.type == SDL_KEYUP)
			{
				switch(event.key.keysym.sym)
				{
				case SDLK_UP:
						to_step(data,up);
					break;
				case SDLK_DOWN:
						to_step(data,down);
					break;
				case SDLK_LEFT:
					to_step(data,left);
					break;
				case SDLK_RIGHT:
					to_step(data,right);
					break;
				case SDLK_k:
					data = last_data;
					score = last_score_op;
					draw();
					break;
				default:
					break;
				}
			}
		}
		SDL_Delay(5);
		if(NEED_DRAW)
		{
			SDL_LockMutex(mutex);
			NEED_DRAW = false;
			SDL_UnlockMutex(mutex);
			draw();
			//std::cout << "draw " << std::endl;
			
		}
	}

	destroy();
	return 0;
}