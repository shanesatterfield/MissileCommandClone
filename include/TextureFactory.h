#ifndef TEXTUREFACTORY_H
#define TEXTUREFACTORY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <map>

#include <MC_Config.h>

namespace mc
{
	class TextureFactory
	{
	public:
		static SDL_Texture* loadTexture( std::string name, std::string path );
		static bool isLoadedTexture( std::string name );
		static SDL_Texture* getTexture( std::string name );

		static void close();

	protected:
		static std::map< std::string, SDL_Texture*> textureMap;
	};
}

#endif