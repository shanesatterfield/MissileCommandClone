#include <MC_Config.h>

SDL_Window* mc::window      = nullptr;
SDL_Renderer* mc::renderer  = nullptr;

const int mc::SCREEN_WIDTH  = 1280;
const int mc::SCREEN_HEIGHT = 720;

SDL_Event mc::event;
mc::ScreenManager mc::sm;
