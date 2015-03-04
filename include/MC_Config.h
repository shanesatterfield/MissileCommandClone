#ifndef MC_CONFIG_H
#define MC_CONFIG_H

#include <SDL2/SDL.h>
#include <ScreenManager.h>

#define MC_RES_PATH "/home/shane/repos/MissileCommandClone/res/"

namespace mc
{
	extern SDL_Window* window;
	extern const int SCREEN_WIDTH;
	extern const int SCREEN_HEIGHT;
	extern SDL_Renderer* renderer;
	extern SDL_Event event;
	extern ScreenManager sm;
}

#endif
