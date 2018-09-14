#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <ostream>
#include <string>

#include "helpers.cpp"
#include "cleanup.hpp"

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int TILE_SIZE = 40;

const std::string TITLE = "Hello world";

int main(int argc, char const *argv[])
{

	bool quit = false;

	

	const std::string resPath = getResourcePath("assets"); 


	//load textures 
	SDL_Texture *background = loadTexture(resPath + "background.bmp", renderer); 
	SDL_Texture *image = loadTexture(resPath + "image.png", renderer); 

	//load font
	SDL_Color color = { 255, 255, 255, 255 }; 
	SDL_Texture *text = renderText("TTF fonts are cools ", resPath + "sample.ttf", color, 64, renderer); 
	if(text == nullptr)
	{
		cleanup(renderer, window); 
		TTF_Quit(); 
		SDL_Quit(); 
		return 1; 
	}

	if(background == nullptr || image == nullptr)
	{
		cleanup(background, image, renderer, window); 
		SDL_Quit(); 
		return 1; 
	}

	while(!quit)
	{
		SDL_Event event; 
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
			{	
				quit = true; 
			}
		}

		

		int bW, bH;
		SDL_QueryTexture(background, NULL, NULL, &bW, &bH);
		renderTexture(background, renderer, 0, 0);
		renderTexture(background, renderer, bW, 0);
		renderTexture(background, renderer, 0, bH);
		renderTexture(background, renderer, bW, bH);

		int iW, iH;
		SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
		int x = WINDOW_WIDTH / 2 - iW / 2;
		int y = WINDOW_HEIGHT / 2 - iH / 2;
		renderTexture(image, renderer, x, y);

		renderTexture(text, renderer, x, y); 

		SDL_RenderPresent(renderer);
	} 

	cleanup(background, image, renderer, window);
	SDL_Quit();

	return 0;
}

