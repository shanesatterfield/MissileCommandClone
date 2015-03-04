#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include <ScreenManager.h>
#include <TextureFactory.h>
#include <MC_Config.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

bool init();
bool failed( std::string errorMessage );
void close();

int main( int argc, char* argv[] )
{
	if( init() == false )
	{
		close();
		exit( EXIT_FAILURE );
	}

	//mc::ScreenManager mc::sm;
	if( mc::sm.init() == false )
	{
		close();
		exit( EXIT_FAILURE );
	}

	bool quit = false;
	while( quit == false )
	{
		while( SDL_PollEvent( &mc::event ) )
		{
			if( mc::event.type == SDL_QUIT )
				quit = true;
			else if( mc::event.type == SDL_KEYDOWN  	 ||
					 mc::event.type == SDL_MOUSEBUTTONUP ||
					 mc::event.type == SDL_MOUSEMOTION )
				mc::sm.handleEvent( mc::event );
		}

		SDL_RenderClear( mc::renderer );
		// SDL_RenderCopy( renderer, bitmapTex, nullptr, nullptr );

		mc::sm.update();
		mc::sm.render();

		SDL_RenderPresent( mc::renderer );
	}

	close();
	return 0;
}

bool init()
{
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	{
		return failed("Error: Failed to initialize SDL2.\n");
	}

	TTF_Init();
	IMG_Init( IMG_INIT_JPG | IMG_INIT_PNG );

	mc::window = SDL_CreateWindow( "Missile Command Clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mc::SCREEN_WIDTH, mc::SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	if( mc::window == nullptr )
	{
		return failed("Error: Failed to create window.\n");
	}

	mc::renderer = SDL_CreateRenderer( mc::window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	if( mc::renderer == nullptr )
	{
		return failed("Error: Failed to create renderer.\n");
	}

	srand( time(nullptr) );

	return true;
}

bool failed( std::string errorMessage )
{
	printf("%s", errorMessage.c_str());
	std::cout << "SDL2 Error: " << SDL_GetError() << std::endl;
	return false;
}

void close()
{
	if( mc::window != nullptr )
	{
		SDL_DestroyWindow( mc::window );
		mc::window = nullptr;
	}

	if( mc::renderer != nullptr )
	{
		SDL_DestroyRenderer( mc::renderer );
		mc::renderer = nullptr;
	}

	mc::TextureFactory::close();

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}
