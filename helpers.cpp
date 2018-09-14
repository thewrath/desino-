#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "cleanup.hpp" 

#include <iostream>
#include <ostream>

const std::string RESOURCE_PATH = "ressources";

/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message to
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(std::ostream &os, const std::string &msg)
{
	os << msg << " error: " << SDL_GetError() << std::endl;
}

/**
* Init and SDL window and Renderer with SDL extensions 
* @param w to initialise 
* @param r to initialise 
*/
bool initSDL(int width, int height, std::string title, SDL_Window*& w, SDL_Renderer*& r)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		logSDLError(std::cout, "SDL_Init");
		return false;
	}

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
		logSDLError(std::cout, "IMG_Init");
		SDL_Quit();
		return false;
	}

	if (TTF_Init() != 0){
		logSDLError(std::cout, "TTF_Init");
		SDL_Quit();
		return 1;
	}

	w = SDL_CreateWindow(title.c_str(), 100, 100, width, height, SDL_WINDOW_SHOWN); 
	
	if(w == nullptr)
	{
		logSDLError(std::cout, "SDL_CreateWindow");
		SDL_Quit();
		return false; 
	}
	
	r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); 
	if(r == nullptr)
	{
		logSDLError(std::cout, "SDL_CreateRenderer"); 
		cleanup(w); 
		SDL_Quit(); 
		return false; 
	}
	return true; 
}

/**
* Loads a BMP image into a texture on the rendering device
* @param file The BMP image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
{
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str()); 
	if(texture == nullptr)
	{
		logSDLError(std::cout, "LoadTexture"); 
	}
	return texture; 
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h)
{
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

/**
* Draw and SDL_Texture to an SDL_Renderer at position x, y, preserving 
* the texture's width and height 
* @param tex The source texture we want to draw 
* @param ren The renderer we want to draw to  
* @param x The x coordinate to draw to 
* @param y The y coordinate to draw to 
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y)
{
	int w, h; 
	SDL_QueryTexture(tex, NULL, NULL, &w, &h); 
	renderTexture(tex, ren, x, y, w, h); 
}

/**
* Get path of a resource
*
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
*/
std::string getResourcePath(const std::string &subDir = "")
{
	//We need to choose the path separator properly based on which
	//platform we're running on, since Windows uses a different
	//separator than most systems
	#ifdef _WIN32
		const char PATH_SEP = '\\';
	#else
		const char PATH_SEP = '/';
	#endif
		//This will hold the base resource path: Lessons/res/
		//We give it static lifetime so that we'll only need to call
		//SDL_GetBasePath once to get the executable path
		static std::string baseRes;
		if (baseRes.empty()){
			//SDL_GetBasePath will return NULL if something went wrong in retrieving the path
			char *basePath = SDL_GetBasePath();
			if (basePath){
				baseRes = basePath;
				SDL_free(basePath);
			}
			else {
				std::cerr << "Error getting resource path: " << SDL_GetError() << std::endl;
				return "";
			}
			//We replace the last bin/ with res/ to get the the resource path
			size_t pos = baseRes.rfind("bin");
			baseRes = baseRes.substr(0, pos) + RESOURCE_PATH + PATH_SEP;
		}
		//If we want a specific subdirectory path in the resource directory
		//append it to the base path. This would be something like Lessons/res/Lesson0
	return subDir.empty() ? baseRes : baseRes + subDir + PATH_SEP;
}

/**
* Render the message we want to display to a texture for drawing
* @param message The message we want to display
* @param fontFile The font we want to use to render the text
* @param color The color we want the text to be
* @param fontSize The size we want the font to be
* @param renderer The renderer to load the texture in
* @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
*/
SDL_Texture* renderText(const std::string &message, const std::string &fontFile,
	SDL_Color color, int fontSize, SDL_Renderer *renderer)
{
	//Open the font
	TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (font == nullptr){
		logSDLError(std::cout, "TTF_OpenFont");
		return nullptr;
	}	
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	if (surf == nullptr){
		TTF_CloseFont(font);
		logSDLError(std::cout, "TTF_RenderText");
		return nullptr;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr){
		logSDLError(std::cout, "CreateTexture");
	}
	//Clean up the surface and font
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	return texture;
}
