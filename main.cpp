#include "desino.hpp"

using namespace Desino;

int main(int argc, char const *argv[])
{
	Game game; 
	Engine* e = Engine::get();
	if(!e->init(800, 600, "Test", &game))
	{
		return 1; 
	}

	e->start(); 

	return 0;
}