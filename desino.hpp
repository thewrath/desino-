#ifndef DESINO_HPP
#define DESINO_HPP

#include <SDL2/SDL.h>

#include <iostream>

#include "helpers.cpp"

namespace Desino
{

	class Game
	{
	public:
		Game(){};
		~Game(){};
		bool init(){ return true; }; 
		bool update(){ return true; }; 
		void destroy(){}; 
		
	};

	class Engine
	{
	public:
		static Engine* get()
		{
			static Engine* instance;

			return instance; 
		}

		bool init(int w, int h, std::string t, Game* g)
		{
			if(!initSDL(w, h, t, this->window, this->renderer))
			{
				logSDLError(std::cout, "SDL init failed ");
				return false; 
			}

			//call at the end 
			//this->game = g; 
			if(this->game->init())
			{
				logSDLError(std::cout, "Game class init failed");
				return false;
			} 
			this->isInit = true; 
			return true;
		}

		void start()
		{
			if(!this->isInit)
			{	
				logSDLError(std::cout, "Engine not initialised");
			}
			else
			{
				this->update();
			}
		}

		
		Engine(Engine const &) = delete;
		void operator=(Engine const&) = delete; 
	private: 
		Engine()
		{
			this->window = nullptr; 
			this->renderer = nullptr;
		}; 

		void update()
		{
			if(!this->isInit)
			{	
				logSDLError(std::cout, "Engine not initialised");
			}
			else
			{
				while(!this->quit)
				{
					SDL_RenderClear(this->renderer);

					this->quit = !this->game->update();
				}
				this->game->destroy(); 
			}
		}

		bool isInit = false; 
		bool quit = false; 
		SDL_Window * window; 
		SDL_Renderer * renderer;
		Game * game = new Game(); 

	};

}; 


#endif 